#!/bin/sh

CPUFREQ_TABLE_DETAIL=$(cat ./cpufreq_table_detail.txt)
CPUFREQ_TABLE_VF=$(cat ./cpufreq_table_vf.txt)
GOV_NODE="/sys/devices/system/cpu/cpu0/cpufreq/scaling_governor"
GOV_SPEED_NODE="/sys/devices/system/cpu/cpu0/cpufreq/scaling_setspeed"
SCALING_MAX_FREQ="/sys/devices/system/cpu/cpu0/cpufreq/scaling_max_freq"
SCALING_MIN_FREQ="/sys/devices/system/cpu/cpu0/cpufreq/scaling_min_freq"
CPUINFO_MAX_FREQ="/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq"
CPUINFO_MIN_FREQ="/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_min_freq"
CUR_FREQ="/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_cur_freq"
CPUFREQ_ARRAY_DETAIL_NUM=0

HOTPLUG_LOCK_FILE="/sys/devices/system/cpu/cpufreq/fantasys/hotplug_lock"

LAST_GOV=
LAST_MAX_FREQ=
LAST_MIN_FREQ=
LAST_HOTPLUG_LOCK=
LAST_CUR_FREQ=


# echo ${CPUFREQ_TABLE_DETAIL}
# echo ${CPUFREQ_TABLE_VF}


# initialize CPUFREQ_ARRAY_DETAIL, CPUFREQ_ARRAY_DETAIL_NUM
i=0
for tmp in ${CPUFREQ_TABLE_DETAIL}; do
	CPUFREQ_ARRAY_DETAIL[${i}]=${tmp}
	i=$((i+1))
done
CPUFREQ_ARRAY_DETAIL_NUM=${i}


i=0
for tmp in ${CPUFREQ_TABLE_VF}; do
	CPUFREQ_ARRAY_VF[${i}]=${tmp}
	i=$((i+1))
done
CPUFREQ_ARRAY_VF_NUM=${i}


# -------------------- function implementation --------------------------

function backup_env()
{
	LAST_GOV=$(cat ${GOV_NODE})
	LAST_MAX_FREQ=$(cat ${SCALING_MAX_FREQ})
	LAST_MIN_FREQ=$(cat ${SCALING_MIN_FREQ})

	LAST_HOTPLUG_LOCK=$(cat ${HOTPLUG_LOCK_FILE})
	LAST_CUR_FREQ=$(cat ${CUR_FREQ})
	echo "last governor = ${LAST_GOV}, max freq=${LAST_MAX_FREQ}, min freq=${LAST_MIN_FREQ}"
	echo "last hotplug_lock= ${LAST_HOTPLUG_LOCK}, last cpuinfo_cur_freq=${CUR_FREQ}"
}

function restore_env()
{
	# restore governor
	echo ${LAST_GOV} > ${GOV_NODE}

	# restore scaling_max_freq scaling_min_freq
	min_freq=$(cat ${SCALING_MIN_FREQ})
	max_freq=$(cat ${SCALING_MAX_FREQ})

	if [ ${LAST_MAX_FREQ} -lt ${min_freq} ]; then
		echo ${LAST_MIN_FREQ} > ${SCALING_MIN_FREQ}
		echo ${LAST_MAX_FREQ} > ${SCALING_MAX_FREQ}
	else
		echo ${LAST_MAX_FREQ} > ${SCALING_MAX_FREQ}
		echo ${LAST_MIN_FREQ} > ${SCALING_MIN_FREQ}
	fi

	# restore hotplug_lock
	echo ${LAST_HOTPLUG_LOCK} > ${HOTPLUG_LOCK_FILE}

	echo "restore env finished!"
}

# Function: set cpu max freq and min freq to same freq
function set_cpufreq()
{
	min_freq=$(cat ${SCALING_MIN_FREQ})
	max_freq=$(cat ${SCALING_MAX_FREQ})

	if [ ${1} -lt ${min_freq} ]; then
		echo ${1} > ${SCALING_MIN_FREQ}
		echo ${1} > ${SCALING_MAX_FREQ}
	else
		echo ${1} > ${SCALING_MAX_FREQ}
		echo ${1} > ${SCALING_MIN_FREQ}
	fi
}


# Function: switch freq as interval_s for loop_num, freq get from CPUFREQ_TABLE_VF
# Usage: freq_switch_order <interval_s> <loop_num>
function freq_switch_order()
{
	interval_s=${1}
	loop_num=${2}
	if [ ${#} -ne 2 ]; then
		echo "ERROR Usage: freq_switch_order <interval_s> <loop_num>"
		echo "Use default value: interval_s=1 loop_num=10"
		interval_s=1
		loop_num=10
	fi

	echo "start exec freq_switch_order ${interval_s} ${loop_num}"

	last_max_freq=$(cat ${SCALING_MAX_FREQ})
	last_min_freq=$(cat ${SCALING_MIN_FREQ})

	for loop in $(busybox seq 1 ${loop_num}); do
		echo ""
		echo "===============freq_switch_order loop num: ${loop}"
		echo ""

		i=0
		for cf in ${CPUFREQ_TABLE_VF}; do
			i=$((i+1));

			# echo ${cf} > ${GOV_SPEED_NODE}
			echo "set cpu freq to: [${index}] ${cf}"
			set_cpufreq ${cf}

			sleep ${interval_s}

			cur_freq=$(cat ${CUR_FREQ})

			# echo "----${i}, cf=${cf}, cur_freq=${cur_freq}"

			if [ "${cf}" != "${cur_freq}" ]; then
				echo "index=${i}, setspeed failed, cur_freq=${cur_freq}, want_freq=${cf}"
			fi
		done
	done

	echo "go back scaling_max_freq=${last_max_freq} scaling_min_freq=${last_min_freq}"

	if [ last_max_freq -lt $(cat ${SCALING_MIN_FREQ}) ]; then
		echo ${last_min_freq} > ${SCALING_MIN_FREQ}
		echo ${last_max_freq} > ${SCALING_MAX_FREQ}
	else
		echo ${last_max_freq} > ${SCALING_MAX_FREQ}
		echo ${last_min_freq} > ${SCALING_MIN_FREQ}
	fi

	echo "end exec freq_switch_order ${interval_s} ${loop_num}."
}


# Function: switch freq as interval_s for loop_num, freq get from random
# Usage: freq_switch_random <interval_s> <loop_num>
function freq_switch_random()
{
	interval_s=${1}
	loop_num=${2}
	if [ ${#} -ne 2 ]; then
		echo "ERROR Usage: freq_switch_random <interval_s> <loop_num>"
		echo "Use default value: interval_s=1 loop_num=10"
		interval_s=1
		loop_num=10
	fi

	echo "start exec freq_switch_random ${interval_s} ${loop_num}"

	last_max_freq=$(cat ${SCALING_MAX_FREQ})
	last_min_freq=$(cat ${SCALING_MIN_FREQ})

	for loop in $(busybox seq 1 ${loop_num}); do
		echo ""
		echo "===============freq_switch_random loop num: ${loop}"
		echo ""

		index=$((${RANDOM}%${CPUFREQ_ARRAY_DETAIL_NUM}))
		cf=${CPUFREQ_ARRAY_DETAIL[${index}]}

		echo "set cpu freq to: [${index}] ${cf}"
		set_cpufreq ${cf}

		sleep ${interval_s}

		cur_freq=$(cat ${CUR_FREQ})

		# echo "----${i}, cf=${cf}, cur_freq=${cur_freq}"

		if [ "${cf}" != "${cur_freq}" ]; then
			echo "index=${loop}, setspeed failed, cur_freq=${cur_freq}, want_freq=${cf}"
		fi
	done

	echo "go back scaling_max_freq=${last_max_freq} scaling_min_freq=${last_min_freq}"

	if [ last_max_freq -lt $(cat ${SCALING_MIN_FREQ}) ]; then
		echo ${last_min_freq} > ${SCALING_MIN_FREQ}
		echo ${last_max_freq} > ${SCALING_MAX_FREQ}
	else
		echo ${last_max_freq} > ${SCALING_MAX_FREQ}
		echo ${last_min_freq} > ${SCALING_MIN_FREQ}
	fi

	echo "end exec freq_switch_random ${interval_s} ${loop_num}."
}

# Function: up/down cpu as interval_s for loop_num
# Usage: cpu_hotplug <interval_s> <loop_num>
function cpu_hotplug()
{
	interval_s=${1}
	loop_num=${2}
	if [ ${#} -ne 2 ]; then
		echo "ERROR Usage: freq_switch_random <interval_s> <loop_num>"
		echo "Use default value: interval_s=1 loop_num=10"
		interval_s=1
		loop_num=10
	fi

	echo "start cpu_hotplug interval_s=${interval_s} loop_num=${loop_num}"

	last_hotplug_lock=$(cat ${HOTPLUG_LOCK_FILE})

	for loop in $(busybox seq 1 ${loop_num}); do
		for cpu_core in 0 1 2; do
			echo ${cpu_core} > ${HOTPLUG_LOCK_FILE}

			sleep ${interval_s}

			cur_cpu_core=$(cat ${HOTPLUG_LOCK_FILE})

			# echo "----${loop}, cpu_core=${cpu_core}, cur_cpu_core=${cur_cpu_core}"

			if [ ${cur_cpu_core} -ne ${cpu_core} ]; then
				echo "loop=${loop}, cpu_hotplug failed, cur_cpu_core=${cur_cpu_core}, want_cpu_core=${cpu_core}"
			fi
		done
	done

	echo ${last_hotplug_lock} > ${HOTPLUG_LOCK_FILE}

	echo "end cpu_hotplug interval_s=${interval_s} loop_num=${loop_num}"
}

function dhcpcd_switch()
{
	loop_num=${1}
	if [ ${#} -ne 1 ]; then
		echo "ERROR Usage: dhcpcd_switch <loop_num>"
		echo "Use default value: loop_num=10"
		loop_num=10
	fi

	for i in `busybox seq 1 ${loop_num}`; do
		/system/bin/dhcpcd -aABDKL wlan0 &
		cat /proc/meminfo
		busybox free
		sleep 5
		ps | grep dhcp | busybox awk '{print $2}' | busybox xargs kill
		sleep 3
		echo "----test dhcp switch $i times"
	done
}


function exit_handler()
{
	restore_env
	echo "catch signal and exit exit_handler...!"
	exit 0
}

# --------------------------- main process ----------------------------

backup_env

trap exit_handler SIGINT SIGTERM SIGQUIT

echo ""
echo ""
echo "==============start freq_switch_order 0.05 2000"
echo ""
echo ""

freq_switch_order 0.05 100000

echo ""
echo ""
echo "==============start freq_switch_random 0.05 100000"
echo ""
echo ""
freq_switch_random 0.05 100000

echo ""
echo ""
echo "==============start freq_switch_random 1 10000"
echo ""
echo ""
freq_switch_random 1 10000

echo ""
echo ""
echo "==============start freq_switch_order 1 2000"
echo ""
echo ""
freq_switch_order 1 2000

echo ""
echo ""
echo "==============start cpu_hotplug 0.5 5000"
echo ""
echo ""
cpu_hotplug 0.5 5000

echo ""
echo ""
echo "==============start freq_switch_order 7200 1"
echo ""
echo ""
freq_switch_order 7200 1

echo ""
echo ""
echo "==============start dhcpcd_switch 5000"
echo ""
echo ""
# dhcpcd_switch 5000


echo "cpufreq test finished!..."

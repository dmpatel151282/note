#!/bin/sh

usage() 
{
  cat <<-EOF >&2

  usage: ${0##*/} [ -f testname ] [ -i interval ] [ -n loop_num]
              
  -f testname   freq_switch  cpu_hotplug  dhcpcd_switch.
  -i interval   The interval of loop. unit is second.    
  -n loop_num   The count of cycle.
  -s sequence   just for freq_switch, 1 for order, 0 for random.
  
  example:
    -f freq_switch -n 100 -i 0.5 -s 1
    -f cpu_hotplug -n 100 -i 0.5 
    -f dhcpcd_switch -n 100

	EOF
exit 0
}

function backup_env()
{
	LAST_GOV=$(cat ${GOV_NODE})
	LAST_MAX_FREQ=$(cat ${SCALING_MAX_FREQ})
	LAST_MIN_FREQ=$(cat ${SCALING_MIN_FREQ})

	LAST_HOTPLUG_LOCK=$(cat ${HOTPLUG_LOCK_FILE})
	LAST_CUR_FREQ=$(cat ${CUR_FREQ})
	echo "last governor = ${LAST_GOV}, max freq=${LAST_MAX_FREQ}, min freq=${LAST_MIN_FREQ}"
	echo "last hotplug_lock= ${LAST_HOTPLUG_LOCK}, last cpuinfo_cur_freq=${LAST_CUR_FREQ}"
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
# Usage: freq_switch <interval_s> <loop_num> <sequence>
function freq_switch()
{
	interval_s=${1}
	loop_num=${2}
    sequence=${3}
	if [ $# -ne 3 ]; then
		echo "ERROR Usage: freq_switch <interval_s> <loop_num> <sequence>"
        exit $E_ARGS
	fi

	echo "start exec freq_switch interval=${interval_s} loop_num=${loop_num} sequence=${sequence}"

	last_max_freq=$(cat ${SCALING_MAX_FREQ})
	last_min_freq=$(cat ${SCALING_MIN_FREQ})

	for loop in $(busybox seq 1 ${loop_num}); do
        if [ $sequence -eq 1 ]; then
		    echo ""
		    echo "===============freq_switch order loop num: ${loop}"
		    echo ""
            index=$((${loop}%${CPUFREQ_ARRAY_DETAIL_NUM}))
            cf=${CPUFREQ_ARRAY_DETAIL[${index}]}
        else  
            echo ""
		    echo "===============freq_switch random loop num: ${loop}"
		    echo ""
            index=$((${RANDOM}%${CPUFREQ_ARRAY_DETAIL_NUM}))
            cf=${CPUFREQ_ARRAY_DETAIL[${index}]}
        fi

		echo "set cpu freq to: [${index}] ${cf}"
		set_cpufreq ${cf}

		sleep ${interval_s}

		cur_freq=$(cat ${CUR_FREQ})

		if [ "${cf}" != "${cur_freq}" ]; then
			echo "index=${loop}, setspeed failed, cur_freq=${cur_freq}, want_freq=${cf}"
            exit $E_FREQ_SWITCH
		fi
	done

	echo "go back scaling_max_freq=${last_max_freq} scaling_min_freq=${last_min_freq}"

	if [ $last_max_freq -lt `cat ${SCALING_MIN_FREQ}` ]; then
		echo ${last_min_freq} > ${SCALING_MIN_FREQ}
		echo ${last_max_freq} > ${SCALING_MAX_FREQ}
	else
		echo ${last_max_freq} > ${SCALING_MAX_FREQ}
		echo ${last_min_freq} > ${SCALING_MIN_FREQ}
	fi

	echo "end freq_switch ${interval_s} ${loop_num} sequence=${sequence}."
}

function cpu_hotplug()
{
	interval_s=${1}
	loop_num=${2}
	if [ $# -ne 2 ]; then
		echo "ERROR Usage: freq_switch_random <interval_s> <loop_num>"
		echo "Use default value: interval_s=1 loop_num=10"
		interval_s=1
		loop_num=10
	fi

	echo "start cpu_hotplug interval_s=${interval_s} loop_num=${loop_num}"

	last_hotplug_lock=$(cat ${HOTPLUG_LOCK_FILE})

	for loop in $(busybox seq 1 ${loop_num}); do
        echo "$loop cycle"
		for cpu_core in 0 1 2; do
			echo ${cpu_core} > ${HOTPLUG_LOCK_FILE}

			sleep ${interval_s}

			cur_cpu_core=$(cat ${HOTPLUG_LOCK_FILE})

			# echo "----${loop}, cpu_core=${cpu_core}, cur_cpu_core=${cur_cpu_core}"

			if [ ${cur_cpu_core} -ne ${cpu_core} ]; then
				echo "loop=${loop}, cpu_hotplug failed, cur_cpu_core=${cur_cpu_core}, want_cpu_core=${cpu_core}"
                exit $E_CPU_HOTPLUG
			fi
		done
	done

	echo ${last_hotplug_lock} > ${HOTPLUG_LOCK_FILE}

	echo "end cpu_hotplug interval_s=${interval_s} loop_num=${loop_num}"
}

function dhcpcd_switch()
{
	loop_num=${1}
	if [ $# -ne 1 ]; then
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
	exit $E_SIGNAL
}

function main() {
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
    
    E_ARGS=10
    E_SIGNAL=9
    E_FREQ_SWITCH=8
    E_CPU_HOTPLUG=7
    E_DHCPCD=6

    KMSG_LOGNAME="log.txt"

    while getopts f:i:n:s: arg
    do  case $arg in
        f)  TEST_NAME=$OPTARG;;
  
        i)  INTERVAL=$OPTARG;;
  
        n)  LOOP_NUM=$OPTARG;;
  
        s)  SEQUENCE=$OPTARG;;
  
        \?) usage;;
        esac
    done
  
    [ -z $TEST_NAME ] && \
    {
        echo "must have -f parameter" 
        usage
    }
  
    #initialize CPUFREQ_ARRAY_DETAIL, CPUFREQ_ARRAY_DETAIL_NUM
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

    backup_env
  
    if [ "$TEST_NAME" == "freq_switch" ]
    then
        freq_switch $INTERVAL $LOOP_NUM $SEQUENCE
    elif [ "$TEST_NAME" == "cpu_hotplug" ]
    then
        cpu_hotplug $INTERVAL $LOOP_NUM
    elif [ "$TEST_NAME" == "dhcpcd_switch" ]
    then
        dhcpcd_switch $LOOP_NUM
    else
        echo "unknow test name"
        usage
        exit $E_ARGS
    fi

    restore_env
    echo "cpufreq test finished!"
}

trap exit_handler SIGINT SIGTERM SIGQUIT
main "$@"

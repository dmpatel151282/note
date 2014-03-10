#!/bin/sh

CUR_FILE_PATH=`dirname $0`

echo ""
echo "********* install Livesuit *******************"
apt-get install dkms --force-yes -y
chmod +x ${CUR_FILE_PATH}/livesuit/LiveSuit.run
${CUR_FILE_PATH}/livesuit/LiveSuit.run

rule="/etc/udev/rules.d/10-local.rules"
if [ ! -e "$rule" ]; then
    echo "SUBSYSTEM!=\"usb_device\", ACTION!=\"add\", GOTO=\"objdev_rules_end\"
#USBasp
ATTRS{idVendor}==\"1f3a\", ATTRS{idProduct}==\"efe8\", GROUP=\"liushui\",
MODE=\"0666\"
LABEL=\"objdev_rules_end\"" > $rule

    sudo service udev restart
fi

#cp ${CUR_FILE_PATH}/livesuit/LiveSuit_unformat ${USER}/Bin/LiveSuit/bin/LiveSuit -v
#chmod 755 ${USER}/Bin/LiveSuit/bin/LiveSuit

echo "*********** install LiveSuit successful! *************"

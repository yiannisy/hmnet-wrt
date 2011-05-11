#!/bin/sh

uci set openflow.@ofswitch[0].ofctl=$1
uci commit
#/etc/init.d/openflow restart &

OFCTRL="$(uci get openflow.@ofswitch[0].ofctl)"
echo $OFCTRL
exit 0


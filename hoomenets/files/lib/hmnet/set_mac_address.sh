#!/bin/sh

if [ $1 = "FF:FF:FF:FF:FF:FF" ]; then
	# no need for special mac, roll-back to original.
	echo "no need for special mac"
	uci delete network.wan.macaddr
else
	uci set network.wan.macaddr=$1
	MAC="$(uci get network.wan.macaddr)"
	echo $MAC
fi
uci commit
exit 0


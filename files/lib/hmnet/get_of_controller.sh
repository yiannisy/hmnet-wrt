#!/bin/sh

OFCTRL="$(uci get openflow.@ofswitch[0].ofctl)"
echo $OFCTRL
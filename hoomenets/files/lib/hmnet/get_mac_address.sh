#!/bin/sh

MAC="$(uci get network.wan.macaddr)"

if [ "$MAC" = "" ]; then
	MAC="FF:FF:FF:FF:FF:FF"
fi
echo $MAC

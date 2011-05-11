#!/bin/sh

uci set homenets.homenets_ap.conf_server=$1
uci commit

CONF_SERVER="$(uci get homenets.homenets_ap.conf_server)"
echo $CONF_SERVER
exit 0


#!/bin/sh

uci set homenets.homenets_ap.version="$1"
uci set homenets.homenets_ap.img_filename="$2"
uci set homenets.homenets_ap.img_url="$3"
uci commit homenets
exit 0


#!/bin/sh

uci set homenets.homenets_ap.version="$1"
uci set homenets.homenets_ap.img_filename="$2"
uci set homenets.homenets_ap.img_url="$3"
exit 0


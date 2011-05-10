#!/bin/sh

uci set homenets.homenets_ap.version="$1"
uci set homenets.homenets_ap.img_filename="$2"
exit 0


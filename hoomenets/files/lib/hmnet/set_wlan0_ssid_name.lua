#!/usr/bin/lua

wlan_name="wlan0"
ssid_name=arg[1]
require("uci")
x = uci.cursor()

x:set("wireless",wlan_name,"ssid",ssid_name)
x:set("wireless",wlan_name,"hidden",0)

x:commit("wireless")
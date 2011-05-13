#!/usr/bin/lua

wlan_name="wlan0"
default_ssid = "wlan0-ssid"

require("uci")
x = uci.cursor()

x:set("wireless",wlan_name,"wifi-iface")
x:set("wireless",wlan_name,"device","radio0")
x:set("wireless",wlan_name,"mode","ap")
x:set("wireless",wlan_name,"ssid",default_ssid)
x:set("wireless",wlan_name,"hidden",1)
x:set("wireless",wlan_name,"encryption","none")
x:commit("wireless")
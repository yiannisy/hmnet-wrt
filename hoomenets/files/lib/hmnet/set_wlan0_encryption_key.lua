#!/usr/bin/lua

wlan_name="wlan0"
encryption_key=arg[1]
require("uci")
x = uci.cursor()

x:set("wireless",wlan_name,"key",encryption_key)

x:commit("wireless")
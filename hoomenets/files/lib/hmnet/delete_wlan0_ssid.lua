#!/usr/bin/lua

wlan_name="wlan0"

require("uci")
x = uci.cursor()

x:delete("wireless",wlan_name)

x:commit("wireless")
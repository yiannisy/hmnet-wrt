#!/usr/bin/lua

wlan_name="wlan2"

require("uci")
x = uci.cursor()

x:delete("wireless",wlan_name)

x:commit("wireless")
#!/usr/bin/lua

value=arg[1]

require("uci")
x = uci.cursor()

x:set("wireless","wlan0-1","ssid",value)
x:commit("wireless")
#!/usr/bin/lua

value=arg[1]

require("uci")
x = uci.cursor()

x:set("wireless","wlan0-2","encryption",value)
x:commit("wireless")
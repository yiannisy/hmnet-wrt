#!/usr/bin/lua

value=arg[1]

require("uci")
x = uci.cursor()

x:set("wireless","wlan2","key",value)
x:commit("wireless")
#!/usr/bin/lua

require("uci")
x = uci.cursor()

ret = x:get("wireless","wlan0-2","key")
print (ret)
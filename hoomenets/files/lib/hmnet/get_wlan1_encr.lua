#!/usr/bin/lua

require("uci")
x = uci.cursor()

ret = x:get("wireless","wlan1","encryption")
print (ret)
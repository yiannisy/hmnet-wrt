#!/usr/bin/lua

require("uci")
x = uci.cursor()

ret = x:get("wireless","wlan2","encryption")
print (ret)
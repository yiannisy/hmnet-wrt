#!/usr/bin/lua

config=arg[1]
section=arg[2]
option=arg[3]

require("uci")
x = uci.cursor()

ret = x:get(config,section,option)
print (ret)

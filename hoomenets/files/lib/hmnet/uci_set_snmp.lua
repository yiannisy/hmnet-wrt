#!/usr/bin/lua

config=arg[1]
section=arg[2]
option=arg[3]
value=arg[4]

require("uci")
x = uci.cursor()

x:set(config,section,option,value)
x:commit(config)
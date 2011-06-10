#!/usr/bin/lua

require("uci")
x = uci.cursor()

print ("Restarting HomeNets!")

os.execute("/etc/init.d/hmnet restart")

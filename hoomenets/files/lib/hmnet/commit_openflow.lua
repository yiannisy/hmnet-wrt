#!/usr/bin/lua

require("uci")
x = uci.cursor()

print ("Restarting OpenFlow!")

os.execute("/etc/init.d/openflow restart")

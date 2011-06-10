#!/usr/bin/lua

require("uci")
x = uci.cursor()

print ("Restarting Wireless!")

os.execute("wifi")
os.execute("/etc/init.d/openflow restart")

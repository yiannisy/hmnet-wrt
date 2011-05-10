#!/usr/bin/lua

wlan_name="wlan1"
encryption_type=arg[1]

print(encryption_type)

require("uci")
x = uci.cursor()

x:set("wireless",wlan_name,"encryption",encryption_type)

x:commit("wireless")

os.execute("wifi")

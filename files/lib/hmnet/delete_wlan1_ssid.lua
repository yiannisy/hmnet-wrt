#!/usr/bin/lua

wlan_name="wlan1"

require("uci")
x = uci.cursor()

x:delete("wireless",wlan_name)
x:commit("wireless")

os.execute("wifi")

-- TODO : remove from ofports
#!/usr/bin/lua

wlan_name="wlan1"
default_ssid = "wlan1-ssid"

require("uci")
x = uci.cursor()

x:set("wireless",wlan_name,"wifi-iface")
x:set("wireless",wlan_name,"device","radio0")
x:set("wireless",wlan_name,"mode","ap")
x:set("wireless",wlan_name,"ssid",default_ssid)
x:set("wireless",wlan_name,"hidden",1)
x:set("wireless",wlan_name,"encryption","none")
x:commit("wireless")

ofports = x:get("openflow","switch0","ofports")
ofports = ofports .. " " .. wlan_name
x:set("openflow", "switch0", "ofports", ofports)
x:commit("openflow")

os.execute("wifi")
os.execute("/etc/init.d/openflow restart")
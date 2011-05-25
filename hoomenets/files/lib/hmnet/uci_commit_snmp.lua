#!/usr/bin/lua

config=arg[1]

require("uci")
x = uci.cursor()

print ("Restarting " .. config .. "!")

if config == "wireless" then
   os.execute("wifi")
   os.execute("/etc/init.d/openflow restart")
elseif config == "openflow" then
   os.execute("/etc/init.d/openflow restart")
elseif config == "reboot" then
   os.execute("reboot")
end
module("trigger.homenets", package.seeall)
require("uci.trigger")

uci.trigger.add {
	{
		id = "hmnet_config_restart",
		title = "Restart the hmnet daemon",
		package = "homenets",
		action = uci.trigger.service_restart("hmnet")
	},
	{
		id = "hmnet_network_restart",
		title = "Restart the hmnet daemon",
		package = "network",
		action = uci.trigger.service_restart("hmnet")
	},
}


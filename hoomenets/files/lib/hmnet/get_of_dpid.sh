#!/bin/sh

OFDPID="$(uci get openflow.@ofswitch[0].dpid)"
echo $OFDPID

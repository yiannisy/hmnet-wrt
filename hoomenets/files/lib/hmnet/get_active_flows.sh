#!/bin/sh

ACTIVE_FLOWS=`dpctl dump-flows tcp:127.0.0.1 | grep "duration_sec" -c`
echo $ACTIVE_FLOWS
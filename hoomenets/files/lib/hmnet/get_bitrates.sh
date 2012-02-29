#!/bin/sh

# write the bitrates log filename using the serial number of the device (e.g. MAC address)
mac_key=`ifconfig eth0.1 | awk '/eth/ { print $5 }'`	
serial_no=${mac_key//:/}
LOGFILE="/tmp/bitrates-${serial_no}.log"
i=0
SECS_PER_FILE=300
echo "Start dumping bitrates in $LOGFILE"
echo "date,mac,inactive,tx_bitrate,rx_bitrate" > $LOGFILE
while true;
do
	ts=`date +"%Y%m%d%H%M%S"`
	iw dev wlan0 station dump | awk '{if($1=="Station"){printf("%s,%s,","'$ts'",$2)}else if($1=="inactive" && $2=="time:"){printf("%s%s,",$3,$4)}else if($1=="tx" && $2=="bitrate:"){printf("%s,",$3)}else if($1=="rx" && $2=="bitrate:"){printf("%s\n",$3)}}' >> $LOGFILE
	sleep 1;
	let "i+=1"
	if [ $i == $SECS_PER_FILE ]; then
		cp $LOGFILE ${LOGFILE}.${ts} 
		scp -i /root/.ssh/apreport_rsa ${LOGFILE}.${ts} apreport@homenets.stanford.edu:~/bitrate-logs/
		rm ${LOGFILE}.${ts}
		echo "date,mac,inactive,tx_bitrate,rx_bitrate" > $LOGFILE
		i=0
	fi
done


#!/bin/sh

SERVER_URL=$1
IMAGE_FILENAME=$2
IMAGE_VERSION=$3
EXTENSION_BIN="bin"
EXTENSION_SIG="sig"

BIN_URL="${SERVER_URL}/${IMAGE_FILENAME}-${IMAGE_VERSION}.$EXTENSION_BIN"
SIG_URL="${SERVER_URL}/${IMAGE_FILENAME}-${IMAGE_VERSION}.$EXTENSION_SIG"

BIN_FILENAME="${IMAGE_FILENAME}-${IMAGE_VERSION}.$EXTENSION_BIN"
SIG_FILENAME="${IMAGE_FILENAME}-${IMAGE_VERSION}.$EXTENSION_SIG"

current_version=`cat /var/run/hmnet_version`

if [ $current_version = $IMAGE_VERSION ]; then
	echo "already at that version - skipping update..."
else
	echo "updating to version $IMAGE_VERSION"

	cd /tmp/

	rm *.sig
	rm *.bin

	wget ${BIN_URL}
	wget ${SIG_URL}

	echo "downloaded binary and signature"
	echo "checking ${BIN_FILENAME} against ${SIG_FILENAME}"
	md5sum -c --status ${SIG_FILENAME}
	sigcheck=$?
	
	if [ $sigcheck -eq 0  ]; then
		echo "signature verified - installing image..."
	else
		echo "Invalid signature - skipping..."
	fi
fi
#!/bin/bash

ARDUINO_PROTO_DIR=./arduino/protocol
NANOPBC=./script/nanopb-protoc.sh

FILES=`find protocol -name "*.proto"`

for file in $FILES; do
	$NANOPBC $file $ARDUINO_PROTO_DIR
done

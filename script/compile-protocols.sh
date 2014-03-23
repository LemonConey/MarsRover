#!/bin/bash

PROTO_DIR=./protocol
ARDUINO_PROTO_DIR=./arduino/protocol
RUBY_PROTO_DIR=./host/ruby/protocol

protoc -I $PROTO_DIR --ruby_out $RUBY_PROTO_DIR $PROTO_DIR/*.proto
protoc -I $PROTO_DIR --nanopb_out $ARDUINO_PROTO_DIR $PROTO_DIR/*.proto




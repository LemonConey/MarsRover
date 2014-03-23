#!/bin/bash -e

DEST_DIR=${2:-.}
PBFILE=$DEST_DIR/`basename $1 .proto`.pb
echo $PBFILE

echo $DEST_DIR

protoc -o$PBFILE $1
nanopb_generator $PBFILE
rm -f $PBFILE




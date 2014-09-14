#!/bin/bash
# Usage:
# install_lib.sh src-lib dst-lib

#export ARDUINO_ROOT=/Users/coney/Applications/Arduino.app/Contents/Resources/Java
ARDUINO_ROOT=${1:?no arduino root specified}
ARDUINO_LIB=$ARDUINO_ROOT/libraries

function install {
  TARGET=$ARDUINO_LIB/$2
  echo install $1 to $TARGET
  if ! [[ -e $TARGET ]]; then
    ln -sf `pwd`/$1 $ARDUINO_LIB/$2
  fi
}

#echo will install libraries to $ARDUINO_LIB

install lib RoverLib
install nanopb nanopb
install protocol RoverProto
install vendor/U8glib U8glib

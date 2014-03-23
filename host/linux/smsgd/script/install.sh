#!/bin/bash -e

SVC_NAME=smsgd
SVC_PATH=./script/smsgd.sh
BIN_PATH=../blade-bin/smsgd/smsgd
TARGET_BINDIR=/usr/local/bin

function log {
    echo $*
}

function install_bin {
    if ! [[ -x $BIN_PATH ]]; then
        log "can't find bin file '$BIN_PATH'"
        exit -1
    fi
    
    install -v $BIN_PATH $TARGET_BINDIR/
}

function install_service {
    if ! [[ -x $SVC_PATH ]]; then
        log "can't find service script '$SVC_PATH'"
        exit -1
    fi
    
    install -v $SVC_PATH /etc/init.d/$SVC_NAME
    update-rc.d $SVC_NAME defaults
}


install_bin
install_service




#!/bin/bash



if [ "$1" = "server" ]
then
    if [ "$2" = "release" ]
    then
        ./bin/release/open-builder server
    else 
        ./bin/debug/open-builder server
    fi
elif [ "$1" = "client" ]
then
    if [ "$2" = "release" ]
    then
        ./bin/release/open-builder client
    else 
        ./bin/debug/open-builder client
    fi
else
    if [ "$2" = "release" ]
    then
        ./bin/release/open-builder
    else 
        ./bin/debug/open-builder
    fi
fi
#!/bin/bash

if [ "$1" = "release" ]
then
    ./bin/release/open-builder $2 $3 $4 $5 $6 $7 $8 $9
else
    ./bin/debug/open-builder $1 $2 $3 $4 $5 $6 $7 $8 $9 
fi

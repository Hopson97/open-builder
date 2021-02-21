#!/bin/bash

target_release() {
    cd release
    cmake -DCMAKE_BUILD_TYPE=Release ../..
    make
    echo "Built target in bin/release/"
    cd ../..
}

target_debug() {
    cd debug 
    cmake -DCMAKE_BUILD_TYPE=Debug ../..
    make
    echo "Built target in bin/debug/"
    cd ../..
}

# Create folder for distribution
if [ "$1" = "release" ]
then
    if [ -d "$open-builder" ]
    then
        rm -rf -d open-builder
    fi

    mkdir -p open-builder
fi

# Creates the folder for the binaries
mkdir -p open-builder
mkdir -p bin
cd bin 
mkdir -p release
mkdir -p debug

# Builds target
if [ "$1" = "release" ]
then
    target_release
    cp bin/release/open-builder open-builder/open-builder
else
    target_debug
fi

cp -R Data open-builder/Data

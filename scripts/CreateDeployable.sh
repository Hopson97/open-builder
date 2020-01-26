#!/bin/bash

sh scripts/build.sh release 

rm -rf -d open-builder

mkdir open-builder

cp bin/release/open-builder open-builder
cp -r res open-builder
cp -r shaders open-builder
cp -r texture_packs open-builder
cp -r mods open-builder
cp  credits.txt open-builder
cp  config.obd open-builder
cp  LICENSE open-builder


echo "Deploy build created."
echo "cd open-builder to find it"
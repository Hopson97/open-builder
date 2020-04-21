#!/bin/bash

sh scripts/build.sh release 

rm -rf -d open-builder

mkdir open-builder

cp bin/release/open-builder open-builder
cp -r res open-builder
cp -r shaders open-builder

echo "Deploy build created."
echo "cd open-builder to find it"

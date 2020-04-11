#!/bin/bash

sh scripts/build.sh
gdb --args ./bin/debug/open-builder -server 4

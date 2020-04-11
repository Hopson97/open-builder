#!/bin/bash

sh scripts/build.sh
gdb --args ./bin/debug/open-builder -client 4

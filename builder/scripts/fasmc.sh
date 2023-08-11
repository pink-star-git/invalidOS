#!/bin/bash

# builder/scripts/fasmc.sh
# Copyright (C) 2023  Alex Zebra

printf "\33[37;1m[ * * *  ]\33[0m compiling \33[1m$1.\33[0m\n"


mkdir log/fasm/$3 -p

fasm ../src/$2/$1.s bin/$3/$1.bin &> log/fasm/$3/$1.log

if (grep "error" log/fasm/$3/$1.log)
then
    printf "\33[1;31m[ ERROR  ]\33[0m when \33[1m$1\33[0m compilation.\n"
else
    printf "\33[1;32m[   OK   ]\33[0;1m $1\33[0m compilation done.\33[0m\n"
fi
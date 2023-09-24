#!/bin/bash

# builder/scripts/cc.sh
# Copyright (C) 2023  Alex Zebra


printf "\33[37;1m[ * * *  ]\33[0m C compiling \33[1m$1.\33[0m\n"

mkdir log/c/$2 -p

gcc-13 -x c++ -fpermissive -Os -I ../include -nostdlib -m32\
    -march=i386 -fno-pie -ffreestanding -o o/$3/$1.o \
    -Wl,--nmagic,--script=../builder/c/linker.ld \
    ../src/$2/$1.cpp &> log/c/$2/$1.log

# gcc-13 -x c++ -fpermissive -Os -I ../include -nostdlib\
#     -fno-pie -ffreestanding -o o/$3/$1.o \
#     -Wl,--nmagic,--script=../builder/c/linker.ld \
#     ../src/$2/$1.cpp &> log/c/$2/$1.log

if (grep "error:" log/c/$2/$1.log || grep "шибка:" log/c/$2/$1.log)
then
    printf "\33[1;31m[ ERROR  ]\33[0m when \33[1m$1\33[0m compilation C.\n"
    exit
else
    printf "\33[1;32m[   OK   ]\33[0;1m $1\33[0m compilation C done.\33[0m\n"
fi

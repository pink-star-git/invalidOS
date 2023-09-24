#!/bin/bash

# builder/build.sh
# Copyright (C) 2023  Alex Zebra

# start
printf "\33[1m
┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
┃ ╭─────────────┬─────────────────╮ ┃╗
┃ │             ╎                 │ ┃║
┃ │ Developers  ╎  Zёbra   @2022  │ ┃║
┃ │             ╎                 │ ┃║
┃ ╞═════════════╪═════════════════╡ ┃║
┃ │   Made in   ╎     Russia      │ ┃║
┃ ╞═════════════╪═════════════════╡ ┃║
┃ │   Version   ╎     0.0.1       │ ┃║
┃ ╰─────────────┴─────────────────╯ ┃║
┃                                   ┃║
┃          ┏━━━━━━━━━━━━━┓          ┃║
┃          ┃             ┃          ┃║
┃          ┃  invalidOS  ┃          ┃║
┃          ┃             ┃          ┃║
┃          ┗━━━━━━━━━━━━━┛          ┃║
┃                                   ┃║
┃                                   ┃║
┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛║
 ╚═══════════════════════════════════╝
\33[0m
"

cd ..
mkdir build -p
cd build

# pre compilation
printf "\33[1;36m[  INFO  ]\33[0m pre compilation\n"

mkdir bin -p
mkdir o -p
mkdir log -p
touch log/fasm.log 2> /dev/null
touch log/c.log 2> /dev/null
printf "\33[1;32m[  DONE  ]\33[0m pre compilation done\n\n"


# C compilation
printf "\33[1;36m[  INFO  ]\33[0m c compilation\n"

# ../builder/script/ccs.sh main kernel
../builder/scripts/cc.sh main kernel
printf "\33[1;32m[  DONE  ]\33[0m c compilation done\n\n"


# post compilation
printf "\33[1;36m[  INFO  ]\33[0m post compilation\n"

../builder/scripts/fasmc.sh boot asm
../builder/scripts/fasmc.sh setup asm
# ../builder/scripts/fasmc.sh test asm
../builder/scripts/fasmc.sh system_interrupt asm
../builder/scripts/fasmc.sh main ../builder/s/kernel
#../builder/script/fasmc.sh shell ../builder/s/kernel
../builder/scripts/fasmc.sh os ../builder/s/os ..
printf "\33[1;32m[  DONE  ]\33[0m post compilation done\n\n"

# copy img
mv -f os.bin ../os.img

# start os
printf "\33[1;37m[ * * *  ]\33[0m start OS\n"

if [[ $1 == "-k" ]]
then
    printf "\33[1;36m[  INFO  ]\33[0m enable KVM\n"
    qemu-system-x86_64 -enable-kvm -s -drive file=../os.img,index=0,format=raw
else
    qemu-system-x86_64 -s -drive file=../os.img,index=0,format=raw
fi
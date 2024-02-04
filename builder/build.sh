#!/bin/bash
# builder/build.sh
# Copyright (C) 2023  Alex Zebra


source $(dirname "$0")/scripts/ext.sh


#    ,-------------------,
#    | MAIN BUILDER FILE |
#    '-------------------'


#    START
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
mkdir -p build
cd build
remove_status_file


#    PRE BUILD
BLOCK_NAME="Pre-build"
log_block_start
    mkdir -p bin
    mkdir -p out
    mkdir -p log
log_block_done



#    C++ COMPILATION
BLOCK_NAME="C++"
log_block_start
    cpp_compilation main kernel
log_block_end



#    FASM COMPILATION
BLOCK_NAME="Fasm"
log_block_start
    fasm_compilation boot asm
    fasm_compilation setup asm
log_block_done



#    POST BUILD
BLOCK_NAME="Post-build"
log_block_start
    fasm ../builder/s/os/os.s os.bin > /dev/null
    mv -f os.bin ../os.img
log_block_done



# start os
printf "$LOAD_STAT start OS\n"

if [[ $1 == "-k" ]]
then
    printf "$INFO_STAT enable KVM\n"
    qemu-system-x86_64 -enable-kvm -s -m 6G -drive file=../os.img,index=0,format=raw
else
    qemu-system-x86_64 -s -m 2G -drive file=../os.img,index=0,format=raw
fi
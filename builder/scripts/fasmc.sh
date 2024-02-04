#!/bin/bash
# builder/scripts/fasmc.sh
# Copyright (C) 2023  Alex Zebra


source $(dirname "$0")/ext.sh


#    ,------------------,
#    | C++ BUILDER FILE |
#    '------------------'

BLOCK_NAME=$1
file=$2
dir=$3

name="\33[1m$file\33[0m"



#    START

printf "$LOAD_STAT $BLOCK_NAME: $name - compilation started."



#    PATH

# build
src_dir="../src/$dir"
src_file="$src_dir/$file.s"

out_dir="bin/out/$dir"
out_suf=".bin"
out_file="$out_dir/$file.o"
mkdir -p $out_dir

# log
log_dir="../build/log/fasm/$dir"
log_e_dir="../build/log/error/fasm/$dir"
log_suf=".log"
log_file="$log_dir/$file$log_suf"
log_e_file="$log_e_dir/$file$log_suf"

mkdir -p $log_dir
mkdir -p $log_e_dir



#    BUILD

fasm $src_file $out_file &> $log_file



#    LOGGING BUILD

printf "$CLEAR"

if (grep "error" "$log_file" > /dev/null)
then
    status_fail
    printf "$FAIL_STAT $BLOCK_NAME: $name - compilation error:\n"
    printf "$INFO_STAT Log: $log_file\n"
else
    status_complete
    printf "$DONE_STAT $BLOCK_NAME: $name - compilation done.\33[0m\n"
fi
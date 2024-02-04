#! /bin/bash
# builder/scripts/cc.sh
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
include_dir="../include"

src_dir="../src/$dir"
src_file="$src_dir/$file.cpp"

out_dir="bin/out/$dir"
bld_suf=".bin"
out_suf=".x"
bld_file="$out_dir/$file$bld_suf"
out_file="$out_dir/$file$out_suf"
mkdir -p $out_dir

# log
log_dir="../build/log/c/$dir"
log_e_dir="../build/log/error/c/$dir"
log_suf=".log"
log_file="$log_dir/$file$log_suf"
log_e_file="$log_e_dir/$file$log_suf"

mkdir -p $log_dir
mkdir -p $log_e_dir



#    BUILD

gcc-13  -x c++ -std=c++20 -lgcc -fpermissive -Os -nostdlib -march=i386 -fno-pie \
        -ffreestanding -m32 -Wl,--nmagic,--script=../builder/c/linker.ld \
         -I $include_dir -o $bld_file $src_file &> $log_file



#    LOGGING BUILD

grep_sep="--group-separator=------"

printf "$CLEAR"

if (grep -C4 $grep_sep -iw -E "error|ошибка" $log_file > $log_e_file)
  then
    status_fail
    printf "$FAIL_STAT $BLOCK_NAME: $name - compilation error:\n"
    printf "$INFO_STAT Log: $log_file\n"
    exit 1
else
    status_complete
    printf "$DONE_STAT $BLOCK_NAME: $name - compilation done.\33[0m\n"
fi



#    OUT FILE ALIGN

dd if=$bld_file of=$out_file obs=512 conv=sync &> "dd.log"

sed -n '2p' "dd.log"
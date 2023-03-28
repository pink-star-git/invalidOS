#!/bin/bash

# start
printf "\33[1m
┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
┃ ╭──────────────┬────────────────╮ ┃╗
┃ │              ╎  zёbra   @2022 │ ┃║
┃ │  Developers  ├────────────────┤ ┃║
┃ │              ╎ SapeNeCo @2023 │ ┃║
┃ ╞══════════════╪════════════════╡ ┃║
┃ │   Made in    ╎     russia     │ ┃║
┃ ╞══════════════╪════════════════╡ ┃║
┃ │   Version    ╎     0.0.1      │ ┃║
┃ ╰──────────────┴────────────────╯ ┃║
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

cd ../build

printf "\33[1;36m[  INFO  ]\33[0m pre compilation\n"
# pre compilation
mkdir bin -p
mkdir o -p
mkdir log -p
touch log/fasm.log 2> /dev/null
touch log/c.log 2> /dev/null
printf "\33[1;32m[  DONE  ]\33[0m pre compilation done\n\n"

printf "\33[1;36m[  INFO  ]\33[0m c compilation\n"
# C compilation
# ../builder/script/ccs.sh main kernel
../builder/script/cc.sh main kernel
printf "\33[1;32m[  DONE  ]\33[0m c compilation done\n\n"

printf "\33[1;36m[  INFO  ]\33[0m post compilation\n"
# post compilation
../builder/script/fasmc.sh boot asm
../builder/script/fasmc.sh setup asm
../builder/script/fasmc.sh test asm
../builder/script/fasmc.sh system_interrupt asm
../builder/script/fasmc.sh main ../build/s/kernel
#../builder/script/fasmc.sh shell ../build/s/kernel
../builder/script/fasmc.sh os ../build/s/os ..
printf "\33[1;32m[  DONE  ]\33[0m post compilation done\n\n"

# copy img
mv -f os.bin ../os.img

printf "\33[1;37m[ * * *  ]\33[0m start OS\n"
# start os
if [[ $1 == "-k" ]]
then
    qemu-system-x86_64 -enable-kvm -drive file=../os.img,index=0,format=raw
else
    qemu-system-x86_64 -drive file=../os.img,index=0,format=raw
fi
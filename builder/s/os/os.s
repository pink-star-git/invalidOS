; builder/s/os/os.s
; Copyright (C) 2023  Alex Zebra

boot: file 'bin/out/asm/boot.o'
setup: file 'bin/out/asm/setup.o'
;;
kernel: file 'bin/out/kernel/main.x'

times(512*1024*8) db 45
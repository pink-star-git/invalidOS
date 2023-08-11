; builder/s/os/os.s
; Copyright (C) 2023  Alex Zebra

boot: file 'bin/boot.bin'
setup: file 'bin/setup.bin'
; system_interrupt: file 'bin/system_interrupt.bin'
kernel: file 'bin/main.bin'
; shell: file 'bin/shell.bin'


times(512*1024*8) db 45
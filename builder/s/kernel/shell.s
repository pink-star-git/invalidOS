; builder/s/kernel/shell.s
; Copyright (C) 2023  Alex Zebra

size = 4
o: file 'o/shell.o'
times(size*2*512-($-o)) db 0
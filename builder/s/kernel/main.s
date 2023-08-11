; builder/s/kernel/main.s
; Copyright (C) 2023  Alex Zebra

size = 10
o: file 'o/main.o'
times(size*2*512-($-o)) db 0
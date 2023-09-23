#include "define/integer.h"

struct Superblock {
    u_char8 magic_number = 0x69;
    u_char8 fs_version[4] = "0001";
    u_char8 disk_type;
    u_char8 disk_name[32];
    u_char8 pointer_len;
} __attribute__((packed));

// include/define/ide.h
// Copyright (C) 2023  OSDev.org

#include "ata.h"
#include "integer.h"

#ifndef IDE

struct IDEChannelRegisters {
    u_short16 base;  // I/O Base.
    u_short16 ctrl;  // Control Base
    u_short16 bmide; // Bus Master IDE
    u_char8   nIEN;  // nIEN (No Interrupt);
};

struct ide_device {
    u_char8   reserved;    // 0 (Empty) or 1 (This Drive really exists).
    u_char8   channel;     // 0 (Primary Channel) or 1 (Secondary Channel).
    u_char8   drive;       // 0 (Master Drive) or 1 (Slave Drive).
    u_short16 type;        // 0: ATA, 1:ATAPI.
    u_short16 signature;   // Drive Signature
    u_short16 capabilities;// Features.
    u_int32   commandSets; // Command Sets Supported.
    u_int32   size;        // Size in Sectors.
    u_char8   model[41];   // Model in string.
};

#endif

#define IDE
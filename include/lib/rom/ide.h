// include/lib/rom/ide.h
// Copyright (C) 2023  Alex Zebra

#pragma once

#include "define/integer.h"
#include "define/ide.h"
#include "define/color.h"

#include "lib/asm.h"
#include "lib/graphics/text/text.h"
#include "lib/str.h"

static IDEChannelRegisters channels[2];
static ide_device ide_devices[4];

static u_char8 ide_buf[2048] = {1};
static volatile u_char8 ide_irq_invoked = 0;
static u_char8 atapi_packet[12] = {0xA8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static u_char8 package[4] = {0, 0, 0, 0};

void
ide_write (u_char8 channel, u_char8 reg, u_char8 data) {
    if (reg > 0x07 && reg < 0x0C)
        ide_write(channel, ATA_REG_CONTROL, 0x80 | channels[channel].nIEN);

    if (reg < 0x08)
        outb(channels[channel].base  + reg - 0x00, data);
    else if (reg < 0x0C)
        outb(channels[channel].base  + reg - 0x06, data);
    else if (reg < 0x0E)
        outb(channels[channel].ctrl  + reg - 0x0A, data);
    else if (reg < 0x16)
        outb(channels[channel].bmide + reg - 0x0E, data);

    if (reg > 0x07 && reg < 0x0C)
        ide_write(channel, ATA_REG_CONTROL, channels[channel].nIEN);
}

u_char8
ide_read (u_char8 channel, u_char8 reg) {
    u_char8 result;
    if (reg > 0x07 && reg < 0x0C)
        ide_write(channel, ATA_REG_CONTROL, 0x80 | channels[channel].nIEN);

    if (reg < 0x08)
        result = inb(channels[channel].base + reg - 0x00);
    else if (reg < 0x0C)
        result = inb(channels[channel].base  + reg - 0x06);
    else if (reg < 0x0E)
        result = inb(channels[channel].ctrl  + reg - 0x0A);
    else if (reg < 0x16)
        result = inb(channels[channel].bmide + reg - 0x0E);

    if (reg > 0x07 && reg < 0x0C)
        ide_write(channel, ATA_REG_CONTROL, channels[channel].nIEN);

    return result;
}

void
ide_read_buffer (u_char8 channel, u_char8 reg,
                u_int32 *buffer, u_int32 quads) {
    if (reg > 0x07 && reg < 0x0C)
        ide_write(channel, ATA_REG_CONTROL, 0x80 | channels[channel].nIEN);

    asm("pushw %es      \n\t"
        "movw  %ds, %ax \n\t"
        "movw  %ax, %es ");

    if      (reg < 0x08)
        insl(channels[channel].base  + reg - 0x00, buffer, quads);
    else if (reg < 0x0C)
        insl(channels[channel].base  + reg - 0x06, buffer, quads);
    else if (reg < 0x0E)
        insl(channels[channel].ctrl  + reg - 0x0A, buffer, quads);
    else if (reg < 0x16)
        insl(channels[channel].bmide + reg - 0x0E, buffer, quads);

    asm("popw %es");

    if (reg > 0x07 && reg < 0x0C)
        ide_write(channel, ATA_REG_CONTROL, channels[channel].nIEN);
}

static u_char8
ide_polling (u_char8 channel, u_int32 advanced_check) {
    for(u_char8 i = 0; i < 4; i++)
        ide_read(channel, ATA_REG_ALTSTATUS);

    while (ide_read(channel, ATA_REG_STATUS) & ATA_SR_BSY)
        ;

    if (advanced_check) {
        u_char8 state = ide_read(channel, ATA_REG_STATUS);

        if (state & ATA_SR_ERR)
            return 2; // Error.

        if (state & ATA_SR_DF)
            return 1; // Device Fault.

        if ((state & ATA_SR_DRQ) == 0)
            return 3; // DRQ should be set
    }

    return 0; // No Error.
}

static u_char8
ide_print_error (u_int32 drive, u_char8 err) {
    if (err == 0)
        return err;

    print("IDE: Error:", " ", C_Red);
    if        (err == 1)      {print("- Device Fault", "\n", C_Red);              err = 19;}
    else   if (err == 2) {
        u_char8 st = ide_read(ide_devices[drive].channel, ATA_REG_ERROR);
        if (st & ATA_ER_AMNF)   {print("- No Address Mark Found", "\n", C_Red);     err = 7;}
        if (st & ATA_ER_TK0NF)  {print("- No Media or Media Error", "\n", C_Red);   err = 3;}
        if (st & ATA_ER_ABRT)   {print("- Command Aborted", "\n", C_Red);           err = 20;}
        if (st & ATA_ER_MCR)    {print("- No Media or Media Error", "\n", C_Red);   err = 3;}
        if (st & ATA_ER_IDNF)   {print("- ID mark not Found", "\n", C_Red);         err = 21;}
        if (st & ATA_ER_MC)     {print("- No Media or Media Error", "\n", C_Red);   err = 3;}
        if (st & ATA_ER_UNC)    {print("- Uncorrectable Data Error", "\n", C_Red);  err = 22;}
        if (st & ATA_ER_BBK)    {print("- Bad Sectors", "\n", C_Red);               err = 13;}
    } else if (err == 3)      {print("- Reads Nothing", "\n", C_Red);             err = 23;}
    else   if (err == 4)      {print("- Write Protected", "\n", C_Red);           err = 8;}
    // printk("- [%s %s] %s\n",
    print((const u_char8 *[]){"Primary", "Secondary"}[ide_devices[drive].channel], " ", C_Red); // Use the channel as an index into the array
    print((const u_char8 *[]){"Master", "Slave"}[ide_devices[drive].drive], "\n", C_Red); // Same as above, using the drive
    print(ide_devices[drive].model, "\n", C_Red);

    return err;
}


// ide_initialize (u_int32 BAR0, u_int32 BAR1, u_int32 BAR2,
//                 u_int32 BAR3, u_int32 BAR4)
static void
ide_initialize () {
    u_char8 i, j, k, count = 0;

    // 1 - Detect I/O Ports which interface IDE Controller:
    channels[ATA_PRIMARY  ].base  = 0x1F0;
    channels[ATA_PRIMARY  ].ctrl  = 0x3F6;
    channels[ATA_SECONDARY].base  = 0x170;
    channels[ATA_SECONDARY].ctrl  = 0x376;
    channels[ATA_PRIMARY  ].bmide = 0; // Bus Master IDE
    channels[ATA_SECONDARY].bmide = 8; // Bus Master IDE

    // 2 - Disable IRQs:
    ide_write(ATA_PRIMARY  , ATA_REG_CONTROL, 2);
    ide_write(ATA_SECONDARY, ATA_REG_CONTROL, 2);

    // 3 - Detect ATA-ATAPI Devices:
    for (i = 0; i < 2; i++)
        for (j = 0; j < 2; j++) {
            u_char8 err = 0, type = IDE_ATA, status;
            ide_devices[count].reserved = 0; // Assuming that no drive here.

            // (I) Select Drive:
            ide_write(i, ATA_REG_HDDEVSEL, 0xA0 | (j << 4)); // Select Drive.
            sleep(1); // Wait 1ms for drive select to work.

            // (II) Send ATA Identify Command:
            ide_write(i, ATA_REG_COMMAND, ATA_CMD_IDENTIFY);
            sleep(1); // This function should be implemented in your OS. which waits for 1 ms.
                     // it is based on System Timer Device Driver.

            // (III) Polling:
            if (ide_read(i, ATA_REG_STATUS) == 0)
                continue; // If Status = 0, No Device.

            while(1) {
                status = ide_read(i, ATA_REG_STATUS);
                if ((status & ATA_SR_ERR)) {
                    err = 1;
                    break;
                } // If Err, Device is not ATA.

                if (!(status & ATA_SR_BSY) && (status & ATA_SR_DRQ))
                    break; // Everything is right.
            }

            // (IV) Probe for ATAPI Devices:
            static u_char8 cl;
            static u_char8 ch;

            if (err != 0) {
                cl = ide_read(i, ATA_REG_LBA1);
                ch = ide_read(i, ATA_REG_LBA2);

                if (cl == 0x14 && ch ==0xEB)
                    type = IDE_ATAPI;
                else if (cl == 0x69 && ch == 0x96)
                    type = IDE_ATAPI;
                else
                    continue; // Unknown Type (may not be a device).

                ide_write(i, ATA_REG_COMMAND, ATA_CMD_IDENTIFY_PACKET);
                sleep(1);
            }

            // (V) Read Identification Space of the Device:
            ide_read_buffer(i, ATA_REG_DATA, (u_int32 *)ide_buf, 128);

            // (VI) Read Device Parameters:
            ide_devices[count].reserved     = 1;
            ide_devices[count].type         = type;
            ide_devices[count].channel      = i;
            ide_devices[count].drive        = j;
            ide_devices[count].signature    = *((u_short16 *)(ide_buf + ATA_IDENT_DEVICETYPE));
            ide_devices[count].capabilities = *((u_short16 *)(ide_buf + ATA_IDENT_CAPABILITIES));
            ide_devices[count].commandSets  = *((u_int32 *)(ide_buf + ATA_IDENT_COMMANDSETS));

            // (VII) Get Size:
            if (ide_devices[count].commandSets & (1 << 26))
                // Device uses 48-Bit Addressing:
                ide_devices[count].size   = *((u_int32 *)(ide_buf + ATA_IDENT_MAX_LBA_EXT));
            else
                // Device uses CHS or 28-bit Addressing:
                ide_devices[count].size   = *((u_int32 *)(ide_buf + ATA_IDENT_MAX_LBA));

            // (VIII) String indicates model of device (like Western Digital HDD and SONY DVD-RW...):
            for(k = 0; k < 40; k += 2) {
                ide_devices[count].model[k] = ide_buf[ATA_IDENT_MODEL + k + 1];
                ide_devices[count].model[k + 1] = ide_buf[ATA_IDENT_MODEL + k];
            }
            ide_devices[count].model[40] = 0; // Terminate String.

            count++;
        }

    // 4 - Print Summary:
    print("IDE:");
    for (i = 0; i < 4; i++) {
        print("   ", " ");
        print(dec_2_str(i).data, " ");
        print("Dev:", " ");
        if (ide_devices[i].reserved == 1) {
            u_int32 size = ide_devices[i].size / 2;
            print("Found");
            print("        Type:", " ");
            print((const u_char8 *[]){"ATA", "ATAPI"}[ide_devices[i].type]);
            print("        Size:", " ");
            print(dec_2_str(size).data, ".");
            print(dec_2_str(ide_devices[i].size % 2 * 5).data, " ");
            print("KB;", " ");
            print(dec_2_str(size / 1024).data, ".");
            print(dec_2_str(size / 10 % 100 / 11 + size / 10 % 10 / 5 ).data, " ");
            print("MB");
            print("        Model:", " ");
            print(ide_devices[i].model);
        } else {
            print("None");
        }
        print(" ");
    }
}

static u_char8
ide_ata_access(u_char8 direction, u_char8 drive, u_int32 lba,
                u_char8 numsects, u_short16 selector, u_int32 edi) {
    u_char8   lba_mode /* 0: CHS,    1:LBA28,   2: LBA48 */,
              dma      /* 0: No DMA, 1: DMA */,
              cmd;
    u_char8   lba_io[6];
    u_int32   channel   = ide_devices[drive].channel; // Read the Channel.
    u_int32   slavebit  = ide_devices[drive].drive; // Read the Drive [Master/Slave]
    u_int32   bus       = channels[channel].base; // Bus Base, like 0x1F0 which is also data port.
    u_int32   words     = 256; // Almost every ATA drive has a sector-size of 512-byte.
    u_short16 cyl,
              i;
    u_char8   head,
              sect,
              err;

    ide_write(channel, ATA_REG_CONTROL, channels[channel].nIEN = (ide_irq_invoked = 0x0) + 0x02);

      // (I) Select one from LBA28, LBA48 or CHS;
    if (lba >= 0x10000000) { // Sure Drive should support LBA in this case, or you are
     // giving a wrong LBA.
        // LBA48:
        lba_mode  = 2;
        lba_io[0] = (lba & 0x000000FF) >> 0;
        lba_io[1] = (lba & 0x0000FF00) >> 8;
        lba_io[2] = (lba & 0x00FF0000) >> 16;
        lba_io[3] = (lba & 0xFF000000) >> 24;
        lba_io[4] = 0; // LBA28 is integer, so 32-bits are enough to access 2TB.
        lba_io[5] = 0; // LBA28 is integer, so 32-bits are enough to access 2TB.
        head      = 0; // Lower 4-bits of HDDEVSEL are not used here.
    } else if (ide_devices[drive].capabilities & 0x200) { // Drive supports LBA?
        // LBA28:
        lba_mode  = 1;
        lba_io[0] = (lba & 0x00000FF) >> 0;
        lba_io[1] = (lba & 0x000FF00) >> 8;
        lba_io[2] = (lba & 0x0FF0000) >> 16;
        lba_io[3] = 0; // These Registers are not used here.
        lba_io[4] = 0; // These Registers are not used here.
        lba_io[5] = 0; // These Registers are not used here.
        head      = (lba & 0xF000000) >> 24;
    } else {
        // CHS:
        lba_mode  = 0;
        sect      = (lba % 63) + 1;
        cyl       = (lba + 1  - sect) / (16 * 63);
        lba_io[0] = sect;
        lba_io[1] = (cyl >> 0) & 0xFF;
        lba_io[2] = (cyl >> 8) & 0xFF;
        lba_io[3] = 0;
        lba_io[4] = 0;
        lba_io[5] = 0;
        head      = (lba + 1  - sect) % (16 * 63) / (63); // Head number is written to HDDEVSEL lower 4-bits.
    }

      // (II) See if drive supports DMA or not;
    dma = 0; // We don't support DMA

      // (III) Wait if the drive is busy;
    while (ide_read(channel, ATA_REG_STATUS) & ATA_SR_BSY)
        ;

      // (IV) Select Drive from the controller;
    if (lba_mode == 0)
        ide_write(channel, ATA_REG_HDDEVSEL, 0xA0 | (slavebit << 4) | head); // Drive & CHS.
    else
        ide_write(channel, ATA_REG_HDDEVSEL, 0xE0 | (slavebit << 4) | head); // Drive & LBA

    if (lba_mode == 2) {
        ide_write(channel, ATA_REG_SECCOUNT1,   0);
        ide_write(channel, ATA_REG_LBA3,   lba_io[3]);
        ide_write(channel, ATA_REG_LBA4,   lba_io[4]);
        ide_write(channel, ATA_REG_LBA5,   lba_io[5]);
    }
    ide_write(channel, ATA_REG_SECCOUNT0,   numsects);
    ide_write(channel, ATA_REG_LBA0,   lba_io[0]);
    ide_write(channel, ATA_REG_LBA1,   lba_io[1]);
    ide_write(channel, ATA_REG_LBA2,   lba_io[2]);

    if (lba_mode == 0 && dma == 0 && direction == 0) cmd = ATA_CMD_READ_PIO;
    if (lba_mode == 1 && dma == 0 && direction == 0) cmd = ATA_CMD_READ_PIO;
    if (lba_mode == 2 && dma == 0 && direction == 0) cmd = ATA_CMD_READ_PIO_EXT;
    if (lba_mode == 0 && dma == 1 && direction == 0) cmd = ATA_CMD_READ_DMA;
    if (lba_mode == 1 && dma == 1 && direction == 0) cmd = ATA_CMD_READ_DMA;
    if (lba_mode == 2 && dma == 1 && direction == 0) cmd = ATA_CMD_READ_DMA_EXT;
    if (lba_mode == 0 && dma == 0 && direction == 1) cmd = ATA_CMD_WRITE_PIO;
    if (lba_mode == 1 && dma == 0 && direction == 1) cmd = ATA_CMD_WRITE_PIO;
    if (lba_mode == 2 && dma == 0 && direction == 1) cmd = ATA_CMD_WRITE_PIO_EXT;
    if (lba_mode == 0 && dma == 1 && direction == 1) cmd = ATA_CMD_WRITE_DMA;
    if (lba_mode == 1 && dma == 1 && direction == 1) cmd = ATA_CMD_WRITE_DMA;
    if (lba_mode == 2 && dma == 1 && direction == 1) cmd = ATA_CMD_WRITE_DMA_EXT;
    ide_write(channel, ATA_REG_COMMAND, cmd);               // Send the Command.

    if (dma)
        if (direction == 0);
            // DMA Read.
        else;
            // DMA Write.
    else
        if (direction == 0) {
            // PIO Read.
            for (i = 0; i < numsects; i++) {
                if (err = ide_polling(channel, 1))
                    return err; // Polling, set error and exit if there is.
                asm("pushw %es");
                asm("mov %%ax, %%es" : : "a"(selector));
                asm("rep insw" : : "c"(words), "d"(bus), "D"(edi)); // Receive Data.
                asm("popw %es");
                edi += (words*2);
            }
        } else {
            // PIO Write.
            for (i = 0; i < numsects; i++) {
                ide_polling(channel, 0); // Polling.
                asm("pushw %ds");
                asm("mov %%ax, %%ds"::"a"(selector));
                asm("rep outsw"::"c"(words), "d"(bus), "S"(edi)); // Send Data
                asm("popw %ds");
                edi += (words*2);
            }
            ide_write(channel, ATA_REG_COMMAND, (char []) {   ATA_CMD_CACHE_FLUSH,
                            ATA_CMD_CACHE_FLUSH,
                            ATA_CMD_CACHE_FLUSH_EXT}[lba_mode]);
            ide_polling(channel, 0); // Polling.
        }
    
    return 0; // Easy, isn't it?
}

void ide_wait_irq() {
   while (!ide_irq_invoked)
      ;
   ide_irq_invoked = 0;
}

void ide_irq() {
   ide_irq_invoked = 1;
}

static unsigned char ide_atapi_read(unsigned char drive, unsigned int lba, unsigned char numsects,
          unsigned short selector, unsigned int edi) {

    unsigned int   channel  = ide_devices[drive].channel;
    unsigned int   slavebit = ide_devices[drive].drive;
    unsigned int   bus      = channels[channel].base;
    unsigned int   words    = 1024; // Sector Size. ATAPI drives have a sector size of 2048 bytes.
    unsigned char  err      = 0;

      // (I): Enable IRQs:
    ide_write(channel, ATA_REG_CONTROL, channels[channel].nIEN = ide_irq_invoked = 0x0);

      // (II): Setup SCSI Packet:
    atapi_packet[ 0] = ATAPI_CMD_READ;
    atapi_packet[ 1] = 0x0;
    atapi_packet[ 2] = (lba >> 24) & 0xFF;
    atapi_packet[ 3] = (lba >> 16) & 0xFF;
    atapi_packet[ 4] = (lba >> 8) & 0xFF;
    atapi_packet[ 5] = (lba >> 0) & 0xFF;
    atapi_packet[ 6] = 0x0;
    atapi_packet[ 7] = 0x0;
    atapi_packet[ 8] = 0x0;
    atapi_packet[ 9] = numsects;
    atapi_packet[10] = 0x0;
    atapi_packet[11] = 0x0;

      // (III): Select the drive:
    ide_write(channel, ATA_REG_HDDEVSEL, slavebit << 4);

      // (IV): Delay 400 nanoseconds for select to complete:
    for(int i = 0; i < 4; i++)
        ide_read(channel, ATA_REG_ALTSTATUS); // Reading the Alternate Status port wastes 100ns.

      // (V): Inform the Controller that we use PIO mode:
    ide_write(channel, ATA_REG_FEATURES, 0);         // PIO mode.

      // (VI): Tell the Controller the size of buffer:
    ide_write(channel, ATA_REG_LBA1, (words * 2) & 0xFF);   // Lower Byte of Sector Size.
    ide_write(channel, ATA_REG_LBA2, (words * 2) >> 8);   // Upper Byte of Sector Size.

      // (VII): Send the Packet Command:
    ide_write(channel, ATA_REG_COMMAND, ATA_CMD_PACKET);      // Send the Command.

      // (VIII): Waiting for the driver to finish or return an error code:
    if (err = ide_polling(channel, 1)) return err;         // Polling and return if error.

      // (IX): Sending the packet data:
    asm("rep   outsw" : : "c"(6), "d"(bus), "S"(atapi_packet));   // Send Packet Data

      // (X): Receiving Data:
    for (int i = 0; i < numsects; i++) {
        ide_wait_irq();                  // Wait for an IRQ.
        if (err = ide_polling(channel, 1))
            return err;      // Polling and return if error.
        asm("pushw %es");
        asm("mov %%ax, %%es"::"a"(selector));
        asm("rep insw"::"c"(words), "d"(bus), "D"(edi));// Receive Data.
        asm("popw %es");
        edi += (words * 2);
    }

      // (XI): Waiting for an IRQ:
    ide_wait_irq();

      // (XII): Waiting for BSY & DRQ to clear:
    while (ide_read(channel, ATA_REG_STATUS) & (ATA_SR_BSY | ATA_SR_DRQ))
        ;

    return 0; // Easy, ... Isn't it?
}

static void
ide_read_sectors(unsigned char drive, unsigned char numsects, unsigned int lba,
                      unsigned short es, unsigned int edi) {

      // (I): Check if the drive presents:
    if (drive > 3 || ide_devices[drive].reserved == 0) package[0] = 0x1;      // Drive Not Found!

      // (II): Check if inputs are valid:
    else if (((lba + numsects) > ide_devices[drive].size) && (ide_devices[drive].type == IDE_ATA))
        package[0] = 0x2;                     // Seeking to invalid position.

      // (III): Read in PIO Mode through Polling & IRQs:
    else {
        unsigned char err;
        if (ide_devices[drive].type == IDE_ATA)
            err = ide_ata_access(ATA_READ, drive, lba, numsects, es, edi);
        else if (ide_devices[drive].type == IDE_ATAPI)
            for (int i = 0; i < numsects; i++)
                err = ide_atapi_read(drive, lba + i, 1, es, edi + (i*2048));
        package[0] = ide_print_error(drive, err);
        package[1] = err;
    }
}

void ide_write_sectors(unsigned char drive, unsigned char numsects, unsigned int lba,
                       unsigned short es, unsigned int edi) {

      // (I): Check if the drive presents:
    if (drive > 3 || ide_devices[drive].reserved == 0)
        package[0] = 0x1;      // Drive Not Found!

      // (II): Check if inputs are valid:
    else if (((lba + numsects) > ide_devices[drive].size) && (ide_devices[drive].type == IDE_ATA))
        package[0] = 0x2;                     // Seeking to invalid position.

      // (III): Read in PIO Mode through Polling & IRQs:
    else {
        unsigned char err;
        if (ide_devices[drive].type == IDE_ATA)
            err = ide_ata_access(ATA_WRITE, drive, lba, numsects, es, edi);
        else if (ide_devices[drive].type == IDE_ATAPI)
            err = 4; // Write-Protected.
        package[0] = ide_print_error(drive, err);
    }
}
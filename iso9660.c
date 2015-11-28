#include <stdio.h>
#include <stdlib.h>

#include "common.h"

#define ISO_9660_BOOT_RECORD 0
#define ISO_9660_PRIMARY_VOLUME_DESCRIPTOR 1
#define ISO_9660_SUPPLEMENTARY_VOLUME_DESCRIPTOR 2
#define ISO_9660_VOLUME_PARTITION_DESCRIPTOR 3
#define ISO_9660_VOLUME_DESCRIPTOR_SET_TERMINATOR 255

void get_sector(FILE* file, int sector, u8* buffer) {
    fseek(file, sector * 2048, SEEK_SET);
    fread(buffer, 1, 2048, file);
}

void show_as_hex(u8* ptr) {
    int i;
    int j;
    int addr = 0;

    for (i = 0; i < 128; ++i) {
        printf("%08X: ", addr);
        addr += 16;

        for (j = 0; j < 16; ++j) {
            printf("%02X ", *ptr);

            if (j == 7) {
                printf(" ");
            }

            ++ptr;
        }

        printf("|");
        ptr -= 16;

        for (j = 0; j < 16; ++j) {
            if (*ptr >= ' ' && *ptr <= '~'){
                printf("%c", *ptr);
            } else {
                printf(".");
            }
            ++ptr;
        }

        printf("|\n");
    }
}

void show_volume_desciptor_type_code(u8* ptr) {
    if (*ptr == ISO_9660_BOOT_RECORD) {
        printf("Volume type: Boot Record\n");
    } else if (*ptr == ISO_9660_PRIMARY_VOLUME_DESCRIPTOR) {
        printf("Volume type: Primary Volume Descriptor\n");
    } else if (*ptr == ISO_9660_SUPPLEMENTARY_VOLUME_DESCRIPTOR) {
        printf("Volume type: Supplementary Volume Descriptor\n");
    } else if (*ptr == ISO_9660_VOLUME_PARTITION_DESCRIPTOR) {
        printf("Volume type: Volume Partition Descriptor\n");
    } else if (*ptr == ISO_9660_VOLUME_DESCRIPTOR_SET_TERMINATOR) {
        printf("Volume type: Volume Descriptor Set Terminator\n");
    } else {
        printf("Volume type: Unknown\n");
    }
}

void show_flags(u8* ptr) {
    int a = *ptr & (1 << 0) ? 1 : 0;
    int b = *ptr & (1 << 1) ? 1 : 0;
    int c = *ptr & (1 << 2) ? 1 : 0;
    int d = *ptr & (1 << 3) ? 1 : 0;
    int e = *ptr & (1 << 4) ? 1 : 0;
    int f = *ptr & (1 << 7) ? 1 : 0;

    printf("HID=%i DIR=%i ASF=%i INF=%i PERM=%i FDIR=%i", a, b, c, d, e, f);
}

void show_directory_info(u8* ptr) {
    int i;
    u32 id_size = (u32) *(ptr + 32);

    printf("Lenght of directory: %i\n", (u32) *ptr);
    printf("Extended record length: %i\n", (u32) *(ptr + 1));
    printf("LBA: %i\n", (u32) *((u32*) (ptr + 2)));
    printf("Data length: %i\n", (u32) *((u32*) (ptr + 10)));
    printf("File flags: ");
    show_flags(ptr + 25);
    printf("\nLength of file identifier: %i\n", (u32) *(ptr + 32));
    printf("Identifier: ");

    for (i = 0; i < id_size; ++i) {
        printf("%c", (char) *(ptr + 33 + i));
    }

    printf("\n");
}

void show_directory_contents(u8* ptr) {
    u32 dir_size;

    while (*ptr != 0) {
        show_directory_info(ptr);
        printf("\n");
        ptr = ptr + ((u8) *ptr);
    }
}

void show_primary_volume_descriptor_info(u8* ptr) {
    int i;

    printf("Type code: %i\n", (u32) *(ptr + 0));
    printf("Identifier: ");

    for (i = 1; i <= 5; ++i) {
        printf("%c", (char) *(ptr + i));
    }

    printf("\nVersion: %i\n", (u32) *(ptr + 6));

    printf("System identifier: ");

    for (i = 8; i < 32; ++i) {
        printf("%c", ptr[i]);
    }

    printf("\nVolume identifier: ");

    for (i = 40; i < 40 + 32; ++i) {
        printf("%c", ptr[i]);
    }

    printf("\nVolume space size: %i logical blocks\n", *((u32*) (ptr + 80)));
    printf("Volume set size: %i disk(s)\n", *((u16*) (ptr + 120)));
    printf("Volume sequence number: %i\n", *((u16*) (ptr + 124)));
    printf("Logical block size: %i\n", *((u16*) (ptr + 128)));
    printf("Path table size: %i\n", *((u32*) (ptr + 132)));
    printf("Path table LBA: %i\n", *((u32*) (ptr + 140)));

    if (*((u32*) (ptr + 144)) != 0) {
        printf("Optional path table LBA: %i\n", *((u32*) (ptr + 144)));
    }
    
    printf("Root directory info...\n");
    show_directory_info(ptr + 156); 
}


int main(int argc, char** argv) {
    int i;
    char buffer[2048];
    FILE* file;

    file = fopen(argv[1], "r");

    for (i = 0; i < 50; ++i) {
        get_sector(file, i, buffer);
        printf("Sector %i: ", i);
        show_volume_desciptor_type_code(buffer);
    }

    get_sector(file, 16, buffer);
    show_primary_volume_descriptor_info(buffer);

    get_sector(file, 24, buffer);
    show_as_hex(buffer);
    show_directory_info(buffer);
    printf("\n\n");
    show_directory_contents(buffer);

    get_sector(file, 25, buffer);
    show_as_hex(buffer);

    fclose(file);

    return 0;
}

/*
 * dump_rom.c  --  emits the embedded Crystal Clear ROM as a real .gbc file.
 *
 * Crystal_Clear_rom.c does not contain Game Boy *source code* -- it contains a
 * fully-built 2 MiB GBC ROM image stored as a C byte array (rom_data[]). You do
 * not "compile" it with a Game Boy toolchain (RGBDS / GBDK); you just need to
 * write those bytes back out to disk. This file does exactly that.
 *
 * Build (host compiler, e.g. gcc/clang -- NOT a GB toolchain):
 *     gcc -O2 -o dump_rom Crystal_Clear_rom.c dump_rom.c
 *
 * Run:
 *     ./dump_rom
 *
 * Result: Crystal_Clear.gbc  -- a 2,097,152-byte ROM, ready for an emulator,
 * a flashcart (e.g. EverDrive), or an Analogue Pocket.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

/* Defined in Crystal_Clear_rom.c */
extern const uint8_t rom_data[];
extern const size_t  rom_size;

#define OUT_PATH "Crystal_Clear.gbc"

/* Standard GB/GBC header checksum over 0x134..0x14C. */
static uint8_t header_checksum(const uint8_t *rom)
{
    uint8_t c = 0;
    for (size_t a = 0x134; a <= 0x14C; ++a)
        c = (uint8_t)(c - rom[a] - 1);
    return c;
}

/* 16-bit global checksum: sum of every byte except the two checksum bytes. */
static uint16_t global_checksum(const uint8_t *rom, size_t len)
{
    uint32_t s = 0;
    for (size_t i = 0; i < len; ++i)
        s += rom[i];
    s -= rom[0x14E];
    s -= rom[0x14F];
    return (uint16_t)(s & 0xFFFF);
}

int main(void)
{
    if (rom_size == 0) {
        fprintf(stderr, "error: rom_data is empty\n");
        return 1;
    }

    /* Sanity-check the cartridge header before writing anything. */
    uint8_t  hc      = header_checksum(rom_data);
    uint16_t gc      = global_checksum(rom_data, rom_size);
    uint8_t  hc_file = rom_data[0x14D];
    uint16_t gc_file = (uint16_t)((rom_data[0x14E] << 8) | rom_data[0x14F]);

    printf("ROM size        : %zu bytes (%.2f MiB)\n",
           rom_size, (double)rom_size / (1024.0 * 1024.0));
    printf("Header checksum : computed 0x%02X / stored 0x%02X  [%s]\n",
           hc, hc_file, (hc == hc_file) ? "OK" : "MISMATCH");
    printf("Global checksum : computed 0x%04X / stored 0x%04X  [%s]\n",
           gc, gc_file, (gc == gc_file) ? "OK" : "MISMATCH");

    FILE *f = fopen(OUT_PATH, "wb");
    if (!f) {
        perror("fopen");
        return 1;
    }

    size_t written = fwrite(rom_data, 1, rom_size, f);
    if (fclose(f) != 0 || written != rom_size) {
        fprintf(stderr, "error: wrote %zu of %zu bytes\n", written, rom_size);
        return 1;
    }

    printf("Wrote %s (%zu bytes)\n", OUT_PATH, written);
    return 0;
}

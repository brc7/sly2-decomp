// cd.cpp -- CD/DVD access: ISO 9660 path tokenizing, the media-mode check, the disc-identity poll (a CRC of a
// sector), cdrom0: paths and direct sector reads.
// UpdateCd runs every frame: when the sound manager is up and the tray no longer reports a PS2 DVD, it pauses the
// game's disc users and polls by reading one known sector and comparing its CRC-32 until the right disc is back.
// Entry points: StartupCd, UpdateCd, CdPath (host path -> "cdrom0:\PATH\FILE.EXT;1"), ReadCdDirect,
// IsCdCommandComplete, IsPs2DvdInDrive / SetCdMmode.
// TU 0x14B7F8..0x14BEA8 (10 functions, address order).
// SLY2_ALIAS: __static_initialization_and_destruction_0=StaticInit_cd
// SLY2_ALIAS: _GLOBAL_$I$*=GlobalConstructors_cd
#include "sly2/slotheap.h"
#include "sly2/sndmgr.h"
#include "sly2/scrap.h"

// 0x14BD50 StaticInit_cd and GlobalConstructors_cd: g++-generated for the six engine slot pools.
SLY2_INSTANTIATE_SLOT_POOLS()

/* cd's .rodata, defined at the end of the file (after UpdateCd: with their values visible g++ folds them and
 * UpdateCd no longer matches). */
extern const char D_002A2E50[4], D_002A2E58[4];
extern const int D_002A2E5C;
extern const unsigned int D_002A2E60, D_002A2E68[256];
extern char D_003DA4F0[];
/* cd's .data (g_cds: by layout, between carmelita's data and g_scecd; sound, mpeg and cdutil read it too). */
int g_cds = 0;                 /* disc-poll state (1 read, 2 wait, 3 idle) */
int g_scecd = -1;                /* last disc type */
/* cd's .bss. */
unsigned char D_002DD680[0x800] __attribute__((aligned(64)));    /* sector buffer (64-aligned by layout: 0x2DD680 after carmelita's word at 0x2DD648; sceCdRead DMAs into it) */
int s_retCdRead, s_retCdError;
extern "C" {
char *strupr(char *) __asm__("UpperizePchz__FPc");
char *strcpy(char *, const char *);
char *strcat(char *, const char *) __asm__("strcpy1");
unsigned int strlen(const char *);
int sprintf(char *, const char *, ...);
int sceCdGetDiskType(void);
int sceCdGetError(void);
int sceCdMmode(int n);
int sceCdSync(int n);
int sceCdRead(unsigned int sector, unsigned int sectorCount, void *buf, void *mode);
void ShowScreen52D(void *p);
int IsScreen52DShown(void *p);
void HideScreen52D(void *p);
}
int IsPs2DvdInDrive();
int IsCdCommandComplete(int wait);

// Rewrites a path in place into ISO 9660 form: upper case, ';1' version appended to a file name, separators
// become NULs (so the path turns into consecutive component strings), names cut to 8.3 and characters outside
// [A-Z0-9] replaced by '_'. Returns the number of components.
int CpchzTokenizePath(char *path)
{
    strupr(path);
    int length = strlen(path);
    if (length != 0 && ((unsigned char *)path)[length - 1] != '\\' && ((unsigned char *)path)[length - 1] != '/') {
        strcat(path, ";1");
        length = strlen(path);
    }
    unsigned char *end = (unsigned char *)path + length;
    unsigned char *out = (unsigned char *)path;
    unsigned char *in = (unsigned char *)path;
    int componentCount = 0;
    int componentLength = 0;
    int maxComponentLength = 8;
    for (; in < end; in++) {
        switch (*in) {
        case '/':
        case '\\':
            if (componentLength == 0)
                break;
            *out++ = 0;
            componentLength = 0;
            maxComponentLength = 8;
            componentCount++;
            break;
        case '.':
            if (componentLength == 0)
                break;
            if (maxComponentLength != 8)
                break;
            *out++ = *in;
            componentLength = 0;
            maxComponentLength = 3;
            break;
        case ';':
            if (componentLength != 0 && in == end - 2 && in[1] == '1') {
                *out++ = *in++;
                *out++ = *in;
                componentLength += 2;
                break;
            }
        default:
            if (componentLength < maxComponentLength) {
                __asm__ __volatile__("" : : : "memory");
                unsigned char c = *in;
                if ((unsigned char)(c - 'A') < 26 || (unsigned char)(c - '0') < 10)
                    *out = c;
                else
                    *out = '_';
                out++;
                componentLength++;
            }
            break;
        }
    }
    if (componentLength != 0) {
        *out = 0;
        componentCount++;
    }
    return componentCount;
}

// 0x14B9B8: dead-code scrap (sly2/scrap.h)
SLY2_SCRAP("addiu $2, $2, 0xb8");

// Returns whether the drive holds a PS2 DVD (disk type 0x14), caching the last disk type seen in g_scecd.
// Identical to Sly 1's source.
// Also known as: FValidCdMmode (upstream readable / Sly 1 name).
int IsPs2DvdInDrive()
{
    int diskType = sceCdGetDiskType();
    if (g_scecd != diskType) {
        g_scecd = diskType;
    }
    return g_scecd == 0x14;
}

// Puts the drive in DVD media mode (sceCdMmode(2)) if the last disc seen was a PS2 DVD.
void SetCdMmode()
{
    if (g_scecd == 0x14) {
        sceCdMmode(2);
    }
}

// Boot step for the CD layer; empty in the retail build.
void StartupCd()
{
}

// UpdateCd (Claude agent, 2026-10-17): matched. Keys: `int sector = D_002A2E5C; sector += 0xCA0;` (fixes s0/s1), CRC table via a local `unsigned int *table` with a byte offset `(char *)table + (idx << 2)` (gives addu v0,t1,v0).
extern int g_cds;
extern char D_003DA4F0[];
// CRC-32 of size bytes at data.
static inline unsigned int ComputeCrc32(unsigned char *data, int size)
{
    unsigned int *table = (unsigned int *)D_002A2E68;
    unsigned int crc = 0xFFFFFFFF;
    while (size--)
        crc = *(unsigned int *)((char *)table + (((crc ^ *data++) & 0xFF) << 2)) ^ (crc >> 8);
    return crc;
}

// Per-frame disc-identity poll (state g_cds: 3 idle, 1 read, 2 wait). Idle: if the sound manager exists
// and the disc is not a PS2 DVD, pause the disc users (ShowScreen52D, D_003DA4F0; inferred) and go to read. Read: once
// IsScreen52DShown allows it, start reading the identity sector (LSN 0x132AA3 + 0xCA0) into D_002DD680. Wait: when
// the read completes, compare the sector's CRC-32 to the expected value; on a match resume them (HideScreen52D) and go
// idle, otherwise read again.
void UpdateCd(void)
{
    int pollState = g_cds;
    switch (pollState) {
    case 3:
        if (g_sndmgr.statusResult && !IsPs2DvdInDrive()) {
            ShowScreen52D(D_003DA4F0);
            g_cds = 1;
        }
        break;
    case 1:
        if (IsScreen52DShown(D_003DA4F0)) {
            int sector = D_002A2E5C;
            sector += 0xCA0;
            if (IsPs2DvdInDrive() && sceCdRead(sector, 1, D_002DD680, (void *)D_002A2E50) == pollState)
                g_cds = 2;
        }
        break;
    case 2:
        if (IsCdCommandComplete(0)) {
            if (ComputeCrc32(D_002DD680, 0x800) != D_002A2E60) {
                g_cds = 1;
            } else {
                HideScreen52D(D_003DA4F0);
                g_cds = 3;
            }
        }
        break;
    }
}

// .rodata order (tu-merging.md "Phase 5: data order"): the disc-type names and their format string are literals
// no code in the ELF references (a debug dump the original compiled away), between CpchzTokenizePath's and CdPath's
// literals; defined here under their address names to keep the layout.
namespace RO_ORDER {
extern const char r_D_002A4F08[] __asm__("D_002A4F08") = "PlayStation2 DVD";
extern const char r_D_002A4F20[] __asm__("D_002A4F20") = "PlayStation2 CD";
extern const char r_D_002A4F30[] __asm__("D_002A4F30") = "PlayStation2 CD with CDDA";
extern const char r_D_002A4F50[] __asm__("D_002A4F50") = "PlayStation CD";
extern const char r_D_002A4F60[] __asm__("D_002A4F60") = "PlayStation CD with CDDA";
extern const char r_D_002A4F80[] __asm__("D_002A4F80") = "IllgalMedia";
extern const char r_D_002A4F90[] __asm__("D_002A4F90") = "DVD video";
extern const char r_D_002A4FA0[] __asm__("D_002A4FA0") = "CD-DA";
extern const char r_D_002A4FA8[] __asm__("D_002A4FA8") = "Working";
extern const char r_D_002A4FB0[] __asm__("D_002A4FB0") = "No Disc";
extern const char r_D_002A4FB8[] __asm__("D_002A4FB8") = "Unknown";
extern const char r_D_002A4FC0[] __asm__("D_002A4FC0") = "    %#x = %s\n";
}
// Builds the device path for a game file: ISO-tokenizes `path`, rejoins the components with a backslash and
// prefixes "cdrom0:" when useCdromPrefix is set (otherwise a bare "\PATH" for the host). Writes to `out`.
void CdPath(char *out, char *path, int useCdromPrefix)
{
    char isoPath[256];
    int componentCount;
    int i;
    strcpy(isoPath, "\\");
    strcat(isoPath, path);
    componentCount = CpchzTokenizePath(isoPath);
    char *joined = isoPath;
    for (i = 1; i < componentCount; i++) {
        joined[strlen(joined)] = '\\';
    }
    sprintf(out, "%s\\%s", useCdromPrefix ? "cdrom0:" : "", isoPath);
}

// Starts an asynchronous read of sectorCount sectors at `sector` into `buffer`, retrying until the drive
// accepts the command (sceCdRead returns 1); readMode 1 selects spindle mode 0, anything else mode 1. Records the
// last result and error in s_retCdRead / s_retCdError. Sly 1: ReadCdDirect, plus the read-mode argument.
// Also known as: ReadCdDirect (upstream readable / Sly 1 name; ReadCdDirect__FUiUiPv).
extern "C" void ReadCdDirect(unsigned int sector, unsigned int sectorCount, void *buffer, int readMode)
{
    void *cdReadMode;
    if (readMode == 1) cdReadMode = (void *)D_002A2E50;
    else cdReadMode = (void *)D_002A2E58;
    do {
        s_retCdRead = sceCdRead(sector, sectorCount, buffer, cdReadMode);
        if (s_retCdRead == 1) break;
        sceCdSync(0);
        s_retCdError = sceCdGetError();
    } while (1);
}

// Returns whether the pending CD command has completed (wait != 0 blocks until it has).
// (Sly 1-derived name: FCdCompleted.)
// Also known as: FCdCompleted (upstream readable / Sly 1 name).
int IsCdCommandComplete(int wait)
{
    return sceCdSync(wait == 0) == 0;
}

/* cd's .rodata (address order): the two sceCdRMode read modes {trycount, spindlctrl, datapattern, pad}, the identity
 * sector's LSN base and expected CRC, and the CRC-32 table (reflected polynomial 0xEDB88320). */
/* In the ELF these constants lie in the first part of .rodata, which holds the objects' named constants in TU
 * order before every object's literals and jump tables; a section of their own places them there
 * (tu-merging.md "Phase 6: one link mode", ".rodata.early"). */
extern const char D_002A2E50[4] __attribute__((section(".rodata.early"))) = {0x7F, 0, 0, 0};    /* read mode (fMode 1) */
extern const char D_002A2E58[4] __attribute__((section(".rodata.early"))) = {0x7F, 1, 0, 0};    /* read mode (other) */
extern const int D_002A2E5C __attribute__((section(".rodata.early"))) = 0x132AA3;               /* LSN base of the identity sector */
extern const unsigned int D_002A2E60 __attribute__((section(".rodata.early"))) = 0xACDD3EE4;    /* its expected CRC */
extern const unsigned int D_002A2E68[256] __attribute__((section(".rodata.early"))) = {
    0x00000000, 0x77073096, 0xEE0E612C, 0x990951BA, 0x076DC419, 0x706AF48F, 0xE963A535, 0x9E6495A3,
    0x0EDB8832, 0x79DCB8A4, 0xE0D5E91E, 0x97D2D988, 0x09B64C2B, 0x7EB17CBD, 0xE7B82D07, 0x90BF1D91,
    0x1DB71064, 0x6AB020F2, 0xF3B97148, 0x84BE41DE, 0x1ADAD47D, 0x6DDDE4EB, 0xF4D4B551, 0x83D385C7,
    0x136C9856, 0x646BA8C0, 0xFD62F97A, 0x8A65C9EC, 0x14015C4F, 0x63066CD9, 0xFA0F3D63, 0x8D080DF5,
    0x3B6E20C8, 0x4C69105E, 0xD56041E4, 0xA2677172, 0x3C03E4D1, 0x4B04D447, 0xD20D85FD, 0xA50AB56B,
    0x35B5A8FA, 0x42B2986C, 0xDBBBC9D6, 0xACBCF940, 0x32D86CE3, 0x45DF5C75, 0xDCD60DCF, 0xABD13D59,
    0x26D930AC, 0x51DE003A, 0xC8D75180, 0xBFD06116, 0x21B4F4B5, 0x56B3C423, 0xCFBA9599, 0xB8BDA50F,
    0x2802B89E, 0x5F058808, 0xC60CD9B2, 0xB10BE924, 0x2F6F7C87, 0x58684C11, 0xC1611DAB, 0xB6662D3D,
    0x76DC4190, 0x01DB7106, 0x98D220BC, 0xEFD5102A, 0x71B18589, 0x06B6B51F, 0x9FBFE4A5, 0xE8B8D433,
    0x7807C9A2, 0x0F00F934, 0x9609A88E, 0xE10E9818, 0x7F6A0DBB, 0x086D3D2D, 0x91646C97, 0xE6635C01,
    0x6B6B51F4, 0x1C6C6162, 0x856530D8, 0xF262004E, 0x6C0695ED, 0x1B01A57B, 0x8208F4C1, 0xF50FC457,
    0x65B0D9C6, 0x12B7E950, 0x8BBEB8EA, 0xFCB9887C, 0x62DD1DDF, 0x15DA2D49, 0x8CD37CF3, 0xFBD44C65,
    0x4DB26158, 0x3AB551CE, 0xA3BC0074, 0xD4BB30E2, 0x4ADFA541, 0x3DD895D7, 0xA4D1C46D, 0xD3D6F4FB,
    0x4369E96A, 0x346ED9FC, 0xAD678846, 0xDA60B8D0, 0x44042D73, 0x33031DE5, 0xAA0A4C5F, 0xDD0D7CC9,
    0x5005713C, 0x270241AA, 0xBE0B1010, 0xC90C2086, 0x5768B525, 0x206F85B3, 0xB966D409, 0xCE61E49F,
    0x5EDEF90E, 0x29D9C998, 0xB0D09822, 0xC7D7A8B4, 0x59B33D17, 0x2EB40D81, 0xB7BD5C3B, 0xC0BA6CAD,
    0xEDB88320, 0x9ABFB3B6, 0x03B6E20C, 0x74B1D29A, 0xEAD54739, 0x9DD277AF, 0x04DB2615, 0x73DC1683,
    0xE3630B12, 0x94643B84, 0x0D6D6A3E, 0x7A6A5AA8, 0xE40ECF0B, 0x9309FF9D, 0x0A00AE27, 0x7D079EB1,
    0xF00F9344, 0x8708A3D2, 0x1E01F268, 0x6906C2FE, 0xF762575D, 0x806567CB, 0x196C3671, 0x6E6B06E7,
    0xFED41B76, 0x89D32BE0, 0x10DA7A5A, 0x67DD4ACC, 0xF9B9DF6F, 0x8EBEEFF9, 0x17B7BE43, 0x60B08ED5,
    0xD6D6A3E8, 0xA1D1937E, 0x38D8C2C4, 0x4FDFF252, 0xD1BB67F1, 0xA6BC5767, 0x3FB506DD, 0x48B2364B,
    0xD80D2BDA, 0xAF0A1B4C, 0x36034AF6, 0x41047A60, 0xDF60EFC3, 0xA867DF55, 0x316E8EEF, 0x4669BE79,
    0xCB61B38C, 0xBC66831A, 0x256FD2A0, 0x5268E236, 0xCC0C7795, 0xBB0B4703, 0x220216B9, 0x5505262F,
    0xC5BA3BBE, 0xB2BD0B28, 0x2BB45A92, 0x5CB36A04, 0xC2D7FFA7, 0xB5D0CF31, 0x2CD99E8B, 0x5BDEAE1D,
    0x9B64C2B0, 0xEC63F226, 0x756AA39C, 0x026D930A, 0x9C0906A9, 0xEB0E363F, 0x72076785, 0x05005713,
    0x95BF4A82, 0xE2B87A14, 0x7BB12BAE, 0x0CB61B38, 0x92D28E9B, 0xE5D5BE0D, 0x7CDCEFB7, 0x0BDBDF21,
    0x86D3D2D4, 0xF1D4E242, 0x68DDB3F8, 0x1FDA836E, 0x81BE16CD, 0xF6B9265B, 0x6FB077E1, 0x18B74777,
    0x88085AE6, 0xFF0F6A70, 0x66063BCA, 0x11010B5C, 0x8F659EFF, 0xF862AE69, 0x616BFFD3, 0x166CCF45,
    0xA00AE278, 0xD70DD2EE, 0x4E048354, 0x3903B3C2, 0xA7672661, 0xD06016F7, 0x4969474D, 0x3E6E77DB,
    0xAED16A4A, 0xD9D65ADC, 0x40DF0B66, 0x37D83BF0, 0xA9BCAE53, 0xDEBB9EC5, 0x47B2CF7F, 0x30B5FFE9,
    0xBDBDF21C, 0xCABAC28A, 0x53B39330, 0x24B4A3A6, 0xBAD03605, 0xCDD70693, 0x54DE5729, 0x23D967BF,
    0xB3667A2E, 0xC4614AB8, 0x5D681B02, 0x2A6F2B94, 0xB40BBE37, 0xC30C8EA1, 0x5A05DF1B, 0x2D02EF8D,
};

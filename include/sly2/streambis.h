/* include/sly2/streambis.h -- CStreamBis (DRAFT class name), the file / CD-sector CBinaryInputStream (bis.h), defined
 * by core/binstream/streambis.cpp (vtable 0x2D2918). Callers keep one on the stack: sw.cpp (ExecuteSwTransition),
 * brx_sw.cpp (LoadSwFromBrx), hud.cpp (LoadWidgetSprites), memcard.cpp (load_file_from_disc).
 *
 * Construction and destruction are C functions in the matching build: CStreamBis_ctor (stores the vptr itself) and
 * _$_10CStreamBis (DestroyCStreamBis: vptr, Close, free when flag bit 0). Callers reach them two ways, both kept
 * because they shape the callers' code:
 *   - a local of the class, constructed through the asm-labelled constructor below (the object stays live in a
 *     register, as g++ treats it as returning this) and destroyed through the destructor (_$_10CStreamBis with
 *     in-charge flag 2): brx_sw.cpp, memcard.cpp;
 *   - CStreamBisStorage, room for one without a constructor, and explicit calls of the C entry points: sw.cpp,
 *     hud.cpp. A local of the class itself would get g++'s implicit constructor (a vptr store).
 * streambis.cpp defines the C functions under the members' symbols; g++ 2.95 accepts a member declaration and a C
 * definition that share an assembler name (bis.cpp does the same with __18CBinaryInputStream).
 *
 * Size: 0x64 bytes (the constructor clears through fillBuffer at +0x60 and nothing reads further). The 0x70-byte
 * stand-ins brx_sw.cpp and memcard.cpp had were stack padding: both frames come out the same with the class.
 *
 * Port: the member constructor and destructor are only declarations of the C functions' symbols, so a native build
 * needs CStreamBis_ctor / _$_10CStreamBis under those member names (or overrides these two declarations); sizes follow
 * the native layout through sizeof(CStreamBis). The layout asserts describe the EE (g++ 2.95 puts the vptr after the
 * base's data, at +0x30). */
#ifndef SLY2_STREAMBIS_H
#define SLY2_STREAMBIS_H
#include "bis.h"

class CStreamBis : public CBinaryInputStream
{
public:
    CStreamBis(int size, void *memory, int flags) __asm__("CStreamBis_ctor");   /* streambis.cpp's CStreamBis_ctor */
    ~CStreamBis();                                       /* _$_10CStreamBis: streambis.cpp's DestroyCStreamBis */
    virtual void Fill() __asm__("CStreamBis_Refill");   /* slot 1; key function (streambis.cpp) */
    virtual void Close() __asm__("CStreamBis_Close");   /* slot 2 */

    int spoolSize;            /* 0x34 size of the spool buffer (multiple of 64) */
    byte *spoolBuffer;        /* 0x38 64-byte aligned spool buffer */
    int fd;                   /* 0x3C file descriptor (-1: not a file stream) */
    int readDone;             /* 0x40 bytes of the current CD read known complete */
    int readRequested;        /* 0x44 bytes requested by the current CD read (0: none in flight) */
    int fileBytesLeft;        /* 0x48 bytes of the file not yet requested */
    int reissueOffset;        /* 0x4C offset at which the current read was reissued */
    int sector;               /* 0x50 next sector to read (-1: not a sector stream) */
    int bufferCount;          /* 0x54 number of spool sub-buffers */
    int fullBuffers;          /* 0x58 sub-buffers holding data */
    int readBuffer;           /* 0x5C sub-buffer being consumed */
    int fillBuffer;           /* 0x60 sub-buffer being filled */
};
typedef char assert_CStreamBis_spoolSize[(int)&((CStreamBis *)0)->spoolSize == 0x34 ? 1 : -1];
typedef char assert_CStreamBis_fillBuffer[(int)&((CStreamBis *)0)->fillBuffer == 0x60 ? 1 : -1];
typedef char assert_CStreamBis_size[sizeof(CStreamBis) == 0x64 ? 1 : -1];

/* Room for a CStreamBis that the caller constructs with CStreamBis_ctor and destroys with DestroyCStreamBis
   (pointer-aligned words, so it has the class's size and alignment on any target). */
struct CStreamBisStorage
{
    void *apv[sizeof(CStreamBis) / sizeof(void *)];
};
typedef char assert_CStreamBisStorage_size[sizeof(CStreamBisStorage) == sizeof(CStreamBis) ? 1 : -1];

extern "C" {
CStreamBis *CStreamBis_ctor(CStreamBis *stream, int size, void *memory, int flags);
void DestroyCStreamBis(CStreamBis *stream, int flags) __asm__("_$_10CStreamBis");
void CStreamBis_Close(CStreamBis *stream);
void CStreamBis_Refill(CStreamBis *stream);
}

#endif

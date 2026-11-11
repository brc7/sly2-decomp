// stream.cpp -- OSTRM, a buffered output stream into memory or a file descriptor (debug output when fd < 0).
// The formatted-text code (text.cpp) prints through it: into a caller's memory block (string formatting), to a
// host file, or to the debug console via printf in 0x80-byte pieces. Entry points: the two constructors, CbWrite,
// Flush (the destructor flushes). TU 0x20D5D0..0x20D9D0 (7 functions, address order).
// SLY2_ALIAS: __static_initialization_and_destruction_0=StaticInit_stream
// SLY2_ALIAS: _GLOBAL_$I$*=GlobalConstructors_stream
#include "sly2/slotheap.h"

// 0x20D878 StaticInit_stream and GlobalConstructors_stream: g++-generated for the six engine slot pools.
SLY2_INSTANTIATE_SLOT_POOLS()

void CopyAb(void *pvDst, void *pvSrc, unsigned int cb);    /* CopyAb__FPvT0Ui (returns void; declared void * CbWrite swaps v0/v1) */
extern "C" {
int func_00236960(const char *fmt, ...);                   /* printf */
int sceWrite(int fd, const void *pv, int cb);
}
extern const char D_002A8910[] = "%s";

class OSTRM
{
public:
    char *memTarget;        /* +0x00 memory target (or 0) */
    int memCapacity;        /* +0x04 */
    int memUsed;            /* +0x08 */
    int fd;                 /* +0x0C file target (-1: memory; < 0: debug output) */
    int totalWritten;       /* +0x10 bytes written */
    int bufferedBytes;      /* +0x14 bytes in buffer */
    enum { BUFFER_SIZE = 0x400 };
    char buffer[BUFFER_SIZE];   /* +0x18 staging buffer for file / debug output */
    OSTRM(int fileDescriptor);
    OSTRM(void *data, int size);
    ~OSTRM();
    void Flush();
    int CbWrite(void *data, int size);
};

// Stream to a file descriptor (a negative fd sends the text to the debug console instead).
OSTRM::OSTRM(int fileDescriptor)
{
    fd = fileDescriptor;
    memTarget = 0;
    memCapacity = 0;
    memUsed = 0;
    totalWritten = 0;
    bufferedBytes = 0;
}

// Stream into a caller's memory block of `size` bytes (writes past the end are truncated).
OSTRM::OSTRM(void *data, int size)
{
    memTarget = (char *)data;
    memCapacity = size;
    fd = -1;
    memUsed = 0;
    totalWritten = 0;
    bufferedBytes = 0;
}

// Flushes whatever is still buffered.
OSTRM::~OSTRM() { Flush(); }

// Writes `size` bytes. Memory streams copy straight into the block (clipped to the space left); file/debug
// streams stage through the 0x400-byte buffer, flushing each time it fills. Returns the (possibly clipped) size
// requested, and adds the bytes actually written to totalWritten.
int OSTRM::CbWrite(void *data, int size)
{
    int written = 0;
    if (memTarget && memCapacity) {
        int available;
        if (size == 0) {
            written = 0;
            goto nothing;   /* shares the tail below; a direct return here changes the layout */
        }
        available = memCapacity - memUsed;
        if (available < size)
            size = available;   /* clip to the space left in the block */
    }
    if (size == 0) {
nothing:
        return 0;
    }
    if (memTarget) {
        CopyAb(memTarget + memUsed, data, size);
        written = size;
        memUsed += size;
    } else {
        while (written < size) {
            int freeInBuffer = BUFFER_SIZE - bufferedBytes;
            int remaining = size - written;
            int chunk = freeInBuffer < remaining ? freeInBuffer : remaining;
            if (chunk) {
                CopyAb(buffer + bufferedBytes, (char *)data + written, chunk);
                written += chunk;
                bufferedBytes += chunk;
            }
            if (bufferedBytes == BUFFER_SIZE)
                Flush();
        }
    }
    totalWritten += written;
    return size;
}

// Sends the staged bytes to the file (sceWrite) or, for fd < 0, to the debug console with printf("%s") in
// NUL-terminated pieces of at most 0x80 bytes. Memory streams have nothing to flush.
void OSTRM::Flush()
{
    if (bufferedBytes == 0 || memTarget)
        return;
    if (fd < 0) {
        char *src = buffer;
        int offset;
        for (offset = 0; offset < bufferedBytes; ) {
            char line[0x88];
            int remaining = bufferedBytes - offset;
            int chunk = 0x80;
            if (remaining < 0x80)
                chunk = remaining;
            CopyAb(line, src, chunk);
            offset += chunk;
            src += chunk;
            line[chunk] = 0;
            func_00236960(D_002A8910, line);
        }
    } else {
        sceWrite(fd, buffer, bufferedBytes);
    }
    bufferedBytes = 0;
}

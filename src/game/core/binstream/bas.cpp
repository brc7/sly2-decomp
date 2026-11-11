// bas.cpp -- CBinaryAsyncStream: double-buffered asynchronous reads from the host file system or the CD.
// The spool buffer is used in two halves: one is being read by the consumer (current, currentSize, position currentPos) while the
// next is read asynchronously into the other. When the consumer has used the current half, FinishSpooling swaps in the
// completed one. Entry points: the constructor, OpenBasFromFileKey / OpenSector, Close, StartSpooling, IsSpooling,
// IsSpoolingComplete, FinishSpooling, Spool, Skip. TU 0x132570..0x132B00 (13 functions, address order).
// SLY2_ALIAS: __static_initialization_and_destruction_0=StaticInit_bas
// SLY2_ALIAS: _GLOBAL_$I$*=GlobalConstructors_bas
#include "sly2/slotheap.h"
#include "sly2/scrap.h"
#include "sly2/port.h"

// 0x1329A8 StaticInit_bas and GlobalConstructors_bas: g++-generated for the six engine slot pools.
SLY2_INSTANTIATE_SLOT_POOLS()

typedef unsigned char byte;
typedef unsigned int uint;
enum BASK { BASK_Nil = -1, BASK_Host = 0, BASK_Cd = 1 };

/* A CD file record as OpenBasFromFileKey reads it. */
struct CDFILE { uint sector; int size; int kind; };

class CBinaryAsyncStream {
public:
    BASK kind;        /* 0x00 */
    int fd;           /* 0x04 */
    uint sector;     /* 0x08 */
    int pendingSize;   /* 0x0c */
    byte *pendingBuffer; /* 0x10 */
    int spoolSize;      /* 0x14 size of the spool buffer (two halves) */
    byte *spoolBuffer;    /* 0x18 spool buffer, 64-byte aligned */
    int currentSize;           /* 0x1c */
    byte *current;         /* 0x20 */
    int currentPos;        /* 0x24 */
    int bytesNotRead;  /* 0x28 */
    int fileSize;       /* 0x2c */
    ~CBinaryAsyncStream();
    int OpenSector(uint isector, uint cb);
    void Close();
    void StartSpooling();
    bool IsSpooling();
    bool IsSpoolingComplete();
    void FinishSpooling();
    void Spool();
    void Skip(int cb);
};
typedef char assert_CBinaryAsyncStream_size[sizeof(CBinaryAsyncStream) == 0x30 ? 1 : -1];

extern "C" {
int sceIoctl(int fd, int req, void *pv);
int sceCdSync(int mode);
int sceCdGetError(void);
int sceClose(int fd);
int sceRead(int fd, void *pv, int cb);
void ReadCdDirect(uint isector, uint csector, void *pv, int n);
}

/* The constructor takes (cbSpool, pvSpool) although its current symbol mangles to (void *) only (Sly 1's signature);
   it is defined under that symbol as a C-linkage function until the symbol is renamed. */
// Constructs a closed stream over the caller's spool buffer (spoolSize bytes at spoolMemory).
// Also known as: CBinaryAsyncStream constructor (upstream readable / Sly 1 name; __18CBinaryAsyncStreamiPv).
extern "C" CBinaryAsyncStream *__18CBinaryAsyncStreamPv(CBinaryAsyncStream *stream, int spoolSize, void *spoolMemory)
{
    stream->spoolSize = spoolSize;
    stream->fd = -1;
    stream->spoolBuffer = (byte *)(((sptr)spoolMemory + 0x3F) & ~0x3F);
    stream->sector = 0;
    stream->pendingBuffer = 0;
    stream->current = 0;
    stream->fileSize = 0;
    stream->bytesNotRead = 0;
    stream->pendingSize = 0;
    stream->currentPos = 0;
    stream->currentSize = 0;
    return stream;
}

// Destructor: closes the stream.
CBinaryAsyncStream::~CBinaryAsyncStream() { Close(); }

// Opens the stream on a file key if it names a disc file (kind 2: sector range); returns whether it did.
// Also known as: FOpenFile (upstream readable / Sly 1 name; FOpenFile__18CBinaryAsyncStreamP13CFileLocation).
extern "C" int OpenBasFromFileKey(CBinaryAsyncStream *stream, CDFILE *fileKey)
{
    int opened = 0;
    if (fileKey->kind == 2)
        opened = stream->OpenSector(fileKey->sector, fileKey->size);
    return opened;
}

// 0x132638: dead-code scrap (sly2/scrap.h)
SLY2_SCRAP("addiu $sp, $sp, 0x30");

// Opens the stream on `size` bytes starting at disc sector `sector` and starts the first read; returns 1.
// (Sly 1-derived name: CBinaryAsyncStream::FOpenSector.)
// Also known as: FOpenSector (upstream readable / Sly 1 name).
int CBinaryAsyncStream::OpenSector(uint firstSector, uint size)
{
    sector = firstSector;
    fileSize = size;
    bytesNotRead = size;
    kind = BASK_Cd;
    current = spoolBuffer;
    currentSize = 0;
    currentPos = 0;
    StartSpooling();
    return 1;
}

// Closes the stream: closes the host file, or waits out a CD read in flight.
void CBinaryAsyncStream::Close()
{
    switch (kind) {
    case BASK_Host:
        if (fd >= 0)
            sceClose(fd);
        fd = -1;
        break;
    case BASK_Cd:
        if (IsSpooling()) {
            sceCdSync(0);
            FinishSpooling();
        }
        sector = 0;
        break;
    }
    kind = BASK_Nil;
}

// Starts reading the next piece (at most half the spool buffer) into the half the consumer is not using, unless a
// read is in flight or the file is exhausted.
void CBinaryAsyncStream::StartSpooling()
{
    int halfSize = spoolSize / 2;
    if (!IsSpooling() && bytesNotRead != 0) {
        int size = bytesNotRead < halfSize ? bytesNotRead : halfSize;
        byte *dest = (current == spoolBuffer) ? spoolBuffer + halfSize : spoolBuffer;
        pendingSize = size;
        pendingBuffer = dest;
        switch (kind) {
        case BASK_Host:
            sceRead(fd, dest, size);
            return;
        case BASK_Cd:
            ReadCdDirect(sector, (uint)(size + 0x7FF) >> 11, dest, 1);
            break;
        }
    }
}

// Returns whether a read is in flight. (Sly 1-derived name: CBinaryAsyncStream::FSpooling.)
// Also known as: FSpooling (upstream readable / Sly 1 name).
bool CBinaryAsyncStream::IsSpooling()
{
    return pendingSize != 0;
}

// Returns whether the read in flight has completed, polled without blocking, and only once the consumer has used
// the current half; a read error abandons the read (returns false).
// (Sly 1-derived name: CBinaryAsyncStream::FSpoolingComplete.)
// Also known as: FSpoolingComplete (upstream readable / Sly 1 name).
bool CBinaryAsyncStream::IsSpoolingComplete()
{
    int status = 0;
    bool isComplete = false;
    if (currentSize == currentPos && IsSpooling())
    {
        switch (kind)
        {
            case BASK_Host:
            {
                int isExecuting;
                status = sceIoctl(fd, 1, &isExecuting);
                isComplete = (isExecuting == 0);
                break;
            }
            case BASK_Cd:
            {
                isComplete = (sceCdSync(1) == 0);
                if (isComplete)
                {
                    status = sceCdGetError();
                }
                break;
            }
        }
    }
    if (status)
    {
        pendingSize = 0;
        pendingBuffer = 0;
        isComplete = false;
    }
    return isComplete;
}

// If the read in flight has completed: makes it the current half (advancing the sector and the bytes left) and
// rewinds the consumer to its start.
void CBinaryAsyncStream::FinishSpooling()
{
    if (!IsSpoolingComplete())
        return;
    if (kind == BASK_Cd)
    {
        int bytes;
        if (pendingSize <= -1)
        {
            bytes = pendingSize + 0x7ffu;
        }
        else
        {
            bytes = pendingSize;
        }
        sector += bytes >> 0x0b;
    }
    bytesNotRead -= pendingSize;
    current = pendingBuffer;
    currentSize = pendingSize;
    pendingSize = 0;
    pendingBuffer = 0;
    currentPos = 0;
}

// Keeps the stream going (call regularly): host reads are started then completed; CD reads completed then the
// next one started.
void CBinaryAsyncStream::Spool()
{
    switch (kind) {
    case BASK_Host:
        StartSpooling();
        FinishSpooling();
        return;
    case BASK_Cd:
        FinishSpooling();
        StartSpooling();
        return;
    }
}

// Advances the consumer's position in the current half by `size` bytes.
void CBinaryAsyncStream::Skip(int size)
{
    currentPos += size;
}

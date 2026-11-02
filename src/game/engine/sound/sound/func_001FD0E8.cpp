// func_001FD0E8: LoadSoundSectionFromBrx (0x1FD0E8), written from the sound section's layout (our names; Lilly's decomp has it as unmatched).
// Reads the level's sound section: the sound bank's file record (the loaded bank is kept when the record's file id is unchanged), per-sound kinds (S8),
// the effect id -> sound index table, the ambience stream record, six ids, then the dialogue/music stream records (one file record per language;
// the record of the language in g_nLang = D_002E1E54 is kept) and the loose file records. The allocation requests are {cb, cbAlign} records built by
// inline helpers; every helper call has its own SWALLOC local, which the compiler gives the same stack slot (sp+0x40).
// Matched without pins or asm: the loops use `int *pn = &anKind[i]` (pointer formed before the read), a separate counter j for the index table,
// memset declared as returning void * (a void return moved a v0/v1 choice), and the ambience stream through a temp (no s-register held for &ambience).
typedef unsigned char    uchar;
typedef unsigned short   ushort;
typedef unsigned int     uint;
typedef uchar byte;
typedef unsigned long ulong;
struct SNDMGR;
class CBinaryStreamBase
{
public:
    int m_grfbis;
    byte *m_pb;
    int m_cb;
    int m_ibStream;
    byte *m_pbRaw;
    int m_cbRaw;
    int m_grfDecomp;
    int m_cbSpillOver;
    int m_unk20;
    byte *m_abWindow;
    int m_unk28;
    int m_unk2c;
};
class CBinaryInputStream : public CBinaryStreamBase
{
public:
    int m_cbSpool;
    byte *m_abSpool;
    int m_fd;
    int m_cbDone;
    int m_cbRequest;
    int m_cbRemaining;
    int m_cbRetry;
    int m_isector;
    int m_cbuf;
    int m_cbufFill;
    int m_icbufRead;
    int m_icbufWrite;
    void Read(int cb, void *pv);
    uchar U8Read();
    char S8Read();
    short S16Read();
    int S32Read();
    float F32Read();
    void ReadFileRecord(void *pv);
};
struct FILEREC
{
    uint aw[16];
};
struct SWALLOC
{
    int cb;
    int cbAlign;
};
void *PvAllocSwImpl(SWALLOC *pswalloc);
void *PvAllocSwClearImpl(SWALLOC *pswalloc);
inline void *PvAllocSwLoad(int cb, int cbAlign)
{
    SWALLOC swalloc;
    swalloc.cb = cb;
    SWALLOC *pswalloc = &swalloc;
    pswalloc->cbAlign = cbAlign;
    return PvAllocSwImpl(pswalloc);
}
inline void *PvAllocSwClearLoad(int cb, int cbAlign)
{
    SWALLOC swalloc;
    swalloc.cb = cb;
    SWALLOC *pswalloc = &swalloc;
    pswalloc->cbAlign = cbAlign;
    return PvAllocSwClearImpl(pswalloc);
}
struct STREAMSND;
struct SNDSTREAM
{
    FILEREC rec;
    int id;
    SNDSTREAM *psndstreamNext;
    int nUnk48;
    int nUnk4C;
    float gUnk50;
    int cpair;
    ulong *apair;
};
struct SNDFILE
{
    FILEREC rec;
    int id;
    SNDFILE *psndfileNext;
};
struct SNDMGR
{
    byte ab[0x18];
    int csfx;
    byte abUnk1c[0x8c];
    FILEREC rec;
    void *pbank;
    SNDSTREAM *psndstreamFirst;
    SNDFILE *psndfileFirst;
    FILEREC *precAmbience;
    int csnd;
    int *anKind;
    int *aisnd;
    byte abUnk104[0xCC];
    int cUnk1D0;
    int anUnk1D4[6];
    byte abUnk1EC[0x3C];
    STREAMSND *psndAmbience;
};
extern int D_002E1E54;
extern "C" void func_002321B8(void *);
extern "C" int func_00231C48(void);
extern "C" void *func_002333B4(void *, int, uint);
extern "C" STREAMSND *func_001FC138(FILEREC *, int);
extern "C" void func_00259B48(void *, STREAMSND *);
inline int FSameFileId(FILEREC *prec, FILEREC *precOther)
{
    if (prec == precOther)
        return 1;
    if (prec == 0 || precOther == 0)
        return 0;
    return (prec->aw[2] ^ prec->aw[14]) == (precOther->aw[2] ^ precOther->aw[14]);
}
extern "C" void LoadSoundSectionFromBrx(SNDMGR *psndmgr, CBinaryInputStream *pbis) __asm__("func_001FD0E8");
/** @sly2-annot addr=0x1FD0E8
 * Purpose: Read level sound section: bank file record, per-sound kinds, sfxid->index table, ambience stream,
 *   dialogue/music stream records per language, loose file records
 * Info: proposed name LoadSoundSectionFromBrx (T1); TU sound
 * Notes: audio format doc cites this function by this name (CONFIRMED(code) in the doc)
 * Confidence: high [T1 0.90] - evidence: static analysis; sources:
 *   r7_1F8DD8+verdicts_tu_r7r8+format-doc:audio
 * See: docs/engine/sound.md#1-sound-manager-soundcpp-0x1faf70-0x1ff398, docs/engine/sound.md#6-key-functions,
 *   docs/engine/world-sw.md#loadswfrombrx-0x13da18-matched-hand-file
 * @end-sly2-annot */
extern "C" void LoadSoundSectionFromBrx(SNDMGR *psndmgr, CBinaryInputStream *pbis)
{
    FILEREC rec;
    pbis->ReadFileRecord(&rec);
    if (!FSameFileId(&rec, &psndmgr->rec))
    {
        psndmgr->rec = rec;
        if (psndmgr->pbank)
        {
            func_002321B8(psndmgr->pbank);
            while (func_00231C48())
                ;
            psndmgr->pbank = 0;
        }
    }
    int i;
    psndmgr->csnd = pbis->S16Read();
    psndmgr->anKind = (int *)PvAllocSwClearLoad(psndmgr->csnd * sizeof(int), 0x10);
    for (i = 0; i < psndmgr->csnd; i++)
    {
        int *pn = &psndmgr->anKind[i];
        *pn = pbis->S8Read();
    }
    psndmgr->csfx = pbis->S16Read();
    psndmgr->aisnd = (int *)PvAllocSwLoad(psndmgr->csfx * sizeof(int), 0x10);
    func_002333B4(psndmgr->aisnd, -1, psndmgr->csfx * sizeof(int));
    int j;
    for (j = 0; j < psndmgr->csnd; j++)
        psndmgr->aisnd[pbis->S16Read()] = j;
    psndmgr->cUnk1D0 = pbis->S16Read();
    if (psndmgr->cUnk1D0 > 0)
    {
        psndmgr->precAmbience = (FILEREC *)PvAllocSwClearLoad(sizeof(FILEREC), 0x10);
        pbis->ReadFileRecord(psndmgr->precAmbience);
        STREAMSND *psnd = func_001FC138(psndmgr->precAmbience, 4);
        func_00259B48(&psndmgr->psndAmbience, psnd);
    }
    func_002333B4(psndmgr->anUnk1D4, -1, sizeof(psndmgr->anUnk1D4));
    for (i = 0; i < psndmgr->cUnk1D0; i++)
        psndmgr->anUnk1D4[i] = pbis->S16Read();
    int clang = pbis->U8Read();
    int ilangMatch = 0;
    for (int ilang = 0; ilang < clang; ilang++)
    {
        if (pbis->U8Read() == D_002E1E54)
            ilangMatch = ilang;
    }
    int cstream = pbis->S32Read();
    SNDSTREAM *astream = (SNDSTREAM *)PvAllocSwClearLoad(cstream * sizeof(SNDSTREAM), 0x10);
    for (i = 0; i < cstream; i++)
    {
        SNDSTREAM *pstream = &astream[i];
        pstream->id = pbis->S16Read();
        pstream->nUnk48 = pbis->S16Read();
        for (int ilang = 0; ilang < clang; ilang++)
        {
            FILEREC recLang;
            pbis->ReadFileRecord(&recLang);
            int nUnk4C = pbis->S8Read();
            float gUnk50 = pbis->F32Read();
            if (ilang == ilangMatch)
            {
                pstream->rec = recLang;
                pstream->nUnk4C = nUnk4C;
                pstream->gUnk50 = gUnk50;
            }
        }
        pstream->cpair = pbis->S32Read();
        pstream->apair = (ulong *)PvAllocSwLoad(pstream->cpair * 8, 0x10);
        pbis->Read(pstream->cpair * 8, pstream->apair);
        pstream->psndstreamNext = psndmgr->psndstreamFirst;
        psndmgr->psndstreamFirst = pstream;
    }
    int cfile = pbis->S32Read();
    SNDFILE *afile = (SNDFILE *)PvAllocSwClearLoad(cfile * sizeof(SNDFILE), 0x10);
    for (i = 0; i < cfile; i++)
    {
        SNDFILE *pfile = &afile[i];
        pfile->id = pbis->S16Read();
        pbis->ReadFileRecord(pfile);
        pfile->psndfileNext = psndmgr->psndfileFirst;
        psndmgr->psndfileFirst = pfile;
    }
}

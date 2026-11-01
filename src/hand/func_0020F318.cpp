// func_0020F318: ported from Lilly's personal Sly 2 decomp (src/P2/sw.c), reduced to the declarations it needs.
// Every global is named by its address; the function keeps its readable name and gets the team symbol through __asm__.
// Globals: D_003E1C4C (g_alevellist), D_003E1C48 (g_anEpisodeOfLevel), D_003E1C44 (g_anWorldOfLevel), D_003E1C3C (g_nUnk3E1C3C), D_003E1C38 (g_cLevelTable), D_003E1C40 (g_aLevelRecord), D_003E1C50 (g_aWorldEntryRecord)
// 0x0020F318 LoadLevelTableFromBrx  (T1038)
// u32 clevel, u32 (unused); clevel x { file record (0x40 bytes); u8 world; u8 episode; u8 fEntry (the record is also the entry of its world:
// copied to g_aWorldEntryRecord[world]); u8 n + n u16 level numbers }.
#include "sly2/sw.h"
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
    uchar U8Read();
    ushort U16Read();
    uint U32Read();
    void ReadFileRecord(void *pv);
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
struct LEVELLIST
{
      int c;
      short *a;
};
extern int D_003E1C38;
extern int D_003E1C3C;
extern FILEREC *D_003E1C40;
extern int *D_003E1C44;
extern int *D_003E1C48;
extern LEVELLIST *D_003E1C4C;
extern FILEREC *D_003E1C50;
extern "C" void LoadLevelTableFromBrx(SW *psw, CBinaryInputStream *pbis) __asm__("func_0020F318");
extern "C" void LoadLevelTableFromBrx(SW *psw, CBinaryInputStream *pbis)
{
    D_003E1C38 = pbis->U32Read();
    D_003E1C3C = pbis->U32Read();
    D_003E1C40 = (FILEREC *)PvAllocSwLoad(D_003E1C38 * sizeof(FILEREC), 0x10);
    D_003E1C44 = (int *)PvAllocSwLoad(D_003E1C38 * sizeof(int), 0x10);
    D_003E1C48 = (int *)PvAllocSwLoad(D_003E1C38 * sizeof(int), 0x10);
    D_003E1C4C = (LEVELLIST *)PvAllocSwLoad(D_003E1C38 * sizeof(LEVELLIST), 0x10);
    D_003E1C50 = (FILEREC *)PvAllocSwClearLoad(9 * sizeof(FILEREC), 0x10);
    for (int ilevel = 0; ilevel < D_003E1C38; ilevel++)
    {
        pbis->ReadFileRecord(&D_003E1C40[ilevel]);
        D_003E1C44[ilevel] = pbis->U8Read();
        D_003E1C48[ilevel] = pbis->U8Read();
        if (pbis->U8Read())
            D_003E1C50[D_003E1C44[ilevel]] = D_003E1C40[ilevel];
        LEVELLIST *plevellist = &D_003E1C4C[ilevel];
        plevellist->c = pbis->U8Read();
        plevellist->a = (short *)PvAllocSwLoad(plevellist->c * sizeof(short), 0x10);
        for (int i = 0; i < plevellist->c; i++)
            plevellist->a[i] = pbis->U16Read();
    }
}

struct QW16 { int a[4]; } __attribute__((aligned(16)));
struct V2 { int a[2]; };
struct V3 { int a[3]; };
struct CRef {
    int m_tagk;
    union { int m_tag; float m_g; void *m_pv; };
    CRef(int tagk) { InitTag(tagk); }
    CRef(const CRef &);
    ~CRef() { Decref(); }
    void InitTag(int);
    void Decref();
};
struct ENT {
    short ch;
    signed char kind;
    unsigned char fBits;
    unsigned short ib;
    unsigned short iChild;
    unsigned short iSibling;
};
struct TRIE;
struct KEY;
struct OUT { ENT *pe; int n; };
extern "C" int FFindSaveVar(TRIE *, KEY *, OUT *);
/** @sly2-annot addr=0x1D80C8
 * Purpose: Write a CRef into a save var by kind (bitfield set/clear for packed bools)
 * Info: proposed name RefSetSaveVar (T1); TU save
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r6_1D5150
 * See: docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#save-0x1d7b88-0x1d93d8,
 *   docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
extern "C" CRef RefSetSaveVar(TRIE *p, KEY *key, const CRef &val, char *pb)
{
    CRef ref(0x24);
    OUT out;
    ENT *pe;
    if (!FFindSaveVar(p, key, &out) || (pe = out.pe)->kind == -1)
        return ref;
    {
        unsigned char *pv = (unsigned char *)pb + pe->ib;
        switch (pe->kind) {
        case 0:
        case 1:
        case 2:
        case 9:
        case 10:
            ((int *)pv)[out.n] = val.m_tag;
            break;
        case 3:
            ((float *)pv)[out.n] = val.m_g;
            break;
        case 4:
            if (pe->fBits) {
                int n = out.n;
                unsigned char b;
                pv += n / 8;
                b = 1 << (n % 8);
                if (val.m_tag)
                    *pv |= b;
                else
                    *pv &= ~b;
            } else {
                *(int *)pv = val.m_tag;
            }
            break;
        case 5:
        case 6:
            ((QW16 *)pv)[out.n] = *(QW16 *)val.m_pv;
            break;
        case 7:
            ((V2 *)pv)[out.n] = *(V2 *)val.m_pv;
            break;
        case 8:
            ((V3 *)pv)[out.n] = *(V3 *)val.m_pv;
            break;
        }
    }
    return ref;
}

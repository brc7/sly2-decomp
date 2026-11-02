struct VECTOR { float x, y, z; } __attribute__((aligned(16)));
struct QW16 { int a[4]; } __attribute__((aligned(16)));
struct V2 { int a[2]; };
struct V3 { int a[3]; };
struct CRef {
    int m_tagk;
    int m_tag;
    CRef() { m_tagk = -1; }
    CRef(const CRef &);
    ~CRef() { Decref(); }
    CRef &operator=(int);
    CRef &operator=(float);
    CRef &operator=(bool);
    CRef &operator=(const VECTOR &);
    CRef &operator=(const QW16 &);
    CRef &operator=(const V2 &);
    CRef &operator=(const V3 &);
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
/** @sly2-annot addr=0x1D7ED8
 * Purpose: Read a save var as a Splice CRef: switch on kind (int/float/bool or bitfield/vector/qw/v2/v3...)
 * Info: proposed name RefGetSaveVar (T1); TU save
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r6_1D5150
 * See: docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#save-0x1d7b88-0x1d93d8
 * @end-sly2-annot */
extern "C" CRef RefGetSaveVar(TRIE *p, KEY *key, char *pb)
{
    CRef ref;
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
        case 10:
            ref = ((int *)pv)[out.n];
            break;
        case 3:
            ref = ((float *)pv)[out.n];
            break;
        case 4:
            if (pe->fBits) {
                int n = out.n;
                unsigned char b;
                pv += n / 8;
                b = 1 << (n % 8);
                ref = (bool)((*pv & b) != 0);
            } else {
                ref = *(bool *)pv;
            }
            break;
        case 5:
            ref = ((VECTOR *)pv)[out.n];
            break;
        case 6:
            ref = ((QW16 *)pv)[out.n];
            break;
        case 7:
            ref = ((V2 *)pv)[out.n];
            break;
        case 8:
            ref = ((V3 *)pv)[out.n];
            break;
        case 9:
            ref = ((int *)pv)[out.n];
            break;
        }
    }
    return ref;
}

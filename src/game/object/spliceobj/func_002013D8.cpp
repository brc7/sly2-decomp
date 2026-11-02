struct CRef;
extern "C" {
void SetTag__4CRef4TAGK(CRef *pr, int tagk);
void func_001291A8(CRef *pr, const CRef *ps);
CRef *func_00129658(CRef *pd, const CRef *ps);
void func_00129EC8(CRef *pr);
}
struct CRef {
    int tagk;
    int tag;
    CRef() { tagk = -1; }
    CRef(int k) { SetTag__4CRef4TAGK(this, k); }
    CRef(const CRef &r) { func_001291A8(this, &r); }
    CRef &operator=(const CRef &r) { func_00129658(this, &r); return *this; }
    ~CRef() { func_00129EC8(this); }
};
struct E { int pad[2]; CRef ref8; E *next; };
struct L { int count; E *first; };
extern "C" CRef func_00201360(int count, CRef *values, int *tags, ...)
    __asm__("RefSetArgsFromSplice__FiP4CRefP4OTYPe");
/** @sly2-annot addr=0x2013D8
 * Purpose: Convert each element of a Splice list into dest[] via RefSetArgsFromSplice; returns void ref
 * Info: proposed name RefSetArgsFromSpliceList (T2); TU spliceobj
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/player.md#tu_1d7238-0x1d7238-0x1d7a30,
 *   docs/reference/functions/splice-scripting.md#spliceobj-0x1ffcf8-0x201620
 * @end-sly2-annot */
extern "C" CRef func_002013D8(L *list, int tag, int unused, int *count, int *dest)
{
    if (list->count == 0) {
        *count = 0;
        return CRef(0x24);
    }
    *count = 0;
    for (E *e = list->first; e; e = e->next) {
        CRef r;
        r = func_00201360(1, &e->ref8, &tag, &dest[*count]);
        ++*count;
    }
    return CRef(0x24);
}

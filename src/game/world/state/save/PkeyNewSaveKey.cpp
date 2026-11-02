typedef char *va_list;
#define va_start(ap, last) (ap = ((char *)__builtin_next_arg(last) - (__builtin_args_info(2) >= 8 ? 0 : (8 - __builtin_args_info(2)) * 8)))
struct MSG { int cb; int nKind; };
struct FRAME {
    int a[4];
    FRAME();
    ~FRAME();
};
struct V { int c; void *pv; };
V *func_001AE1D0(MSG *pmsg);
void *func_001AE238(MSG *pmsg, void *pv);
void BuildSaveKeyV(void *p, va_list ap, V *pv);
/** @sly2-annot addr=0x1D88D0
 * Purpose: Varargs: allocate a key (8 B header) with 0x1AE1D0 allocator then move its id array via 0x1AE238
 *   (realloc-like)
 * Info: proposed name PkeyNewSaveKey (T1); TU save
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/save-memcard.md#save-0x1d7b88-0x1d93d8
 * @end-sly2-annot */
extern "C" V *PkeyNewSaveKey(void *p, ...)
{
    FRAME frame;
    MSG msg;
    MSG *pmsg = &msg;
    va_list ap;
    V *pv;
    va_start(ap, p);
    pmsg->cb = 8;
    pmsg->nKind = 0x10;
    pv = func_001AE1D0(pmsg);
    BuildSaveKeyV(p, ap, pv);
    {
        void *pvOld = pv->pv;
        pmsg->cb = pv->c * 2;
        pmsg->nKind = 0x10;
        pv->pv = func_001AE238(pmsg, pvOld);
    }
    return pv;
}

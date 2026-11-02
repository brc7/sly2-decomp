/* EE (single-float EABI) stdarg, from gcc's va-mips.h */
typedef char *va_list;
#define va_start(ap, last) (ap = ((char *)__builtin_next_arg(last) - (__builtin_args_info(2) >= 8 ? 0 : (8 - __builtin_args_info(2)) * 8)))
struct MSG { int cb; int nKind; };
void *func_001AE318(struct MSG *pmsg);
void func_001D87C0(void *p, va_list ap, void *pv);
/** @sly2-annot addr=0x1D89A8
 * Purpose: Varargs: allocate a key in slot heap (PvAllocSlot) and fill via BuildSaveKeyV
 * Info: proposed name PkeyAllocSaveKey (T1); TU save
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/save-memcard.md#save-0x1d7b88-0x1d93d8
 * @end-sly2-annot */
void *func_001D89A8(void *p, ...) {
    struct MSG msg;
    va_list ap;
    void *pv;
    va_start(ap, p);
    msg.cb = 8;
    msg.nKind = 0x10;
    pv = func_001AE318(&msg);
    func_001D87C0(p, ap, pv);
    return pv;
}

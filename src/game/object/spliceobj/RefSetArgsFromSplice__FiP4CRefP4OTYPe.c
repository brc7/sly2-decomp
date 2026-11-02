/* EE (single-float EABI) stdarg, from gcc's va-mips.h */
typedef char *va_list;
#define va_start(ap, last) (ap = ((char *)__builtin_next_arg(last) - (__builtin_args_info(2) >= 8 ? 0 : (8 - __builtin_args_info(2)) * 8)))
struct CRef { int tagk; int tag; };
struct CRef *func_00201110(struct CRef *ret, int c, struct CRef *aref, int *aotype, int *an);
/** @sly2-annot addr=0x201360
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/engine/splice-scripting.md#1-layout-of-the-runtime,
 *   docs/reference/functions/splice-scripting.md#spliceobj-0x1ffcf8-0x201620
 * @end-sly2-annot */
struct CRef *RefSetArgsFromSplice__FiP4CRefP4OTYPe(struct CRef *ret, int c, struct CRef *aref, int *aotype, ...)
{
    va_list ap;
    int an[32];
    int i;
    va_start(ap, aotype);
    for (i = 0; i < c; i++) {
        ap += 8;
        an[i] = *(int *)(ap - 8);
    }
    func_00201110(ret, c, aref, aotype, an);
    return ret;
}

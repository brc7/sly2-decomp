typedef char *va_list;
#define va_start(ap, last) (ap = ((char *)__builtin_next_arg(last) - (__builtin_args_info(2) >= 8 ? 0 : (8 - __builtin_args_info(2)) * 8)))
int s1_vsprintf(void *f, char *fmt, va_list ap);
/** @sly2-annot addr=0x21ECD8
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/core-library.md#text-0x21d638-0x21efd0
 * @end-sly2-annot */
int fiprintf(void *f, char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    return s1_vsprintf(f, fmt, ap);
}

struct OSTRM { char *pbMem; int cbMemMax; int cbMem; int fd; int cbTotal; int cbBuf; char ab[0x400]; };
void __5OSTRMi(struct OSTRM *p, int fd);
int CchOstrmPrintf__FP5OSTRMPcT1(struct OSTRM *p, char *fmt, char *ap);
void _$_5OSTRM(struct OSTRM *p, int f);
/** @sly2-annot addr=0x21EBB8
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/core-library.md#text-0x21d638-0x21efd0
 * @end-sly2-annot */
int vprintf(char *fmt, char *ap)
{
    struct OSTRM s;
    int r;
    __5OSTRMi(&s, -1);
    r = CchOstrmPrintf__FP5OSTRMPcT1(&s, fmt, ap);
    _$_5OSTRM(&s, 2);
    return r;
}

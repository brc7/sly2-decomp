struct OSTRM { char *pbMem; int cbMemMax; int cbMem; int fd; int cbTotal; int cbBuf; char ab[0x400]; };
void func_0020D5D0(struct OSTRM *p, int fd);
int func_0021EAC8(struct OSTRM *p, char *fmt, char *ap);
void func_0020D618(struct OSTRM *p, int f);
/** @sly2-annot addr=0x21EBB8
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/core-library.md#text-0x21d638-0x21efd0
 * @end-sly2-annot */
int vprintf(char *fmt, char *ap)
{
    struct OSTRM s;
    int r;
    func_0020D5D0(&s, -1);
    r = func_0021EAC8(&s, fmt, ap);
    func_0020D618(&s, 2);
    return r;
}

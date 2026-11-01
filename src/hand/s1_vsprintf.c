struct OSTRM { char *pbMem; int cbMemMax; int cbMem; int fd; int cbTotal; int cbBuf; char ab[0x400]; };
void func_0020D5F0(struct OSTRM *p, char *pb, int cb);
int func_0021EAC8(struct OSTRM *p, char *fmt, char *ap);
void func_0020D618(struct OSTRM *p, int f);
/** @sly2-annot addr=0x21EC60
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/core-library.md#text-0x21d638-0x21efd0
 * @end-sly2-annot */
int s1_vsprintf(char *pchzDest, char *fmt, char *ap)
{
    struct OSTRM s;
    int len;
    func_0020D5F0(&s, pchzDest, 0);
    len = func_0021EAC8(&s, fmt, ap);
    pchzDest[len] = 0;
    func_0020D618(&s, 2);
    return len;
}

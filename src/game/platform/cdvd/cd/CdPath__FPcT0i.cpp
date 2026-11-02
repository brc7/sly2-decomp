extern "C" {
char *strcpy(char *, const char *);
char *strcat(char *, const char *);
unsigned int strlen(const char *);
int sprintf(char *, const char *, ...);
}
int CpchzTokenizePath(char *pchz);
/** @sly2-annot addr=0x14BBB0
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/core-library.md#cd-0x14b7f8-0x14bea8
 * @end-sly2-annot */
void CdPath(char *pchzOut, char *pchzIn, int fCdrom)
{
    char achPath[256];
    int c;
    int i;
    strcpy(achPath, "\\");
    strcat(achPath, pchzIn);
    c = CpchzTokenizePath(achPath);
    char *pch = achPath;
    for (i = 1; i < c; i++) {
        pch[strlen(pch)] = '\\';
    }
    sprintf(pchzOut, "%s\\%s", fCdrom ? "cdrom0:" : "", achPath);
}

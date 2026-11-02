extern "C" {
char *strupr(char *);
char *strcat(char *, const char *);
unsigned int strlen(const char *);
}
/** @sly2-annot addr=0x14B7F8
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/engine/core-library.md#4-binary-input-streams-and-cd, docs/engine/core-library.md#6-source-files,
 *   docs/reference/functions/core-library.md#cd-0x14b7f8-0x14bea8
 * @end-sly2-annot */
int CpchzTokenizePath(char *pchz)
{
    strupr(pchz);
    int cch = strlen(pchz);
    if (cch != 0 && ((unsigned char *)pchz)[cch - 1] != '\\' && ((unsigned char *)pchz)[cch - 1] != '/') {
        strcat(pchz, ";1");
        cch = strlen(pchz);
    }
    unsigned char *pchEnd = (unsigned char *)pchz + cch;
    unsigned char *pchOut = (unsigned char *)pchz;
    unsigned char *pch = (unsigned char *)pchz;
    int cpchz = 0;
    int cchCur = 0;
    int cchMax = 8;
    for (; pch < pchEnd; pch++) {
        switch (*pch) {
        case '/':
        case '\\':
            if (cchCur == 0)
                break;
            *pchOut++ = 0;
            cchCur = 0;
            cchMax = 8;
            cpchz++;
            break;
        case '.':
            if (cchCur == 0)
                break;
            if (cchMax != 8)
                break;
            *pchOut++ = *pch;
            cchCur = 0;
            cchMax = 3;
            break;
        case ';':
            if (cchCur != 0 && pch == pchEnd - 2 && pch[1] == '1') {
                *pchOut++ = *pch++;
                *pchOut++ = *pch;
                cchCur += 2;
                break;
            }
        default:
            if (cchCur < cchMax) {
                __asm__ __volatile__("" : : : "memory");
                unsigned char ch = *pch;
                if ((unsigned char)(ch - 'A') < 26 || (unsigned char)(ch - '0') < 10)
                    *pchOut = ch;
                else
                    *pchOut = '_';
                pchOut++;
                cchCur++;
            }
            break;
        }
    }
    if (cchCur != 0) {
        *pchOut = 0;
        cpchz++;
    }
    return cpchz;
}

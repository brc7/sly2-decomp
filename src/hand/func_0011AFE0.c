extern unsigned char D_002A4150[];
extern unsigned char D_002AA6A1[];

#define toupper(c) ({ int __x = (c); (D_002AA6A1[__x] & 2) ? __x - 'a' + 'A' : __x; })

/** @sly2-annot addr=0x11AFE0
 * Purpose: Case-insensitive string hash h = h*33 + toupper(c) (NULL -> default string D_002A4150)
 * Info: proposed name HashPchzCaseless (T1); TU tu_11AFE0
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r1_116918+verdicts_tu_r1r2
 * See: docs/engine/core-library.md#6-source-files, docs/engine/core-library.md#7-key-functions,
 *   docs/engine/core-library.md#open-questions
 * @end-sly2-annot */
int func_0011AFE0(unsigned char *pchz)
{
    int n;

    if (!pchz) {
        pchz = D_002A4150;
    }
    n = 0;
    while (*pchz) {
        n = n * 33 + toupper(*pchz++);
    }
    return n;
}

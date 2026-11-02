typedef struct { char a[0x40]; } ENT;
typedef struct { char a[0x5C]; } OUT;
extern char D_002E6500[];
extern ENT D_002E6080[];
unsigned int func_0021ED28(const char *);
char *func_0021ED58(char *, const char *);
void *func_002333B4(void *, int, unsigned int);
int func_002446F0(int, int, char *, int, int, ENT *);
int func_001AB098(void *, int, int, int *);
void func_001AA9B8(ENT *, OUT *);

/** @sly2-annot addr=0x1ABE78
 * Purpose: sceMcGetDir into table, then FillMcDirInfo
 * Info: proposed name ReadMcDir (EXISTING); TU memcard
 * Notes: agent/verifier proposes ReadMcDir for this existing name (see existing_name_changes.csv) |
 *   arbitration: draft replacement (not re-derived) | RENAME proposed -> ReadMcDir
 *   (existing_name_changes.csv)
 * Confidence: high [EXISTING 0.50] - evidence: arbitration; pre-existing symbol in symbol_addrs.txt; sources:
 *   r4_17F090+arbitration
 * See: docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
int FUN_0018c888(int *p, char *path, int c, OUT *aout, int *pc)
{
    int f;
    int r;
    int i;
    if (c > 0x12)
        return 0x25;
    if (path) {
        if (func_0021ED28(path) > 0x40)
            return 0xD;
        func_0021ED58(D_002E6500, path);
        f = 0;
    } else {
        f = 1;
        path = D_002E6500;
    }
    func_002333B4(D_002E6080, 0, 0x480);
    r = func_001AB098((char *)p + 0xFD5C, func_002446F0(p[0], p[1], path, f, c, D_002E6080), 0xD, pc);
    if (r)
        return r;
    for (i = 0; i < *pc; i++)
        func_001AA9B8(&D_002E6080[i], &aout[i]);
    return 0;
}

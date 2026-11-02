typedef struct { char ab[0x5408]; } BIG;
typedef struct {
    short ch;
    signed char x2;
    unsigned char x3;
    unsigned short x4;
    unsigned short iChild;
    unsigned short iSibling;
} ENT;
typedef struct { int c; ENT *ae; char ht[1]; } OBJ;
extern BIG D_003D4A60;
extern char D_002A78F8[];
int func_001D8720(void *, char *);
static inline int NFind(OBJ *ph, char *pch) { return func_001D8720(ph->ht, pch); }
/** @sly2-annot addr=0x1D84B0
 * Purpose: Copy D_003D4A60 into a big buffer then set flag byte at ib for every schema entry whose symbol is
 *   'from_memcard'
 * Info: proposed name BuildMemcardGsMask (T2); TU save
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r6_1D5150
 * See: docs/engine/save-memcard.md#2-memory-card-memcardcpp-0x1aa978-0x1ae130,
 *   docs/reference/functions/save-memcard.md#save-0x1d7b88-0x1d93d8
 * @end-sly2-annot */
void func_001D84B0(OBJ *p, BIG *pbig)
{
    int n;
    int i;
    *pbig = D_003D4A60;
    n = NFind(p, D_002A78F8);
    if (n == -1)
        return;
    for (i = 0; i < p->c; i++) {
        ENT *pe = &p->ae[i];
        if (pe->ch == n)
            pbig->ab[pe->x4] = 1;
    }
}

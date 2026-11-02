// Append the clue property and identifier to the selected key array.
struct K { short a, b; };
struct A { int c; short *pk; };
struct HT {void *buckets[0x35];int count;void *entries;};
struct X {char pad[8];struct HT table;};
extern struct X D_003E0EA8;
extern const char D_002A7768[],D_002A7788[];
int IlevelCur__Fv(void);
int func_0021ECD8(char *buf,const char *fmt,...);
int func_001D8720(struct HT *pt,const char *s);
struct A *func_002109B8(int h);
struct A *func_001D8CC8(struct A *p, struct A *pa);
static inline int Find(struct X *px, const char *s) { return func_001D8720(&px->table,s); }
/** @sly2-annot addr=0x1C7B18
 * Purpose: query save data for bit n of 'afClue' for the current level
 * Info: proposed name FClueCollected (T1); TU prize
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r5_1AE130
 * See: docs/reference/functions/game-progression.md#prize-0x1c4e50-0x1c8cc0
 * @end-sly2-annot */
struct A *func_001C7B18(int input)
{
    register int n __asm__("$17")=input;
    int h;
    struct A a;
    struct K k;
    char buf[0x10];
    h = IlevelCur__Fv();
    if (h == -1) return 0;
    func_0021ECD8(buf, D_002A7768, n);
    k.a = Find(&D_003E0EA8, D_002A7788);
    k.b = n;
    a.c = 2;
    a.pk = (short *)&k;
    return func_001D8CC8(func_002109B8(h), &a);
}

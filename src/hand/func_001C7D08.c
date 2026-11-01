struct K { int a; short b; short c; short d; short e; int f; };
struct S { int k; struct K *pk; };
struct X { short a; signed char b; char c; short d; unsigned short h; };
struct T { struct X *px; int n; };
struct E { unsigned short id; short pad[3]; unsigned short next; };
struct M { int a; struct E *pe; char m8[8]; };
struct G { char pad[0x3A18]; struct M m; };
extern struct G D_003DD490;
extern char D_003D4A60[];
extern char D_002A7788[];
int func_00210A88(void);
struct { int a; int b; } *func_002109B8(int n);
void func_001AE5A0(struct K *pk, int a, int b);
void *func_001D7DA0(void *a, void *b, struct T *pt);
short func_001D8720(void *a, char *s);
unsigned int *func_001D7E80(void *a, void *b, void *c);
/** @sly2-annot addr=0x1C7D08
 * Purpose: count collected clue bottles: popcount of afClue words over the level's entries
 * Info: proposed name CClueCollected (T1); TU prize
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r5_1AE130
 * See: docs/reference/functions/game-progression.md#prize-0x1c4e50-0x1c8cc0
 * @end-sly2-annot */
int func_001C7D08(void)
{
    int n;
    int c;
    struct K k;
    struct S s;
    struct T t;
    unsigned int i;
    struct E *pe;
    unsigned int v;
    n = func_00210A88();
    if (n == -1)
        return 0;
    c = 0;
    func_001AE5A0(&k, func_002109B8(n)->b, 6);
    s.pk = &k;
    s.k = 3;
    func_001D7DA0(&D_003DD490.m, &s, &t);
    s.k = 5;
    k.d = func_001D8720(D_003DD490.m.m8, D_002A7788);
    for (i = t.px->h; i; i = pe->next) {
        pe = &D_003DD490.m.pe[i];
        k.c = pe->id;
        v = *func_001D7E80(&D_003DD490.m, &s, D_003D4A60);
        while (v) { v &= v - 1; c++; }
    }
    return c;
}

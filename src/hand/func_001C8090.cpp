struct X { int c; short *ps; };
struct B { int a, b; int c; short Find(char *s) ; };
extern B D_003E0EA8;
extern "C" {
int IlevelCur__Fv(void);
void *func_002109B8(int n);
int func_001D8CC8(void *p, X *px);
int func_001C8090(void);
short func_001D8720(int *p, char *s);
}
inline short B::Find(char *s) { return func_001D8720(&c, s); }
/** @sly2-annot addr=0x1C8090
 * Purpose: query save-data flag 'fVault' for the level
 * Info: proposed name FVaultOpened (T1); TU prize
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r5_1AE130
 * See: docs/reference/functions/game-progression.md#prize-0x1c4e50-0x1c8cc0
 * @end-sly2-annot */
int func_001C8090(void)
{
    int n = IlevelCur__Fv();
    X x;
    short s;
    if (n == -1) return 0;
    s = D_003E0EA8.Find("fVault");
    x.c = 1;
    x.ps = &s;
    return func_001D8CC8(func_002109B8(n), &x);
}

struct P { int c; short *as; };
struct S { char pad[0x180]; unsigned short f180; };
extern "C" {
extern char D_003E0EA8[];
extern char D_002A4BB8[];
extern char D_002A4BC0[];
int func_00210A88(void);
short func_001D8720(void *, char *);
void *func_002109B8(int);
void func_001D8CC8(void *, struct P *);
void func_0013D1A8(struct S *p);
}
/** @sly2-annot addr=0x13D1A8
 * Purpose: Build the save key for this container (level entry + two symbols + id)
 * Info: proposed name PkeyBootyContainerSave (T2); TU booty
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/game-progression.md#booty-0x13bdf0-0x13d558
 * @end-sly2-annot */
void func_0013D1A8(struct S *p) {
    short as[3];
    struct P par;
    int i;
    int v;
    i = 0;
    v = func_00210A88();
    if (v == -1) v = i;
    i = v;
    char *pv = D_003E0EA8;
    pv += 8;
    as[0] = func_001D8720(pv, D_002A4BB8);
    short r = func_001D8720(pv, D_002A4BC0);
    register int t __asm__("$3") = p->f180;
    register int c __asm__("$5") = 3;
    par.c = c;
    __asm__("" : : : "memory");
    as[1] = r;
    as[2] = t;
    par.as = as;
    func_001D8CC8(func_002109B8(i), &par);
}

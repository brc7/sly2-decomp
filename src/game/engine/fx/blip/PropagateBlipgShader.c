/* MATCH (Claude agent, laneC16 2026-10-23), from v6 (3): the c-copy empty asm made volatile. A non-volatile asm stayed between the li a0,1 and the branch, which stops reorg backward search; volatile it is a sched barrier so the li follows it. One pin (px) + two commented empty asms. */
/* v6 (Claude agent, laneC15 2026-10-23): aligned 4 -> 3. ONE pin (px in $9) plus two commented empty asms: `cExtra = 0; asm("" : "+r"(cExtra));` early (gives the target slt/subu/movn) and `c = v; asm("" : "+r"(c));` (gives lw v0 + daddu t0 without a pin on c). Left: li a0,1 (n = 1) scheduled before daddu t0, so reorg fills the loop-guard slot with slt (beql) instead of the li (beqz). n=1 position / for-init: same. */
/* v5 (Claude agent, laneC11 2026-10-23): aligned 5 -> 4. cExtra = 0 moved before c = v, plus a 2nd pin cExtra in $10 (t2). Left: with cExtra pinned, if-conversion gives bnel+subu instead of slt/subu/movn (v1/v0 swap on the slt). cExtra unpinned with the early init: 13; px pinned to $9 too: 7. */
/* v4 (Claude agent, laneC9 2026-10-23): aligned 6 -> 5 with ONE register pin (c in $8) plus v = px->c; c = v; v >>= 1 (gives the target's lw v0 + daddu t0,v0). Left: cExtra = 0 (t2) is emitted after the loops; target sets it right after the load (then the loop-head alignment nops line up). cExtra = 0 right after c = v: 13 (px/cExtra swap t1/t2); declaration order: 13. */
typedef int qword __attribute__((mode(TI)));
typedef union { qword q; struct { long l; int n; int pad; } s; } GE;
struct X { int x0; int x4; int x8; unsigned char ab[4]; int c; long *al; };
struct P { char pad[0x1A0]; int n1A0; int pad1[3]; GE age[0x20]; int x3B0; unsigned char ab[4]; char pad2[0x438 - 0x3B8]; void *p438; };
extern char D_002DD600[];
extern char D_002DD5C0[];
struct X *func_001EC8D0(void *pv);
static __inline__ int NPow2(int c)
{
    int n = 1;
    int v;
    for (v = c >> 1; v; v >>= 1)
        n <<= 1;
    n <<= (n < c);
    return n;
}
/** @sly2-annot addr=0x139370
 * Purpose: Choose blend GIF from the shader flag, cache colour, compute power-of-two texture dims and build
 *   the texture GIF regs
 * Info: proposed name PropagateBlipgShader (T1); TU blip
 * Confidence: mid (draft) [T1 0.65] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/particles-effects.md#blip-0x138f08-0x13a9e8
 * @end-sly2-annot */
void PropagateBlipgShader(struct P *p, void *pv)
{
    register struct X *px asm("$9") = func_001EC8D0(pv); /* register pin (Claude agent): px in t1 */
    int n, v, i, j;
    int cExtra;
    int c;
    GE *pge;
    p->p438 = (px->x8 & 1) ? D_002DD600 : D_002DD5C0;
    if (p->x3B0 == 0) {
        p->x3B0 = 1;
        for (j = 0; j < 4; j++)
            p->ab[j] = (px->ab[j] + 1) >> 1;
    }
    v = px->c;
    cExtra = 0;
    __asm__("" : "+r"(cExtra)); /* empty asm (Claude agent): emulates the original's early cExtra = 0 */
    c = v;
    __asm__ __volatile__("" : "+r"(c)); /* empty volatile asm (Claude agent): emulates the original c copy; volatile so n = 1 is scheduled after it and reorg can take the li into the beqz slot */
    n = 1;
    for (v >>= 1; v; v >>= 1)
        n <<= 1;
    n <<= (n < c);
    while (n >= 32)
        n >>= 1;
    if (c < n)
        cExtra = n - c;
    p->n1A0 = n;
    pge = p->age;
    for (i = 0; i < c; i++) {
        pge->s.l = px->al[i];
        pge->s.n = 6;
        if (i < cExtra) {
            pge[1].q = pge[0].q;
            pge++;
        }
        pge++;
    }
}

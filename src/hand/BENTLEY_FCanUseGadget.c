/* MATCH (Claude agent laneB13, 2026-10-23), from v4 (4). One register pin (f in v1) plus commented empty asms that emulate the original toolchain's reorg/cross-jump state: case 12 has its own `return 0` (with an empty asm so it isn't merged with the shared fail), the shared fail has one too, the 1CADE0 path ends in `return 0`, and an empty asm using pe in the pv != 0 block keeps v0 live so reorg annuls the beql slot. */
/* v4 (Claude agent laneB12, 2026-10-23): aligned 10 -> 4. f pinned to v1 (one commented pin) and used for case 12's x15C and the func_001CADE0 result; cases 2/3/13 test their call results directly; an empty volatile asm "+r"(f) after the 1CADE0 call keeps the target's `daddu v1,v0; bnez v1` (cse otherwise tests v0). Left: reorg fill choices: case 12 beq slot (target steals li v0,1 from `one`, ours takes li v0,0 from the fallthrough; `if (f != k) goto fail` flips the layout: 4), and the pv == 0 test (target beql, ours beqz). */
/* v3 (Claude agent, 2026-10-17): aligned 10. Shared function-scope `void *pv` for case 12 pa11C0 and pe->p4 puts pa in a0 like the target. Left: target loads x15C into v1 and copies the 1CADE0 result to v1 (daddu v1,v0; bnez v1). */
struct A { char pad[0x15C]; int x15C; };
struct E { int x0; void *p4; void *p8; };
struct P { char pad[0x2C0]; char a2C0[0x200]; char a4C0[0xF50 - 0x4C0]; char aF50[0x11B4 - 0xF50]; int x11B4; int pad1[2];
    struct A *pa11C0; char pad2[0x12F0 - 0x11C4]; int x12F0; char pad3[0x13D0 - 0x12F4]; int x13D0; char pad4[0x1818 - 0x13D4];
    long x1818; };
extern int D_002DFBC0;
extern char D_002DE2E0[];
int FStepplayerInJtmode();
int func_0019C8D0(void *pv, int k);
int func_00194558(void *pv, int k);
int func_0019EEF0(void *pv, int k);
struct E *func_00136A28(struct P *p, int k);
int func_00224590(void *pv);
int func_001CADE0(void *pv, int n);
int func_0014FD50(void *pv);
int BENTLEY_FCanUseGadget(struct P *p, int k)
{
    register int f __asm__("$3");  /* register pin: target keeps x15C and the func_001CADE0 result in v1 */
    void *pv;
    if (p->x1818 != 0)
        goto fail;
    if (FStepplayerInJtmode())
        return 0;
    switch (p->x13D0) {
    case 2:
        if (func_0019C8D0(p->a2C0, k))
            goto inner;
        goto fail;
    case 3:
        if (func_00194558(p->a4C0, k))
            goto inner;
        goto fail;
    case 13:
        if (func_0019EEF0(p->aF50, k))
            goto inner;
    default:
    fail:
        __asm__ __volatile__("");  /* keeps the shared return 0 apart from case 12's */
        return 0;
    case 12:
        if ((unsigned int)(p->x11B4 - 4) < 3) {
            switch (k) {
            case 4: case 7: case 9: case 11: case 13:
                pv = p->pa11C0;
                f = ((struct A *)pv)->x15C;
                if (f == k)
                    goto one;
                __asm__ __volatile__("");  /* keeps case 12's return 0 from being merged with the shared one */
                return 0;
            }
        }
        goto fail;
    case 0: case 1: case 5: case 7:
    inner:
        switch (k) {
        case 4: case 5: case 7: case 9: case 11: case 13: {
            struct E *pe = func_00136A28(p, k);
            if (pe->p8) {
                if (func_00224590(pe->p8))
                    return 1;
                goto fail;
            }
            if ((pv = pe->p4)) {
                __asm__ __volatile__("" : : "r"(pe));  /* keeps pe (v0) live into this block, so reorg annuls the beql slot */
                f = func_001CADE0(pv, 2);
                __asm__ __volatile__("" : "+r"(f));  /* keeps the copy in v1 that the target tests */
                if (f)
                    goto one;
            }
            return 0;
        }
        case 3: {
            int r = 0;
            if (p->x12F0 && (D_002DFBC0 & 2))
                r = func_0014FD50(D_002DE2E0) == 0;
            return r;
        }
        case 1: case 2: case 8: case 10: case 12: case 14:
        one:
            return 1;
        }
        goto fail;
    }
}

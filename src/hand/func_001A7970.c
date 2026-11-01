/* Run the 19 initialization stages with CProg color and remaining-count updates. */
struct B { char data[0x70]; };
struct C { int c; } __attribute__((aligned(16)));
extern char D_002A6A58[], D_002C46D8[], D_002C46C0[], D_002DDED0[];
extern void (*D_002BFCE0[])(void);
void func_001700D0(void);
struct B *__5CProgP4RGBAN31(struct B *b, struct C *c0, struct C *c1, struct C *c2, struct C *c3);
int kprintf(char *format, ...);
void SetPhase__F5PHASE(int n);
void Begin__5CProg(struct B *b);
void func_0014D0B8(void *p, int n);
void SetRemain__5CProgi(struct B *b, int n);
void func_001C8DB8(struct B *b);
void ClearPhase(int n);
/** @sly2-annot addr=0x1A7970
 * Purpose: Run the 19-entry startup table D_002BFCE0 with a CProg progress bar (shown only after StartupGs),
 *   MarkClockTick(g_clock,1) per stage; prints build "0813.0032"/"america"
 * Info: proposed name Startup (T1); TU main
 * Confidence: high [T1 0.95] - evidence: static analysis; sources: r4_17F090+verdicts_A2+verify_side
 * See: docs/engine/main-loop-startup.md#1-maincpp-0x1a7518-0x1a7c18,
 *   docs/engine/main-loop-startup.md#8-key-functions, docs/engine/overview.md#1-boot
 * @end-sly2-annot */
void func_001A7970(void)
{
    struct B b;
    struct C c0, c1, c2, c3;
    unsigned int i;
    int f;
    {
    register unsigned int a2 __asm__("$5")=0x80003F3F;
    register unsigned int a3 __asm__("$6")=0x8000003F;
    register unsigned int a0 __asm__("$3")=0x807F0000;
    register unsigned int a1 __asm__("$2")=0x803F3F3F;
    c2.c=a2;
    c3.c=a3;
    c0.c=a0;
    c1.c=a1;
    }
    __5CProgP4RGBAN31(&b, &c0, &c1, &c2, &c3);
    kprintf(D_002A6A58, D_002C46D8, D_002C46C0, 0x2B0);
    SetPhase__F5PHASE(1);
    Begin__5CProg(&b);
    f = 0;
    for (i = 0; i < 19; i++) {
        func_0014D0B8(D_002DDED0, 1);
        if (f)
            SetRemain__5CProgi(&b, 19 - i);
        else if (D_002BFCE0[i] == func_001700D0)
            f = 1;
        D_002BFCE0[i]();
    }
    SetRemain__5CProgi(&b, 0);
    func_001C8DB8(&b);
    ClearPhase(1);
}

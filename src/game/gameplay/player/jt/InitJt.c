struct LM4 { float g0; int n; float g8; float gc; };
struct S {
    char pad[0x264]; int x264;
    char p0[0xD98]; int x1000; char p1[0x1C]; float x1020;
    char p2[0x74]; float x1098; float x109c; float x10a0; float x10a4; struct LM4 lm;
    char p3[0x268]; char x1320[0x4B0]; char x17d0[0xF8]; int x18c8; int p4; int x18d0; int p5; int x18d8;
    char p6[0xD0]; float x19ac; char p7[0x280]; char x1c30[0x380]; int x1fb0; int p8[2]; int x1fbc; int p9[2]; int x1fc8; int p10[2]; int x1fd4; int p11[6]; char x1ff0[4];
};
extern struct S *D_002E1E40;
void InitStepplayer(struct S *p);
void InitGait(void *pv, struct S *p, int n);
void func_00204D48(struct S *p, float g);
void func_001371B8(void *pv, struct S *p);
void func_001CC158(void *pv);
static __inline__ void InitLm(struct LM4 *plm, float g8, float gc) { plm->g8 = g8; plm->gc = gc; plm->n = 0; }
/** @sly2-annot addr=0x188488
 * Purpose: JT vt7 Init: base InitStepPlayer 0x13FF70, defaults (x19AC=15, jts/jtbs fields -1, limits
 *   -100/100/50/-10/250, LM 1..4500), gadget records x1FB0..x1FD4 = ids 0x1A/0x25/0x26/0x29, sets g_pjt
 *   D_002E1E40=pjt
 * Info: proposed name InitJt (T1); class JT slot7; TU jt
 * Notes: names_v2 v1 proposed Chetkido__Fv (0.002) | runtime: g_pjt D_002E1E40 = 0xFB1580 (only JT); x19AC
 *   stored as float 15.0
 * Confidence: high [T1 0.90] - evidence: runtime (save-state RAM); static naming pass; sources:
 *   r4_17F090+verdicts_tu_r4r6+runtime
 * See: docs/engine/main-loop-startup.md#7-source-files, docs/engine/player.md#10-source-files,
 *   docs/engine/player.md#11-key-functions
 * @end-sly2-annot */
void InitJt(struct S *p)
{
    InitStepplayer(p);
    p->x19ac = 15.0f;
    p->x18c8 = -1;
    p->x18d8 = -1;
    p->x18d0 = -1;
    InitGait(p->x1320, p, 0x6F);
    InitGait(p->x1c30, p, 0x6F);
    {
    register float a __asm__("$f5")=-100.0f;
    register float b __asm__("$f1")=100.0f;
    register float c __asm__("$f2")=50.0f;
    register float d __asm__("$f3")=-10.0f;
    register float e __asm__("$f4")=250.0f;
    register float f __asm__("$f6")=1.0f;
    register float g __asm__("$f0")=4500.0f;
    p->x1098=a; p->x109c=b; p->x10a0=c; p->x10a4=d; p->lm.g0=e; InitLm(&p->lm,f,g);
    }
    func_00204D48(p, 1.6f);
    p->x1fb0 = 0x1A;
    p->x1fbc = 0x25;
    p->x1fc8 = 0x26;
    p->x1fd4 = 0x29;
    func_001371B8(p->x1ff0, p);
    p->x1000 = 1;
    p->x1020 = 200.0f;
    p->x264 = 0xC7;
    func_001CC158(p->x17d0);
    D_002E1E40 = p;
}

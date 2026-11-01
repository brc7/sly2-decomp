struct S { char pad[0x4CC]; int x4cc; char p1[0xA80]; char xf50[4]; int xf54; char p2[0x970]; int n; char p3[0x1D8]; float t; };
struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
void func_00141640(struct S *p, int *pf);
void func_0019EA00(void *p, int *pf);
/** @sly2-annot addr=0x18A428
 * Purpose: JT vt50 (p, int *pgrf): stamp x? with g_clock.t while moving, base 0x141640, jts 8 sub, jts 3/5/6
 *   -> *pgrf|=1
 * Info: proposed name JT_vt50 (T2); class JT slot50; TU jt
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/player.md#jt-0x188488-0x18e540
 * @end-sly2-annot */
void func_0018A428(struct S *p, int *pf)
{
    if (p->n != 0 && p->n != 0x12 && p->x4cc && p->xf54 != 1)
        p->t = D_002DDED0.t;
    func_00141640(p, pf);
    switch (p->n) {
    case 8:
        func_0019EA00(p->xf50, pf);
        break;
    case 3: case 5: case 6:
        *pf |= 1;
        break;
    }
}

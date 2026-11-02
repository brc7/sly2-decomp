struct V2 { float x, y; };
struct S { char pad[0x19C]; float a, b, c, d; struct V2 e; float pad2[2]; float f; int pad3[3]; int g; void *h; int i; };
extern struct V2 D_002C4400;
void InitPo(struct S *p);
/** @sly2-annot addr=0x2259D8
 * Purpose: InitPo; lmPan=(-1.5,1.5), lmTilt=(-0.7,0.7), defaults, self pointer
 * Info: proposed name InitTurret (T1); class TURRET slot7; TU turret
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r8_21BB88
 * See: docs/engine/gameplay-objects.md#gameplay-object-classes, docs/engine/gameplay-objects.md#source-files,
 *   docs/engine/minigames.md#4-mounted-turret-turret-0x45-turret-tu-0x2259d8-0x226550-conf-high-ccode
 * @end-sly2-annot */
void InitTurret(struct S *p)
{
    InitPo(p);
    p->e = D_002C4400;
    p->g = 1; /*P*/
    p->f = p->e.y; /*P*/
    p->c = -1.5f; /*P*/
    p->d = 1.5f; /*P*/
    p->a = -0.7f; /*P*/
    p->b = 0.7f; /*P*/
    p->i = 1; /*P*/
    p->h = p; /*P*/
}

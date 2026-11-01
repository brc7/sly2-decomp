struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
struct VTE { short d; short pad; void (*pfn)(void *); };
struct O { char pad[0xC]; struct VTE *vt; char pad2[0x2C]; int n3C; };
struct S { char pad[0x10]; char *p10; int pad2; void *p18; char pad3[0x14]; float g30; float g34; int n38; float g3C; void *p40; };
void PopBhv(void *p);
struct O *func_00205438(void *p, int n);
void func_00171C18(void *p, void *q);
void func_00171258(struct O *o, void *t, int a, void *u);
void ArmBhvpatrolPauseTimer(struct S *p);
void ArmBhvpatrolWalkTimer(struct S *p);
/** @sly2-annot addr=0x179D30
 * Purpose: BHVPATROL state switch: 0 push BHV169 script runner on the route (saves resume point), 1/2 push
 *   BHVIDLE, 3 push BHVIDLE flag 1
 * Info: proposed name SetBhvpatrolState (T1); TU guardbhv
 * Notes: runtime2/4 confirmed the BHVPATROL role on live guards (S02-S41)
 * Confidence: mid (draft) [T1 0.75] - evidence: runtime (save-state RAM); sources:
 *   r9_bhv_screen+runtime(class role)
 * See: docs/engine/guards-ai.md#9-key-functions,
 *   docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void SetBhvpatrolState(struct S *p, int n) {
    if (n == p->n38) return;
    if (p->n38 == 0) func_00171C18(p->p18, p->p10 + 0x2DC);
    PopBhv(&p->p18);
    p->n38 = n;
    p->g3C = D_002DDED0.t;
    switch (n) {
    case 1: case 2:
        { struct O *o = func_00205438(p->p10, 0x9E); struct VTE *e = &o->vt[8]; e->pfn((char *)o + e->d); }
        ArmBhvpatrolPauseTimer(p);
        p->g30 = 3.40282346638528859812e+38f;
        break;
    case 0:
        { struct O *o = func_00205438(p->p10, 0xA9); struct VTE *e; func_00171258(o, p->p40, 0, p->p10 + 0x2DC); e = &o->vt[8]; e->pfn((char *)o + e->d); }
        ArmBhvpatrolWalkTimer(p);
        p->g34 = 3.40282346638528859812e+38f;
        break;
    case 3:
        { struct O *o = func_00205438(p->p10, 0x9E); struct VTE *e; o->n3C = 1; e = &o->vt[8]; e->pfn((char *)o + e->d); }
        break;
    }
}

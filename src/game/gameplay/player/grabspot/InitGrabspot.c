struct VTE { short d; short pad; void *(*pfn)(void *); };
struct S { char pad[8]; float g; char p1[8]; int x14; int p2; int x1c; struct VTE *vt; };
extern float D_002A5680;
void SetLoBusyk(void *p, int n);
/** @sly2-annot addr=0x182C70
 * Purpose: Init the GRABSPOT mix-in sub-object: x14=-1, sBusy-like g=D_002A5680, player mask 7, owner (vt6)
 *   busyk 3
 * Info: proposed name InitGrabspot (T1); TU grabspot
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r4b_rest
 * See: docs/engine/gameplay-objects.md#source-files, docs/engine/player.md#10-source-files,
 *   docs/reference/functions/gameplay-objects.md#tu_1ff3b8-0x1ff3b8-0x1ff690
 * @end-sly2-annot */
void InitGrabspot(struct S *p)
{
    struct VTE *e;
    p->x14 = -1;
    p->g = D_002A5680;
    p->x1c = 7;
    e = &p->vt[6];
    SetLoBusyk(e->pfn((char *)p + e->d), 3);
}

struct AREQ { int cb; int cbAlign; };
struct E { char pad[0x10]; struct E *p10; struct E *p14; int x18; int x1C; };
struct P { char pad[0x4558]; int c; struct E *ae; int n; void *p4564; };
int U16Read__18CBinaryInputStream(void *pbs);
void ReadVector4__18CBinaryInputStreamP7VECTOR4(void *pbs, struct E *pe);
int S16Read__18CBinaryInputStream(void *pbs);
void *func_001AE2A0(struct AREQ *preq);
/** @sly2-annot addr=0x13E0E0
 * Purpose: Reads cell BSP: U16 c x {VECTOR4 plane; S16 front; S16 back}; negative child = new leaf index;
 *   allocs SW.acell (+0x455C, 0x20 each) and SW.aleaf (cleaf x 0x20)
 * Info: proposed name LoadSwCellsFromBrx (T1); TU brx_sw
 * Confidence: mid (draft) [T1 0.75] - evidence: semantic review; sources: r2_131F70
 * See: docs/engine/world-sw.md#loadswfrombrx-0x13da18-matched-hand-file,
 *   docs/engine/world-sw.md#10-key-functions, docs/reference/functions/world-sw.md#brx_sw-0x13da18-0x13e368
 * @end-sly2-annot */
void LoadSwCellsFromBrx__FP2SWP18CBinaryInputStream(struct P *p, void *pbs)
{
    struct AREQ req;
    int i;
    int c = U16Read__18CBinaryInputStream(pbs);
    req.cbAlign = 0x10;
    req.cb = c * sizeof(struct E);
    p->c = c;
    p->ae = (struct E *)func_001AE2A0(&req);
    p->n = 0;
    for (i = 0; i < p->c; i++) {
        struct E *pe = &p->ae[i];
        int j;
        ReadVector4__18CBinaryInputStreamP7VECTOR4(pbs, pe);
        j = S16Read__18CBinaryInputStream(pbs);
        if (j >= 0)
            pe->p10 = &p->ae[j];
        else
            pe->x18 = p->n++;
        j = S16Read__18CBinaryInputStream(pbs);
        if (j >= 0)
            pe->p14 = &p->ae[j];
        else
            pe->x1C = p->n++;
    }
    req.cbAlign = 0x10;
    req.cb = p->n * sizeof(struct E);
    p->p4564 = func_001AE2A0(&req);
}

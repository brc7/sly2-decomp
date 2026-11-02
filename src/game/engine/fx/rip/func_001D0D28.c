typedef int qword __attribute__((mode(TI)));
typedef struct VT { struct { short d; short pad; void (*pfn)(void *, float, qword, int); } a[4]; } VT;
typedef struct {
    char pad[0x18];
    float x18;
    int x1C;
    float x20;
    float x24;
    char pad28[0xEC - 0x28];
    VT *vt;
} EMIT;
typedef struct { char pad[0xE0]; int c; char *a; } Q;
typedef struct {
    char pad[0x70];
    qword x70;
    qword x80;
    char pad90[0xE0 - 0x90];
    Q *q;
    char pade4[0xF4 - 0xE4];
    int xF4;
} OBJ;
extern struct G D_003DD490;
typedef struct { qword pos; char rest[0x50]; } HIT;
int ClsgClipEdgeToObject(void *, qword, qword, int, HIT *);
EMIT *PripNewRipg(struct G *, int, int);
void func_001CEA88(OBJ *, int);
/** @sly2-annot addr=0x1D0D28
 * Purpose: RIPT 1 Update: periodically spawns child rips (PripNewRipg) and touches/ends itself
 * Info: proposed name UpdateRipt1 (T2); TU rip
 * Notes: names_v2 v1 proposed UpdateRose__FP4ROSEf (0.638)
 * Confidence: low (draft) [T2 0.30] - evidence: static naming pass; sources: r5_1AE130
 * See: docs/reference/functions/particles-effects.md#rip-0x1cdcd0-0x1d1370
 * @end-sly2-annot */
void func_001D0D28(OBJ *p, int n)
{
    HIT hit;
    int i;
    EMIT *pe;
    if (n == 0 || p->xF4 == 0)
        goto done;
    hit.pos = p->x70;
    for (i = 0; i < p->q->c; i++) {
        if (ClsgClipEdgeToObject(p->q->a + i * 0xB0, p->x80, p->x70, 1, &hit))
            goto found;
    }
found:
    pe = PripNewRipg(&D_003DD490, 7, 0);
    if (pe) {
        pe->vt->a[1].pfn((char *)pe + pe->vt->a[1].d, 40.0f, hit.pos, 0);
        pe->x18 = 0.75f;
        pe->x20 = 40.0f;
        pe->x24 = 30.0f;
        pe->x1C = p->xF4;
    }
done:
    func_001CEA88(p, n);
}

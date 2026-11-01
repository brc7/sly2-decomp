typedef struct { char pad[0x18]; int x18; } Q;
typedef struct { int pad; int x4; } R;
typedef struct { void *o; Q *q; void *r; int xC; } ENT;
typedef struct { char pad[0x1A0]; ENT a[2]; } OBJ;
extern struct G D_003DD490;
void func_0015E9C8(struct G *, int, int, void *, int, Q **);
void *func_001F6788(void *, int);
void *func_00130488(Q *, void *, int, float, float);
R *func_0012FF10(Q *, void *, int, int);
/** @sly2-annot addr=0x1DEDB0
 * Purpose: For each of two animation slots find an ASEG (class 0x6D) on the object and its SMA/aseg key
 * Info: proposed name BindLasenAnims (T2); TU sensor
 * Notes: names_v2 C proposed RebuildLightFrustrum__FP5LIGHT (0.167)
 * Confidence: low (draft) [T2 0.35] - evidence: static naming pass; sources: r6b_rest
 * See: docs/reference/functions/gameplay-objects.md#sensor-0x1de2a0-0x1dfad8
 * @end-sly2-annot */
void func_001DEDB0(OBJ *p)
{
    unsigned int i;
    for (i = 0; i < 2; i++) {
        ENT *e = &p->a[i];
        void *o = e->o;
        Q *q;
        R *r;
        if (o == 0)
            continue;
        func_0015E9C8(&D_003DD490, 2, 0x6D, o, 1, &e->q);
        q = e->q;
        if (q == 0)
            continue;
        e->r = func_001F6788(o, q->x18);
        if (e->r == 0) {
            e->r = func_00130488(q, o, 8, 0.0f, 1.0f);
            if (e->r == 0)
                continue;
        }
        r = func_0012FF10(q, o, 0xC, 1);
        if (r)
            e->xC = r->x4;
    }
}

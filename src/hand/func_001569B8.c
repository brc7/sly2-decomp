struct VTE { short d; short pad; void (*pfn)(); };
struct O { int a[3]; struct VTE *vt; char pad[0x40]; int n50; float g54; int pad58[2]; float g60; int n64; };
struct S { char pad[0x4020]; struct O *ao[7]; };
extern float D_002BDD00, D_002BDD04, D_002BDD08, D_002BDD0C, D_002BDD10, D_002BDD14, D_002BDD18;
struct O *func_0013E3C8(int a, int b, int c, int d);
static inline void Init(struct O *o) { struct VTE *e = &o->vt[9]; e->pfn((char *)o + e->d); }
/** @sly2-annot addr=0x1569B8
 * Purpose: Creates the seven cid 0x75 noise templates (oids 0x2C2..0x2C8) into SW+0x4020[7]
 * Info: proposed name CreateSwNoiseTemplates (T2); TU noise
 * Notes: names_v2 C proposed FindAsegClosestPoint__FP4ASEGP3ALOP6VECTORfPfT2T2 (0.200)
 * Confidence: low (draft) [T2 0.45] - evidence: static naming pass; sources: r3b_rest
 * See: docs/reference/functions/guards-ai.md#noise-0x1564e8-0x157200
 * @end-sly2-annot */
void func_001569B8(struct S *p)
{
    {
        struct O *o = func_0013E3C8(0x75, 0, 0x2C2, -1);
        o->n50 = 0;
        p->ao[0] = o;
        Init(o);
    }
    {
        struct O *o = func_0013E3C8(0x75, 0, 0x2C3, -1);
        o->n50 = 1;
        o->g60 = D_002BDD00;
        p->ao[1] = o;
        Init(o);
    }
    {
        struct O *o = func_0013E3C8(0x75, 0, 0x2C4, -1);
        o->n50 = 0;
        o->g54 = D_002BDD04;
        p->ao[2] = o;
        Init(o);
    }
    {
        struct O *o = func_0013E3C8(0x75, 0, 0x2C5, -1);
        o->g54 = D_002BDD08;
        o->g60 = D_002BDD0C;
        o->n50 = 0;
        p->ao[3] = o;
        Init(o);
    }
    {
        struct O *o = func_0013E3C8(0x75, 0, 0x2C7, -1);
        o->n64 = 3;
        o->g60 = -1.0f;
        o->n50 = 0;
        p->ao[5] = o;
        Init(o);
    }
    {
        struct O *o = func_0013E3C8(0x75, 0, 0x2C6, -1);
        o->g54 = D_002BDD10;
        o->g60 = D_002BDD14;
        o->n50 = 0;
        p->ao[4] = o;
        Init(o);
    }
    {
        struct O *o = func_0013E3C8(0x75, 0, 0x2C8, -1);
        o->n50 = 0;
        o->g54 = D_002BDD18;
        p->ao[6] = o;
        Init(o);
    }
}

typedef int qword __attribute__((mode(TI)));
struct G { char pad[0x4028]; void *p4028; };
extern struct G D_003DD490;
extern void *D_002BE6D0[];
extern int D_002BE6E8[];
struct VTE { short d; short pad; void *pfn; };
struct O { char pad[0xC]; struct VTE *vt; };
struct C { int pad; void *p4; void *p8; int nC; };
struct A { char pad[0x33C]; int n33C; };
struct S { char pad[0x10]; struct A *p10; char pad2[0x2C]; struct C *p40; char pad3[0xC]; int n50; int pad4; int n58; };
struct M { int n0; int pad; int n8; int nC; int n10; char pad2[0x30]; int n44; };
struct X { int a; int pad[7]; qword v; qword pad2; };
int BHVANIM_FHandleMessage(struct S *p, int msg, struct M *pm);
int func_00209518(struct C *c);
int FAnimBindingDone(struct C *c);
void StopAnimBinding(struct C *c);
void StartAnimBinding(struct C *c, struct A *a, int fl, float g);
float *func_0012F938(void *p, void *q);
void func_00130D30(void *p, float a, float b, int n);
void SetBhvjuggledState(struct S *p, int n);
struct O *PloFindSwObject__FP2SWi3OIDP2LO(struct G *g, int a, int b, int c);
void *func_002333B4(void *p, int c, int n);
qword func_001A62E8(struct A *a);
void func_00156698(void *p, qword v, struct A *a);
/** @sly2-annot addr=0x17AF30
 * Purpose: BHVHELD message handler: BHVANIM events; ASEG 0x3D resets hit count; zap 0x200000 while held =
 *   extra hit (seek labels, spawn effect, raise hit value +0x33C, noise)
 * Info: proposed name BHVJUGGLED_FHandleMessage (T1); class BHVJUGGLED slot13; TU guardbhv
 * Notes: BHV class word normalised: BHVHELD_FHandleMessage -> BHVJUGGLED_FHandleMessage | runtime2/4
 *   confirmed the BHVJUGGLED role on live guards (S02-S41)
 * Confidence: mid (draft) [T1 0.55] - evidence: runtime (save-state RAM); sources:
 *   r9_bhv_screen+runtime(class role)
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
int BHVJUGGLED_FHandleMessage(struct S *p, int msg, struct M *pm) {
    if (BHVANIM_FHandleMessage(p, msg, pm)) return 1;
    switch (msg) {
    case 9:
        if (p->p40 == 0) return 0;
        if (func_00209518(p->p40) != pm->n0) return 0;
        if (pm->n8 == 0x3D) p->n58 = 0;
        return 0;
    case 0xC:
        {
            int i, n;
            float g;
            struct G *pg;
            struct O *o;
            struct A *a;
            if (pm->nC != 0x200000) return 0;
            if (!(pm->n44 & 8)) return 0;
            if (!p->n50) return 0;
            if (FAnimBindingDone(p->p40)) {
                StopAnimBinding(p->p40);
                StartAnimBinding(p->p40, p->p10, p->p40->nC | 1, 0.0f);
            }
            p->n58++;
            g = 0.0f;
            i = 4; if (p->n58 <= 3) i = p->n58;
            while (i > 0) {
                float *r = func_0012F938(p->p40->p4, D_002BE6D0[i]);
                if (r) { g = *r; break; }
                i--;
            }
            func_00130D30(p->p40->p8, g, 1.0f, 0);
            SetBhvjuggledState(p, 0);
            o = PloFindSwObject__FP2SWi3OIDP2LO(&D_003DD490, 5, D_002BE6E8[i], 0);
            if (o) {
                struct X x;
                struct VTE *e;
                func_002333B4(&x, 0, sizeof x);
                x.a = 5;
                x.v = func_001A62E8(p->p10);
                e = &o->vt[0x33];
                ((void (*)(void *, struct X *))e->pfn)((char *)o + e->d, &x);
            }
            a = p->p10;
            {
                int m = a->n33C + pm->n10;
                a->n33C = m < 5 ? 5 : m;
            }
            func_00156698(D_003DD490.p4028, func_001A62E8(p->p10), p->p10);
            return 1;
        }
    }
    return 0;
}

typedef int qword __attribute__((mode(TI)));
struct Q { char pad[0x10]; char x10[0x2C]; float g3c; float g40; float g44; char p1[8]; float g50; float g54; float g58; float g5c; struct Q *next; };
struct E { int a; int b; int c; int d; struct Q *pqFirst; void *pqLast; int ib; int x1c; float g; int oid; float g28; float g2c; };
struct S { char pad[0x1C0]; int x1c0; char p0[4]; int c; struct E ae[4]; };
struct CLOCK { float t; float dt; };
struct V { float a[4]; };
extern struct CLOCK D_002DDED0;
extern void *D_002DFB80;
extern char D_002BE888[];
extern float D_002BE898, D_002BE89C, D_002BE8A0, D_002BE8A4;
void UpdateSo(struct S *p);
float SMaxShape(int oid);
qword func_001A62E8(void *p);
void FindShapeClosestPoint(int oid, qword q, int a, int b, struct V *pv, int c, float *pg);
void FindShapeClosestPointFromS(int oid, qword q, int a, int b, struct V *pv, int c, float *pg, float g);
float GRandInRange__Fff(float lo, float hi);
void func_00181CB0(struct S *p, struct E *pe);
float GSmoothA__FffffP4SMPAPf(void *pv, float *pg, float a, float b, float c, float dt);
void EvaluateShapeS(int oid, struct Q *q, void *pv, float g);
void func_00181E10(struct S *p, struct Q *q);
static __inline__ void Integrate(float *px, float *pv, float a, float dt)
{
    float vOld = *pv;
    float vAvg;
    *pv = vOld + dt * a;
    vAvg = (vOld + *pv) * 0.5f;
    *px += dt * vAvg;
}
/** @sly2-annot addr=0x181F60
 * Purpose: vt16: UpdateSo, per-segment shape tracking and random effects
 * Info: proposed name UpdateHackwall (T2); class HACKWALL slot16; TU tu_17F090
 * Notes: names_v2 C proposed SetSggSggs__FP3SGG4SGGS (0.438) | Cid4C->Cid76 (CID 0x4C=76, hex-literal) |
 *   verifier PLAUSIBLE: Placeholder class. CID 0x4C vtable slot 16: UpdateSo, then per entry (max 4): closest
 *   point on shape to D_002DFB80's position, full search every 0.5-0.75 s (GRandInRange) else from previous
 *   s, piece u | class identity from whole_game WE names/object strings: UpdateCid76 -> UpdateHackwall | T2
 *   forced: verifier: placeholder name
 * Confidence: low (draft) [T2 0.35] - evidence: static naming pass; sources:
 *   r4b_rest+verdicts_followups+whole_game(class identity)
 * See: docs/engine/gameplay-objects.md#key-functions,
 *   docs/reference/functions/gameplay-objects.md#tu_17f090-0x17f090-0x182c70
 * @end-sly2-annot */
void func_00181F60(struct S *p)
{
    int i;
    struct V v;
    UpdateSo(p);
    if (!p->x1c0) return;
    for (i = 0; i < p->c; i++) {
        struct E *pe = &p->ae[i];
        int oid = pe->oid;
        struct Q *q;
        SMaxShape(oid);
        if (!(D_002DDED0.t <= pe->g2c)) {
            float t;
            FindShapeClosestPoint(oid, func_001A62E8(D_002DFB80), 0, 0, &v, 0, &pe->g28);
            t = D_002DDED0.t;
            pe->g2c = t + GRandInRange__Fff(0.5f, 0.75f);
        } else {
            FindShapeClosestPointFromS(oid, func_001A62E8(D_002DFB80), 0, 0, &v, 0, &pe->g28, pe->g28);
        }
        func_00181CB0(p, pe);
        for (q = pe->pqFirst; q; q = q->next) {
            float a, b;
            q->g3c = GSmoothA__FffffP4SMPAPf(D_002BE888, &q->g40, q->g3c, q->g40, q->g44, D_002DDED0.dt);
            EvaluateShapeS(oid, q, q->x10, q->g3c);
            a = D_002BE898 * q->g50 + D_002BE89C * q->g58;
            b = D_002BE8A0 * q->g54 + D_002BE8A4 * q->g5c;
            Integrate(&q->g50, &q->g58, a, D_002DDED0.dt);
            Integrate(&q->g50, &q->g5c, b, D_002DDED0.dt);
            func_00181E10(p, q);
        }
    }
}

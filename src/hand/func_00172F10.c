typedef int qword __attribute__((mode(TI)));
struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
extern float D_002A53D4;
struct Q2 { qword a, b; };
struct E { qword pos; qword pad; };
struct S { char pad[0x18]; void *p18; char pad2[0x20]; float g3C; char pad3[0x10]; struct E ae[4]; unsigned int cD0; };
int FBhvgotoArrived(void *p);
void func_002562A8(struct Q2 *pq);
void SetBhvgotoGoal(void *p, struct Q2 *pq);
/** @sly2-annot addr=0x172F10
 * Purpose: Every ~1 s after arriving, moves BHV186 to the next of the 4 search points
 * Info: proposed name StepBhv171SearchPoint (T2); TU tu_171F58
 * Notes: names_v2 C proposed anticrack_itm_firewall (0.638)
 * Confidence: low (draft) [T2 0.40] - evidence: static naming pass; sources: r3b_rest
 * See: docs/reference/functions/guards-ai.md#tu_171f58-0x171f58-0x173158
 * @end-sly2-annot */
void func_00172F10(struct S *p) {
    void *p18;
    struct E *pe;
    struct Q2 q;
    if (p->cD0 >= 4) return;
    p18 = p->p18;
    if (p->g3C == D_002A53D4) {
        if (FBhvgotoArrived(p18)) p->g3C = D_002DDED0.t + 1.0f;
    }
    if (D_002DDED0.t < p->g3C) return;
    pe = &p->ae[p->cD0++];
    if (p->cD0 >= 4) p->cD0 = 0;
    func_002562A8(&q);
    q.b = pe->pos;
    SetBhvgotoGoal(p18, &q);
    p->g3C = D_002A53D4;
}

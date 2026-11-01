typedef int qword __attribute__((mode(TI)));
struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
struct V { char pad[0x20]; qword q20; };
struct T { char pad[0x35C]; struct V *p35C; };
struct S { char pad[0x10]; struct T *pt; char pad2[0x3C]; int n50; int pad3; float g58; };
void func_00174B68(struct S *p);
void func_0017A5B0(struct S *p, int n);
qword func_001A62E8(void *p);
/** @sly2-annot addr=0x17A670
 * Purpose: BHVSLEEP Update: wake (state 1) when timer expires; keep held noise at the guard
 * Info: proposed name UpdateBhv175 (T2); class BHV175 slot10; TU guardbhv
 * Notes: unverified names_v2 tier-B alignment hint (tier B verified ~33% precise) | COLLISION: ConvertAloVec
 *   already used (same name proposed for 0x1A6FC0); renamed to ConvertAloVec_17A670 | superseded:
 *   ConvertAloVec_17A670 (names_v2:B(unverified)) | BHV class word normalised: UpdateBhvsleep -> UpdateBhv175
 * Confidence: low (draft) [T2 0.40] - evidence: static naming pass; sources:
 *   names_v2:B(unverified)+r9_bhv_screen
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void func_0017A670(struct S *p) {
    int s;
    func_00174B68(p);
    do {
        s = p->n50;
        if (s == 0 && D_002DDED0.t > p->g58) func_0017A5B0(p, 1);
    } while (p->n50 != s);
    { struct T *t = p->pt; struct V *v = t->p35C; v->q20 = func_001A62E8(t); }
}

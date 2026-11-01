typedef unsigned long ulong;
struct S8 { int a, b; };
struct N { int n0; struct N *next; };
struct E { char pad[0x64]; struct N *pn; int pad2; int n6C; };
struct P { char pad[0xEC]; struct E *pe; char pad2[0x148 - 0xF0]; ulong fl; };
struct G;
extern struct G D_003DD490;
extern struct S8 D_002A8278;
void func_00149AB8(struct G *, struct P *, struct S8, int);
/** @sly2-annot addr=0x1F4F10
 * Purpose: SO vt58: if tree touching flags set, schedule touch callback 0x1F7950 (via D_002A8278) with g_sw
 * Info: proposed name ScheduleSoTouchEvents (T2); class SO slot58; TU so
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/guards-ai.md#step-0x203948-0x205e70,
 *   docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F4F10(struct P *p)
{
    struct E *pe = p->pe;
    struct N *pn;
    if (pe->n6C) {
        func_00149AB8(&D_003DD490, p, D_002A8278, 0);
    } else if (p->fl & 0x3000000) {
        for (pn = pe->pn; pn; pn = pn->next) {
            if (pn->n0) {
                func_00149AB8(&D_003DD490, p, D_002A8278, 0);
                break;
            }
        }
    }
}

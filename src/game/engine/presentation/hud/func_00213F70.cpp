typedef int qword __attribute__((mode(TI)));
struct RGBA { unsigned int u; RGBA(unsigned int x) { u = x; } } __attribute__((aligned(16)));
struct QQ { qword a, b; };
struct Z { int x0; int k; char pad8[8]; int id; };
struct Y { int c0; int c1; char pad8[8]; QQ q; Z *pz; char pad34[0x24]; int x58; char pad5c[0xC]; int x68; int x6C; };
struct H { int c; Y *ay; char pad8[0x10]; };
struct A { unsigned short c; unsigned short c2; void *ae; H *ah; char padc[0x24]; };
struct S { char pad[0x84]; A *aa; };
extern "C" void func_00218660(A *pa, int i);
extern "C" void ClearSle(int *p);
extern "C" int IFreeSpriteSlot(A *pa);
extern "C" void func_00217D98(A *pa, int i, int z, void *q, float f, const RGBA &c0, const RGBA &c1);
/** @sly2-annot addr=0x213F70
 * Purpose: recursively find nested-sprite item k and restart it at anim z
 * Info: proposed name RestartNestedSprite (T2); TU hud
 * Notes: names_v2 v1 proposed FUN_001ad970 (0.000)
 * Confidence: low (draft) [T2 0.40] - evidence: static naming pass; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
extern "C" int func_00213F70(S *p, int k, int z, int c, Y *ay)
{
    int i;
    Y *y;
    if (!ay) {
        H *h = p->aa->ah;
        c = h->c;
        ay = h->ay;
    }
    for (i = 0; i < c; i++) {
        Z *pz;
        y = &ay[i];
        pz = y->pz;
        if (!pz || pz->k != 4)
            continue;
        int oldframe=y->x58;
        if (pz->id == k) {
            func_00218660(&p->aa[k], oldframe);
            ClearSle(&y->x68);
            y->x58 = IFreeSpriteSlot(&p->aa[k]);
            func_00217D98(&p->aa[k], z, y->x58, &y->q, 0.0f, RGBA(0x7F7F7F7F), RGBA(0xFFFFFFFF));
            return 1;
        }
        {
            H *h = (H *)(oldframe * sizeof(H) + (int)p->aa[pz->id].ah);
            if (func_00213F70(p, k, z, h->c, h->ay))
                return 1;
        }
    }
    return 0;
}

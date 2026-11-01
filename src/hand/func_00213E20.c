typedef int qword __attribute__((mode(TI)));
struct RGBA { unsigned char r, g, b, a; };
struct QQ { qword a, b; };
struct Z { int x0; int k; char pad8[8]; int id; };
struct Y { struct RGBA c0; struct RGBA c1; char pad8[8]; struct QQ q; struct Z *pz; char pad34[0x24]; int x58; char pad5c[0x14]; };
struct H { int c; struct Y *ay; char pad8[0x10]; };
struct A { unsigned short c; unsigned short c2; void *ae; struct H *ah; char padc[0x24]; };
struct S { char pad[0x84]; struct A *aa; };
/** @sly2-annot addr=0x213E20
 * Purpose: recursive search of nested sprite k: return its M23 and colours
 * Info: proposed name FFindSpriteItemPose (T2); TU hud
 * Notes: names_v2 B proposed ClineWrapPchz__5CFontPcf (0.856)
 * Confidence: low (draft) [T2 0.40] - evidence: static naming pass; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
int func_00213E20(struct S *p, int k, struct QQ *pq, struct RGBA *pc0, struct RGBA *pc1, int c, struct Y *ay)
{
    int i;
    struct Y *y;
    if (!ay) {
        struct H *h = p->aa->ah;
        c = h->c;
        ay = h->ay;
    }
    for (i = 0; i < c; i++) {
        struct Z *z;
        y = &ay[i];
        z = y->pz;
        if (!z || z->k != 4)
            continue;
        if (k == z->id) {
            *pq = y->q;
            *pc0 = y->c0;
            *pc1 = y->c1;
            return 1;
        }
        if (func_00213E20(p, k, pq, pc0, pc1, p->aa[z->id].ah[y->x58].c, p->aa[z->id].ah[y->x58].ay))
            return 1;
    }
    return 0;
}

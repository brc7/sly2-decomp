typedef int qword __attribute__((mode(TI)));
typedef struct { float x, y, z, w; } VEC;
typedef union { qword q; VEC v; } UV;
struct VTE { short d; short pad; void (*pfn)(); };
struct O { char pad[0xC]; struct VTE *vt; };
struct T { char pad[0x4C]; struct O *po; };
struct S {
    char pad[0xC]; struct VTE *vt; char p0[0x198]; float g1a8; char p1[0x5C]; struct T *pt208;
    char p2[0xE1C]; void *x1028; char p3[8]; void *x1034; char p4[0x4DC]; int x1514; char p5[8];
    char x1520[0x100]; char x1620[0x3B4]; int x19d4, x19d8, x19dc, x19e0; char p6[0xC]; int x19f0, x19f4;
};
struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
int func_001430C8(struct S *p);
void SetSmaMask(void *p, int n);
float GLimitAbs__Fff(float g, float h);
void func_00204660(struct S *p, float g);
void RebuildJtXmg(struct S *p, int a, int b, int c, float g, void *pv);
int FMatchJtXmg(struct S *p, void *pv, int n);
int IsmsFindSmOptional__FP2SM3OID(void *p, int n);
void SnapSma(void *p, int n);
/** @sly2-annot addr=0x18A0C8
 * Purpose: Standing: SM mask from 0x1430C8, ride platform rotation, RebuildJtXmg for both feet (x1520/x1620),
 *   FMatchJtXmg
 * Info: proposed name UpdateJtStand (T1); TU jt
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/player.md#jt-0x188488-0x18e540
 * @end-sly2-annot */
void UpdateJtStand(struct S *p)
{
    struct VTE *e;
    SetSmaMask(p->x1034, func_001430C8(p));
    e = &p->vt[92];
    if (!((int (*)(void *))e->pfn)((char *)p + e->d) && p->pt208) {
        struct O *o = p->pt208->po;
        UV u;
        e = &o->vt[34];
        u.q = ((qword (*)(void *))e->pfn)((char *)o + e->d);
        u.v.z = GLimitAbs__Fff(u.v.z, 10.0f);
        func_00204660(p, p->g1a8 + u.v.z * D_002DDED0.dt);
    }
    if (p->x19f0) {
        RebuildJtXmg(p, p->x19d8, p->x19e0, p->x19f0, 5.0f, p->x1520);
        RebuildJtXmg(p, p->x19d4, p->x19dc, p->x19f4, 5.0f, p->x1620);
        if ((!FMatchJtXmg(p, p->x1520, p->x19f0) || !FMatchJtXmg(p, p->x1620, p->x19f4)) &&
            !p->x1514 && p->x1028) {
            if (IsmsFindSmOptional__FP2SM3OID(p->x1028, 0x17C) >= 0) SnapSma(p->x1034, 0x17C);
            p->x1514 = 1;
        }
    }
}

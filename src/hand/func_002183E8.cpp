struct RGBA2 { unsigned char r, g, b, a; };
struct RGBA {
    unsigned char r, g, b, a;
    RGBA(const RGBA &c) { *(RGBA2 *)this = *(const RGBA2 *)&c; }
} __attribute__((aligned(16)));
struct M23 { float a[6]; } __attribute__((aligned(16)));
struct E { char pad[8]; short h8; short hA; float fC; int x10; int x14; };
struct F {char pad[8];float duration;char padC[0xC];};
struct S { int x0; F *af; E *ae; char padc[0x20]; M23 *pm; };
static inline M23 Mul(const M23 &q, const M23 &n)
{
    float m0 = q.a[0], m1 = q.a[1], m2 = q.a[2], m3 = q.a[3], m4 = q.a[4], m5 = q.a[5];
    M23 r;
    r.a[0] = m0 * n.a[0] + m1 * n.a[2];
    r.a[1] = m0 * n.a[1] + m1 * n.a[3];
    r.a[2] = m2 * n.a[0] + m3 * n.a[2];
    r.a[3] = m2 * n.a[1] + m3 * n.a[3];
    r.a[4] = m4 * n.a[0] + m5 * n.a[2] + n.a[4];
    r.a[5] = m4 * n.a[1] + m5 * n.a[3] + n.a[5];
    return r;
}
extern "C" void func_002188B0(float,F *,int,M23 *,RGBA,RGBA);
extern "C" void func_00219FE0(float,F *,int,M23 *,RGBA,RGBA);
/** @sly2-annot addr=0x2183E8
 * Purpose: forward update to 0x2188B0 or 0x219FE0
 * Info: proposed name UpdateSpriteSlot (T2); TU hud
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
extern "C" void func_002183E8(S *p,int i,M23 *pm,float dt,RGBA c0,RGBA c1) {
    E *e=&p->ae[i];
    int h=e->h8;
    if(h<0)return;
    M23 m;
    M23 *pmUse;
    if(p->pm) {
        m=Mul(*pm,*p->pm);
        pmUse=&m;
    } else pmUse=pm;
    if(e->x10) {
        func_002188B0(dt,&p->af[h],i,pmUse,c0,c1);
    } else {
        if(p->af[h].duration <= e->fC) {
            e->fC=0.0f;
            e->hA=0;
        } else e->fC+=dt;
        func_00219FE0(dt,&p->af[h],i,pmUse,c0,c1);
    }
}

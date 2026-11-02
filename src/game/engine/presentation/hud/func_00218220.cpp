typedef int qword __attribute__((mode(TI)));
struct RGBA { unsigned int u; RGBA(unsigned int x) {u=x;} } __attribute__((aligned(16)));
struct M23 {float a[6];} __attribute__((aligned(16)));
struct O {char pad[0xF4]; void *follow; M23 *pm;};
struct S {char pad[0x24]; O *owner; int enabled;};
extern "C" M23 D_002A3A10;
struct G;
extern "C" G D_002DDF40;
extern "C" qword func_001A62E8(void *);
extern "C" qword func_0014F308(void *,qword);
extern "C" void func_002183E8(S *,int,M23 *,float,const RGBA &,const RGBA &);
static inline float GetX(qword q) {float f; __asm__("mtc1 %1,%0":"=f"(f):"r"(q));return f;}
static inline float GetY(qword q) {float f; __asm__("prot3w %0,%1":"=r"(f):"r"(q));return f;}
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
/** @sly2-annot addr=0x218220
 * Purpose: advance main sprite by dt; world-anchored position via PosLoWorld + ConvertCmWorldToScreen
 * Info: proposed name AdvanceWidgetSprite (T2); TU hud
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
extern "C" void func_00218220(S *p,float dt) {
    if(p->enabled) {
        M23 m=D_002A3A10;
        if(p->owner->follow) {
            qword pos=func_001A62E8(p->owner->follow); qword q=func_0014F308(&D_002DDF40,pos);
            m.a[4] = GetX(q)*320.0f+320.0f;
            m.a[5] = GetY(q)*-246.40001f+246.40001f;
        }
        if(p->owner->pm) m=Mul(m,*p->owner->pm);
        func_002183E8(p,0,&m,dt,RGBA(0x7F7F7F7F),RGBA(0xFFFFFFFF));
    }
}

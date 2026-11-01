typedef int qword __attribute__((mode(TI)));
struct M { qword a[3]; };
extern float D_002C0D10, D_002C0D08, D_002C0D0C;
float func_001E8008(void *,float,float);
void func_001C3500(void *,float,int,struct M *);
void func_001E6F10(void *,float,qword *,int);
float func_0011D448(float,float);
qword func_0011E1A0(float,float,float);
static inline float Abs(float g) { return g<0.0f?-g:g; }
static inline qword Bc(float g) {
    union { qword q; float f; } u;
    qword q;
    __asm__("qmtc2.ni %1, %0" : "=j"(q) : "r"(g));
    u.q=q;
    return q;
}
/** @sly2-annot addr=0x1C3A88
 * Purpose: GRABSPOT iface: orientation at distance
 * Info: proposed name GetPipeGrabMat (T2); class PIPE; TU pipe
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r5_1AE130
 * See: docs/reference/functions/player.md#pipe-0x1c3390-0x1c3d70
 * @end-sly2-annot */
void func_001C3A88(void *p,qword *ppos,qword *pdir,float t)
{
    union { qword q; float f[4]; } vp;
    struct M m;
    qword dir,s,r,pos2;
    float n,mag;
    t=func_001E8008(p,t,D_002C0D10);
    func_001C3500(p,t,1,&m);
    func_001E6F10(p,t,&vp.q,0);
    n=func_0011D448(-1.0f,1.0f);
    mag=__builtin_fabsf(n);
    dir=func_0011E1A0((n*mag)*D_002C0D08,-1.0f,0.0f);
    __asm__("vmulax.xyz $ACC, %1, %0x\n\tvmadday.xyz $ACC, %2, %0y\n\tvmaddz.xyz %0, %3, %0z" : "+j"(dir) : "j"(m.a[0]), "j"(m.a[1]), "j"(m.a[2]));
    pos2=vp.q;
    mag=mag*func_0011D448(0.0f,D_002C0D0C);
    s=Bc(mag);
    __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddx.xyzw %0, %2, %3x" : "=j"(r) : "j"(pos2), "j"(dir), "j"(s));
    *ppos=r;
    *pdir=dir;
}

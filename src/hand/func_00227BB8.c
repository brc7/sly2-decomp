typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f[4]; } VU;
struct S { char pad[0xE0]; char aE0[0x10]; int c; qword *points; char pad1[0x18]; qword v110; float g120; };
extern float D_002A9880;
void func_00231170(struct S *p, void *pbis);
void func_00117FD0(void *pbis, void *pv);
void func_00118220(void *pbis, void *pv, void *pv2);
qword func_0011F288(int c, qword *points);
qword func_0011F2B8(int c, qword *points);
/** @sly2-annot addr=0x227BB8
 * Purpose: LoadXfmFromBrx, ReadGeom, ReadBspc, then bounding sphere of the geometry points
 * Info: proposed name LoadVolFromBrx (T1); class VOL slot14; TU vol
 * Notes: names_v2 C proposed GetHpntClosestHidePos__FP4HPNTP6VECTORPf (0.438) | level_parser CONFIRMED-parse:
 *   payload layout followed by the parser, 100%% of bytes in 4 levels
 * Confidence: high [T1 0.90] - evidence: static naming pass; sources:
 *   r8_21BB88+verdicts_tu_r7r8+parse-verified
 * See: docs/engine/physics-collision.md#4-geometry-and-queries,
 *   docs/engine/physics-collision.md#6-source-files, docs/engine/physics-collision.md#7-key-functions
 * @end-sly2-annot */
void func_00227BB8(struct S *p, void *pbis)
{
    func_00231170(p, pbis);
    func_00117FD0(pbis, &p->c);
    func_00118220(pbis, &p->c, p->aE0);
    if (p->c > 0) {
        VU u;
        qword a,b,m,one; register qword h __asm__("vf1");
        qword g;
        a = func_0011F288(p->c, p->points);
        __asm__("qmtc2.ni %1,%0" : "=j"(a) : "r"(a));
        b = func_0011F2B8(p->c, p->points);
        __asm__("qmtc2.ni %1, %0" : "=j"(h) : "r"(0.5f));
        u.q = h;
        __asm__("vmulax.xyzw $ACC, %1, %2x\n\tvmaddx.xyzw %0, %3, %2x" : "=j"(m) : "j"(a), "j"(h), "j"(b));
        __asm__("vsub.xyzw %0, %1, %0" : "+j"(a) : "j"(m));
        p->v110 = m;
        __asm__("vmul.xyz %0, %0, %0\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "+j"(a), "=&j"(one));
        __asm__("qmfc2.ni %0, %1" : "=r"(g) : "j"(a));
        __asm__("sw %1,%0" : "=m"(p->g120) : "r"(g));
    } else {
        p->g120 = D_002A9880;
    }
}

#define OFF(t,m) ((unsigned)&(((t*)0)->m))
typedef char CheckQwordSize[(sizeof(qword)==16)?1:-1];
typedef char CheckQwordAlignment[(__alignof__(qword)==16)?1:-1];
typedef char CheckVUSize[(sizeof(VU)==16)?1:-1];
typedef char CheckVUAlignment[(__alignof__(VU)==16)?1:-1];
typedef char CheckIntSize[(sizeof(int)==4)?1:-1];
typedef char CheckFloatSize[(sizeof(float)==4)?1:-1];
typedef char CheckPointerSize[(sizeof(void*)==4)?1:-1];
typedef char CheckExtraOffset[(OFF(struct S,aE0)==0xE0)?1:-1];
typedef char CheckCountOffset[(OFF(struct S,c)==0xF0)?1:-1];
typedef char CheckPointsOffset[(OFF(struct S,points)==0xF4)?1:-1];
typedef char CheckCenterOffset[(OFF(struct S,v110)==0x110)?1:-1];
typedef char CheckRadiusSquaredOffset[(OFF(struct S,g120)==0x120)?1:-1];
typedef char CheckPointStride[(sizeof(*((struct S*)0)->points)==16)?1:-1];
typedef char CheckVUQwordOffset[(OFF(VU,q)==0)?1:-1];
typedef char CheckVUFloatOffset[(OFF(VU,f)==0)?1:-1];
typedef char CheckVUFloatExtent[(sizeof(((VU*)0)->f)==16)?1:-1];

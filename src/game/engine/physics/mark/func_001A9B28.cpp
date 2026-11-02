typedef int qword __attribute__((mode(TI)));
union U { qword q; float f[4]; U(); };
struct S0 { char pad[0x70]; qword x70; };
struct LO;struct CONTACT;struct NODE;
struct S { struct S0 *x0; struct LO *x4; float x8; };
union DETAIL {void*pointer;int word;};
struct E {qword x0,x10;int x20;DETAIL detail;};
extern "C" int func_001A93C8(qword pos, struct S*b,int c,int d,struct NODE*e,int n, struct E *buf, float f);
extern "C" U*func_002564B8(U*p);
extern "C" void func_001A7C18(void *a, struct S*s,struct S*b,qword p, qword q, int x20, void*z, void*x24,struct CONTACT**g,float f);
inline U::U() { (void)func_002564B8(this); }
#define FIELD_OFFSET(t,m) ((unsigned int)&(((t*)0x10000)->m)-0x10000)
typedef char Offset_0[FIELD_OFFSET(U,q)==0?1:-1];
typedef char Offset_1[FIELD_OFFSET(U,f)==0?1:-1];
typedef char Offset_2[FIELD_OFFSET(DETAIL,pointer)==0?1:-1];
typedef char Offset_3[FIELD_OFFSET(DETAIL,word)==0?1:-1];
typedef char Offset_4[FIELD_OFFSET(S0,x70)==112?1:-1];
typedef char Offset_5[FIELD_OFFSET(S,x0)==0?1:-1];
typedef char Offset_6[FIELD_OFFSET(S,x4)==4?1:-1];
typedef char Offset_7[FIELD_OFFSET(S,x8)==8?1:-1];
typedef char Offset_8[FIELD_OFFSET(E,x0)==0?1:-1];
typedef char Offset_9[FIELD_OFFSET(E,x10)==16?1:-1];
typedef char Offset_10[FIELD_OFFSET(E,x20)==32?1:-1];
typedef char Offset_11[FIELD_OFFSET(E,detail)==36?1:-1];
typedef char Size_0[sizeof(U)==16?1:-1];
typedef char Size_1[sizeof(DETAIL)==4?1:-1];
typedef char Size_2[sizeof(S0)==128?1:-1];
typedef char Size_3[sizeof(S)==12?1:-1];
typedef char Size_4[sizeof(E)==48?1:-1];
typedef char Size_5[sizeof(qword)==16?1:-1];
typedef char Size_6[sizeof(void*)==4?1:-1];
typedef char Size_7[sizeof(int)==4?1:-1];
typedef char Size_8[sizeof(float)==4?1:-1];
/** @sly2-annot addr=0x1A9B28
 * Purpose: Sphere vs BSP shape: CsbiIntersectSphereBsp then PxpAdd for each hit
 * Info: proposed name MarkSoContactsSphereBsp (T1); TU mark
 * Confidence: mid (draft) [T1 0.65] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/physics-collision.md#mark-0x1a7c18-0x1aa978
 * @end-sly2-annot */
extern "C" void func_001A9B28(void *a, struct S*s,struct S*b,int d,struct NODE*e,struct CONTACT**g)
{
    register S*bodyOwner __asm__("$19")=b;
    register void*worldOwner __asm__("$21")=a;
    struct E buf[64];

    int i;
    struct E *pe;
    i = func_001A93C8(s->x0->x70, bodyOwner, 0x7FFFFFFF, d, e, 0x40, buf, s->x8) - 1;
    if (i >= 0) {
        pe = &buf[i];
        do {
            qword A, B;
            i--;
            {register qword word __asm__("$2")=pe->x0;A=word;}
            {register qword word __asm__("$2")=pe->x10;B=word;}
            U r,one;
            register qword scalar __asm__("vf1"),product __asm__("vf2");
            register void*worldArgument __asm__("$4");register S*shapeArgument __asm__("$5");
            __asm__("lqc2 $vf3,%5\n\t"
                    "daddu %2,%6,$zero\n\t"
                    "lqc2 $vf4,%4\n\t"
                    "daddu %3,%7,$zero\n\t"
                    "vmul.xyz %1,$vf3,$vf4\n\t"
                    "vaddw.x %0,$vf0,$vf0w\n\t"
                    "vadday.x $ACC,%1,%1y\n\t"
                    "vmaddaz.x $ACC,%0,%1z\n\t"
                    "vmsubw.x %1,%0,$vf3w"
                    : "=&j"(scalar),"=&j"(product),"=&r"(worldArgument),"=r"(shapeArgument)
                    : "m"(A),"m"(B),"r"(worldOwner),"r"(s) : "vf3","vf4");
            one.q=scalar;
            register S*bodyArgument __asm__("$6");register void*emptyFeature __asm__("$10");
            register qword pointArgument __asm__("$7"),planeArgument __asm__("$8");register float separation __asm__("$f12");
            __asm__("daddu %0,%6,$zero\n\t"
                    "sqc2 %7,%5\n\t"
                    "daddu %1,$zero,$zero\n\t"
                    "lq %2,%8\n\t"
                    "qmfc2.ni $2,%7\n\t"
                    "lq %3,%9\n\t"
                    "mtc1 $2,%4"
                    : "=&r"(bodyArgument),"=&r"(emptyFeature),"=&r"(pointArgument),"=r"(planeArgument),"=f"(separation),"=m"(r.q)
                    : "r"(bodyOwner),"j"(product),"m"(A),"m"(B) : "$2");
            func_001A7C18(worldArgument, shapeArgument, bodyArgument, pointArgument, planeArgument, pe->x20, emptyFeature, pe->detail.pointer, g, separation);
            pe=(E*)((unsigned)pe-sizeof(E));
        } while (i >= 0);
    }
}

typedef char layout_qword_Alignment[(__alignof__(qword)==16)?1:-1];
typedef char layout_U_Alignment[(__alignof__(U)==16)?1:-1];
typedef char layout_S0_Alignment[(__alignof__(S0)==16)?1:-1];
typedef char layout_S_Alignment[(__alignof__(S)==4)?1:-1];
typedef char layout_DETAIL_Alignment[(__alignof__(DETAIL)==4)?1:-1];
typedef char layout_E_Alignment[(__alignof__(E)==16)?1:-1];
typedef char layout_CandidateBuffer_Span[(sizeof(E[64])==0xC00)?1:-1];
typedef char layout_CandidatePoint_Span[(sizeof(((E*)0)->x0)==16)?1:-1];
typedef char layout_CandidatePlane_Span[(sizeof(((E*)0)->x10)==16)?1:-1];
typedef char layout_CandidateKind_Width[(sizeof(((E*)0)->x20)==4)?1:-1];
typedef char layout_CandidateFeature_Width[(sizeof(((E*)0)->detail.pointer)==4)?1:-1];
typedef char layout_OwnerPoint_Span[(sizeof(((S0*)0)->x70)==16)?1:-1];
typedef char layout_ShapeRadius_Width[(sizeof(((S*)0)->x8)==4)?1:-1];
typedef char layout_U_FloatSpan[(sizeof(((U*)0)->f)==16)?1:-1];

// Update actor movement modes and curve transition checks.

typedef int qword __attribute__((mode(TI)));
#define F(T, o) (*(T *)((char *)p + (o)))
#define A(o) ((char *)p + (o))

struct LO { char pad[0x28]; int x28; signed char x2C; char x2D; long k:4; long :8; unsigned int f:1; };
struct X150 { char pad[0x18]; float g18; };
struct X1854 { char pad[0x124]; int x124; };
typedef union { qword q; float f[4]; } VU;

extern float D_002BF3D4;
extern float D_002BF3D8;

extern float D_002BF3DC;

extern void func_001403D0(void *p);
extern void func_0018B7E8(void *p);
extern void func_00204660(void *p, float g);
extern void func_0020BB58(void *p, float g);
extern void func_0020BC08(void *p);
extern void func_001A0450(void *p, void *a, void *animation);
extern void func_0018A0C8(void *p);
extern void func_00190E18(void *p);
extern void func_00185E40(void *p);
extern void func_0018DF60(void *p);

extern void func_00189C40(void *p, void *a, float g, void *geometry, void *response, void *d);
#define C40(p,a,b,c,d,g) func_00189C40(p,a,g,b,c,d)

extern int func_00189EC8(void *p, void *a, void *response);
extern void func_0018ED70(void *p);
extern int func_00132B10(void *p, int n);
extern void func_001E7380(void *p, qword q, void *metric, qword *point, qword *direction, float *normalized, float *distance, float g);
extern void func_0018C250(void *p, int a, int b);
extern qword func_001A62E8(void *p);
extern void func_0011E7C8(qword a, qword b, qword c, qword d, float *parameters, float *distance, qword *direction);
extern void func_00189618(void *p);

static __inline__ qword VMulX(qword a, qword s) { qword r; __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(r) : "j"(a), "j"(s)); return r; }

/** @sly2-annot addr=0x18BCE0
 * Purpose: JT vt16 Update: STEPPLAYER Update 0x1403D0, UpdateJtPre, actuator/aseg updates, UpdateJtStand,
 *   feet, rope/curve checks (SetJtJts 0x16/3/8), UpdateJtLean last
 * Info: proposed name UpdateJt (T1); class JT slot16; TU jt
 * Confidence: high [T1 0.80] - evidence: static analysis; sources: r4_17F090
 * See: docs/engine/player.md#5-sly-jt-jtcpp-0x188488--component-tus, docs/engine/player.md#11-key-functions,
 *   docs/reference/functions/player.md#jt-0x188488-0x18e540
 * @end-sly2-annot */
void func_0018BCE0(char *p)
{
    VU u;
    qword av[3];
    VU w;
    float ag[4];
    int i;
    struct X1854 *po;

    func_001403D0(p);
    if (!((struct LO *)p)->f)
        return;
    if (F(void *, 0x150))
        func_0018B7E8(p);
    switch (F(int, 0x18C8)) {
    case 1: {
        qword s, motion;
        func_00204660(p, F(struct X150 *, 0x150)->g18);
        func_0020BB58(A(0x1320), 1.0f);
        func_0020BC08(A(0x1320));
        {
            unsigned page;
            __asm__("lui %0,%%hi(%2)\n\tlqc2 %1,%3"
                    : "=&r"(page), "=j"(motion)
                    : "i"(&D_002BF3D4), "m"(F(qword, 0x190)));
            __asm__("lw %0,%%lo(%2)(%0)\n\tqmtc2.ni %0,%1"
                    : "+r"(page), "=j"(s)
                    : "i"(&D_002BF3D4) : "memory");
        }
        u.q = s;
        F(qword, 0x190) = VMulX(motion, s);

        break;
    }
    case 5:
        if (F(int, 0x18D0) == 5) {
            qword s, motion;
            func_001A0450(p, A(0x13D0), F(void *, 0x14D0));
            {
                unsigned page;
                __asm__("lui %0,%%hi(%3)\n\tlqc2 %1,%4\n\tlw %0,%%lo(%3)(%0)\n\tqmtc2.ni %0,%2"
                        : "=&r"(page), "=j"(motion), "=j"(s)
                        : "i"(&D_002BF3D8), "m"(F(qword, 0x190)) : "memory");
            }
            u.q = s;
            F(qword, 0x190) = VMulX(motion, s);

            F(float, 0x180) = 1.0f;
        }
        break;
    case 0xA:
        func_001A0450(p, A(0x1AC0), F(void *, 0x1BC0));
        break;
    case 0:
        func_0018A0C8(p);
        break;
    case 3:
        func_00190E18(p);
        break;
    case 0xC:
        func_0020BC08(A(0x1C30));
        break;
    case 0x15:
        func_00185E40(p);
        break;
    case 0x16:
        func_0018DF60(p);
        break;
    case 0x18:
        if (F(void *, 0x19F0)) {
            C40(p, F(void *, 0x19D8), F(void *, 0x19E0), F(void *, 0x19F0), A(0x1520), D_002BF3DC);
            C40(p, F(void *, 0x19D4), F(void *, 0x19DC), F(void *, 0x19F4), A(0x1620), D_002BF3DC);
            (void)func_00189EC8(p, A(0x1520), F(void *, 0x19F0));
            (void)func_00189EC8(p, A(0x1620), F(void *, 0x19F4));
        }
        break;
    }
    func_0018ED70(p);
    if (F(int, 0x18D8) == 0) {
        if (func_00132B10(F(void *, 0x1854), 0xB)) {
            void *po2 = F(void *, 0x1854);
            for (i = 0; i < 3; i++)
                func_001E7380(po2, ((qword *)A(0x1890))[i], 0, &av[i], 0, 0, 0, F(float, 0x1858));
            goto loop;
        L16:
            func_0018C250(p, 0x16, -1);
            goto end;
        } else {
            qword q;
            if (!func_00132B10(F(void *, 0x1854), 0x13))
                goto end;
            q = func_001A62E8(F(void *, 0x1854));
            av[2] = q;
            av[1] = q;
            av[0] = q;
        }
    loop:

        for (i = 1; i < F(int, 0x1880); i++) {
            qword *aq = (qword *)A(0x1890);
            func_0011E7C8(aq[i - 1], aq[i], av[i - 1], av[i], u.f, ag, &w.q);

            if (ag[0] < F(float, 0x19AC)) {
                if (func_00132B10(F(void *, 0x1854), 0x13)) {
                    po = F(struct X1854 *, 0x1854);
                    F(struct X1854 *, 0x18C4) = po;
                    if (po->x124 == 3)
                        goto L16;
                    func_0018C250(p, 3, 1);
                } else {
                    F(void *, 0xF5C) = F(void *, 0x1854);
                    F(float, 0xF60) = F(float, 0x1858);
                    func_0018C250(p, 8, -1);
                }
                break;
            }
        }
    }
end:
    func_00189618(p);
}

typedef char layout_pointer[(sizeof(void*)==4)?1:-1];
typedef char layout_char[(sizeof(char)==1)?1:-1];
typedef char layout_int[(sizeof(int)==4)?1:-1];
typedef char layout_float[(sizeof(float)==4)?1:-1];
typedef char layout_long[(sizeof(long)==8)?1:-1];
typedef char layout_qword[(sizeof(qword)==16)?1:-1];
typedef char layout_VU[(sizeof(VU)==16)?1:-1];
typedef char layout_VU_alignment[(__alignof__(VU)==16)?1:-1];
typedef char layout_LO[(sizeof(struct LO)==0x30)?1:-1];
typedef char layout_LO_word[((unsigned)&((struct LO*)0)->x28==0x28)?1:-1];
typedef char layout_LO_byte2c[((unsigned)&((struct LO*)0)->x2C==0x2C)?1:-1];
typedef char layout_LO_byte2d[((unsigned)&((struct LO*)0)->x2D==0x2D)?1:-1];
typedef char layout_STATE[(sizeof(struct X150)==0x1C)?1:-1];
typedef char layout_STATE_heading[((unsigned)&((struct X150*)0)->g18==0x18)?1:-1];
typedef char layout_TARGET[(sizeof(struct X1854)==0x128)?1:-1];
typedef char layout_TARGET_kind[((unsigned)&((struct X1854*)0)->x124==0x124)?1:-1];

// Update the movement-state controller.
typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f[4]; } U;
struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
extern float D_002BE6C8, D_002BE6CC;
struct VTE { short d; short pad; void *pfn; };
struct C { char pad[0x1C]; float g1C; };
struct B { char pad[0x90]; struct C *p90; };
struct A { char pad[0xC]; struct VTE *vt; char pad2[0x58 - 0x10]; struct B *p58; char pad3[0x208 - 0x5C]; int n208; };
struct Y { char pad[0x60]; void *p60; };
struct X { char pad[0x19A4]; struct Y *p19A4; };
struct S { char pad[0x10]; struct A *p10; char pad2[0x50 - 0x14]; int n50; float g54; int pad3; struct X *p5C; int pad4; struct C *p64; };
struct OUT { char pad[0x30]; qword v; };
void UpdateBhvanim(struct S *p);
void SetBhvjuggledState(struct S *p, int n);
void func_0017ABC8(struct S *p);
qword func_001A62E8(struct A *a);
void LoadIdentityMatrix4(void *a, void *b, struct OUT *pm, struct OUT *other, float g);
static inline void*Frame(struct X*x){register struct Y*y __asm__("$2")=x->p19A4;return y->p60;}
static __inline__ float GetZ(qword q) { float z; __asm__("pextuw %0,$0,%1" : "=r"(z) : "r"(q)); return z; }
/** @sly2-annot addr=0x17AC90
 * Purpose: BHVHELD Update: held/carried states 0-1, released/fall states 2-3 (landed via +0x208), prediction
 *   states 4-5
 * Info: proposed name UpdateBhvjuggled (T1); class BHVJUGGLED slot10; TU guardbhv
 * Notes: BHV class word normalised: UpdateBhvheld -> UpdateBhvjuggled | runtime2/4 confirmed the BHVJUGGLED
 *   role on live guards (S02-S41)
 * Confidence: mid (draft) [T1 0.55] - evidence: runtime (save-state RAM); sources:
 *   r9_bhv_screen+runtime(class role)
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void UpdateBhvjuggled(struct S *p) {
    qword v;
    int s;
    UpdateBhvanim(p);
    {
        struct A *a = p->p10;
        struct VTE *e = &a->vt[33];
        v = ((qword (*)(void *))e->pfn)((char *)a + e->d);
    }
    do {
        s = p->n50;
        switch (s) {
        case 0:
        case 1:
            if (p->n50 == 1 && p->p10->n208) {
                SetBhvjuggledState(p, 3);
            } else if (p->p10->p58->p90 != p->p64 || p->p10->p58->p90->g1C < D_002DDED0.t) {
                SetBhvjuggledState(p, 2);
            } else if (GetZ(v) < 0.0f && p->n50 == 0) {
                SetBhvjuggledState(p, 1);
            } else {
                func_0017ABC8(p);
            }
            break;
        case 2:
            if (p->p10->n208) SetBhvjuggledState(p, 3);
            break;
        case 3:
            break;
        case 4:
        case 5: {
            struct OUT out;
            U a, b, t, vel, ud;
            qword tq, c;
            float dt;
            register void*nullSource __asm__("$5");
            register void*nullOutput __asm__("$7");
            register struct X*frameLink __asm__("$3");
            struct CLOCK *clock=&D_002DDED0;
            a.q = func_001A62E8(p->p10);
            {
                struct A *pa = p->p10;
                struct VTE *e = &pa->vt[33];
                qword velocity = ((qword (*)(void *))e->pfn)((char *)pa + e->d);
                dt=clock->dt;
                /* Preserve velocity and prepare the prediction inputs. */
                __asm__("sq %4,%0\n\tdaddu %1,%z5,$zero\n\tdaddu %2,%z5,$zero\n\tlw %3,%6" : "=m"(b.q),"=&r"(nullOutput),"=&r"(nullSource),"=&r"(frameLink) : "r"(velocity),"J"(0),"m"(p->p5C));
            }
            LoadIdentityMatrix4(Frame(frameLink), nullSource, &out, nullOutput, dt);
            t.q = out.v;
            t.f[2] += D_002BE6C8;
            tq = t.q;
            c = func_001A62E8(p->p10);
            __asm__("vsub.xyzw %0, %1, %2" : "=j"(c) : "j"(tq), "j"(c));
            clock=(struct CLOCK*)*(int*)&clock->dt;
            __asm__("qmtc2.ni %1, %0" : "=j"(ud.q) : "r"(clock));
            __asm__("vdiv $Q, $vf0w, %1x\n\tvwaitq\n\tvmulq.xyzw %0, %2, $Q" : "=j"(vel.q) : "j"(ud.q), "j"(c));
            if (p->n50 == 4) {
                if (a.f[2] < t.f[2] || D_002BE6CC < D_002DDED0.t - p->g54) {
                    SetBhvjuggledState(p, 5);
                    break;
                }
                vel.f[2] = b.f[2];
            }
            {
                struct A *pa = p->p10;
                struct VTE *e = &pa->vt[35];
                ((void (*)(void *, qword))e->pfn)((char *)pa + e->d, vel.q);
            }
            break;
        }
        case 6:
            if (p->p10->n208) SetBhvjuggledState(p, -1);
            break;
        }
    } while (p->n50 != s);
}

typedef char layout_pointer[(sizeof(void*)==4)?1:-1];
typedef char layout_qword[(sizeof(qword)==16)?1:-1];
typedef char layout_U[(sizeof(U)==16)?1:-1];
typedef char layout_VTE[(sizeof(struct VTE)==8)?1:-1];
typedef char layout_CLOCK[(sizeof(struct CLOCK)==8)?1:-1];
typedef char layout_C[(sizeof(struct C)==0x20)?1:-1];
typedef char layout_B[(sizeof(struct B)==0x94)?1:-1];
typedef char layout_A[(sizeof(struct A)==0x20C)?1:-1];
typedef char layout_X[(sizeof(struct X)==0x19A8)?1:-1];
typedef char layout_Y[(sizeof(struct Y)==0x64)?1:-1];
typedef char layout_S[(sizeof(struct S)==0x68)?1:-1];
typedef char layout_OUT[(sizeof(struct OUT)==0x40)?1:-1];
typedef char layout_U_q[((unsigned)&((U*)0)->q==0)?1:-1];
typedef char layout_U_f[((unsigned)&((U*)0)->f==0)?1:-1];
typedef char layout_VTE_delta[((unsigned)&((struct VTE*)0)->d==0)?1:-1];
typedef char layout_VTE_fn[((unsigned)&((struct VTE*)0)->pfn==4)?1:-1];
typedef char layout_CLOCK_time[((unsigned)&((struct CLOCK*)0)->t==0)?1:-1];
typedef char layout_CLOCK_dt[((unsigned)&((struct CLOCK*)0)->dt==4)?1:-1];
typedef char layout_C_gain[((unsigned)&((struct C*)0)->g1C==0x1C)?1:-1];
typedef char layout_B_control[((unsigned)&((struct B*)0)->p90==0x90)?1:-1];
typedef char layout_A_vt[((unsigned)&((struct A*)0)->vt==0xC)?1:-1];
typedef char layout_A_control[((unsigned)&((struct A*)0)->p58==0x58)?1:-1];
typedef char layout_A_active[((unsigned)&((struct A*)0)->n208==0x208)?1:-1];
typedef char layout_X_y[((unsigned)&((struct X*)0)->p19A4==0x19A4)?1:-1];
typedef char layout_Y_frame[((unsigned)&((struct Y*)0)->p60==0x60)?1:-1];
typedef char layout_S_actor[((unsigned)&((struct S*)0)->p10==0x10)?1:-1];
typedef char layout_S_state[((unsigned)&((struct S*)0)->n50==0x50)?1:-1];
typedef char layout_S_time[((unsigned)&((struct S*)0)->g54==0x54)?1:-1];
typedef char layout_S_target[((unsigned)&((struct S*)0)->p5C==0x5C)?1:-1];
typedef char layout_S_control[((unsigned)&((struct S*)0)->p64==0x64)?1:-1];
typedef char layout_OUT_position[((unsigned)&((struct OUT*)0)->v==0x30)?1:-1];

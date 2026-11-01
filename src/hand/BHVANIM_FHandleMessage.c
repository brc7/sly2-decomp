typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f; } U;
struct G;
struct ANIMATION;
extern struct G D_003DD490;
extern void *D_002F7900;
extern char D_002DFC00[];
extern char D_002DDF40[];
struct VTE { short d; short pad; void *pfn; };
struct A {
    char pad[0xC]; struct VTE *vt; char pad2[0x190 - 0x10]; qword v190; char pad3[4]; int n1A4; char pad4[8];
    qword m1B0[3]; char pad5[0xFD0 - 0x1E0]; char aFD0[4];
};
struct O { char pad[0x30]; void *p30; char pad2[0x1C]; void *parent; };
struct S { char pad[0x10]; struct A *p10; char pad2[0x2C]; void *p40; };
struct M { struct ANIMATION *animation; float f4; int n8; };
struct ANIMATION *func_00209518(void *p);
qword func_00204E38(struct A *a, struct ANIMATION *animation, float f);
void LockStepguardMover(void *p, struct S *s);
void func_002097D8(void *p, struct S *s);
void SetBhvBusy(struct S *p);
void ClearBhvBusy(struct S *p);
struct O *func_00205458(struct A *a, int n);
void *PpoHero(struct G *g);
void TriggerJoyRumbleRumk__FP3JOY4RUMKf(void *p, int n, float f);
int FFindLoParent(void *n, void *o);
void func_0014F750(void *p, int n);
void func_001F7A68(struct A *a, int n);
/** @sly2-annot addr=0x174BD0
 * Purpose: BHVANIM message handler: msg 9 = ASEG event of the playing binding: 0x15E root-motion translation,
 *   0x25/0x26 lock/unlock mover, 0x27/0x28 set/clear busy, 0x29/0x2A ground flag, 0x2F/0x34/0x35
 *   rumble/camera jolt on hit
 * Info: proposed name BHVANIM_FHandleMessage (T1); class BHVANIM slot13; TU guardbhv
 * Notes: runtime2/4 confirmed the BHVANIM role on live guards (S02-S41) | BHV slot 13 (_FHandleMessage):
 *   canonical form would be BHVLEAP_FHandleMessage | BHV slot 13 (_FHandleMessage): canonical form would be
 *   BHV175_FHandleMessage
 * Confidence: high [T1 0.80] - evidence: runtime (save-state RAM); sources: r9_bhv_screen+runtime(class role)
 * See: docs/engine/guards-ai.md#9-key-functions,
 *   docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
int BHVANIM_FHandleMessage(struct S *p, int msg, struct M *pm) {
    struct ANIMATION *animation;
    if (msg != 9) return 0;
    if (!p->p40) return 0;
    animation = func_00209518(p->p40);
    if (animation != pm->animation) return 0;
    switch (pm->n8) {
    case 0x15E:
        {
            qword v = func_00204E38(p->p10, animation, pm->f4);
            qword n, r;
            U u;
            struct A *a;
            __asm__("qmtc2.ni %1, %0" : "=j"(u.q) : "r"(-1.0f));
            __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(n) : "j"(v), "j"(u.q));
            p->p10->v190 = n;
            a = p->p10;
            {
                qword *m = a->m1B0;
                __asm__("vmulax.xyz $ACC, %1, %4x\n\tvmadday.xyz $ACC, %2, %4y\n\tvmaddz.xyz %0, %3, %4z"
                        : "=j"(r) : "j"(m[0]), "j"(m[1]), "j"(m[2]), "j"(v));
            }
            { struct VTE *e = &a->vt[0x23]; ((void (*)(void *, qword))e->pfn)((char *)a + e->d, r); }
        }
        return 1;
    case 0x25:
        LockStepguardMover(p->p10->aFD0, p);
        return 1;
    case 0x26:
        func_002097D8(p->p10->aFD0, p);
        return 1;
    case 0x29:
        p->p10->n1A4 = 1;
        return 1;
    case 0x2A:
        p->p10->n1A4 = 0;
        return 1;
    case 0x27:
        SetBhvBusy(p);
        return 1;
    case 0x28:
        ClearBhvBusy(p);
        return 1;
    case 0x2F:
        {
            struct O *o = func_00205458(p->p10, 0xA5);
            if (o && o->p30 == PpoHero(&D_003DD490)) TriggerJoyRumbleRumk__FP3JOY4RUMKf(D_002DFC00, 8, 1.0f);
        }
        return 0;
    case 0x34:
    case 0x35:
        {
            void *g = PpoHero(&D_003DD490);
            struct O *o = func_00205458(p->p10, 0xA0);
            if (o && FFindLoParent(o->parent, g) && g == D_002F7900) {
                switch (pm->n8) {
                case 0x35: func_0014F750(D_002DDF40, 0); break;
                case 0x34: TriggerJoyRumbleRumk__FP3JOY4RUMKf(D_002DFC00, 3, 1.50000005960464477539e-01f); break;
                }
            }
        }
        return 0;
    case 0x36:
        func_001F7A68(p->p10, 2);
        return 0;
    }
    return 0;
}

typedef char layout_pointer[(sizeof(void*)==4)?1:-1];
typedef char layout_qword[(sizeof(qword)==16)?1:-1];
typedef char layout_U[(sizeof(U)==16)?1:-1];
typedef char layout_float[(sizeof(float)==4)?1:-1];
typedef char layout_VTE[(sizeof(struct VTE)==8)?1:-1];
typedef char layout_M[(sizeof(struct M)==12)?1:-1];
typedef char layout_VTE_d[((unsigned)&((struct VTE*)0)->d==0x0)?1:-1];
typedef char layout_VTE_pfn[((unsigned)&((struct VTE*)0)->pfn==0x4)?1:-1];
typedef char layout_A_vt[((unsigned)&((struct A*)0)->vt==0xc)?1:-1];
typedef char layout_A_v190[((unsigned)&((struct A*)0)->v190==0x190)?1:-1];
typedef char layout_A_n1A4[((unsigned)&((struct A*)0)->n1A4==0x1a4)?1:-1];
typedef char layout_A_m1B0[((unsigned)&((struct A*)0)->m1B0==0x1b0)?1:-1];
typedef char layout_A_aFD0[((unsigned)&((struct A*)0)->aFD0==0xfd0)?1:-1];
typedef char layout_S_p10[((unsigned)&((struct S*)0)->p10==0x10)?1:-1];
typedef char layout_S_p40[((unsigned)&((struct S*)0)->p40==0x40)?1:-1];
typedef char layout_O_p30[((unsigned)&((struct O*)0)->p30==0x30)?1:-1];
typedef char layout_O_parent[((unsigned)&((struct O*)0)->parent==0x50)?1:-1];
typedef char layout_M_animation[((unsigned)&((struct M*)0)->animation==0x0)?1:-1];
typedef char layout_M_f4[((unsigned)&((struct M*)0)->f4==0x4)?1:-1];
typedef char layout_M_n8[((unsigned)&((struct M*)0)->n8==0x8)?1:-1];

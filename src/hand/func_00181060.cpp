typedef int qword __attribute__((mode(TI)));
typedef struct { qword r[3]; } MATRIX3;
struct ANIMATION;
struct VT { struct { short d; short pad; void (*pfn)(void *, qword); } a[40]; };
struct X { char pad[0x30]; ANIMATION *x30; };
struct O { char pad[0xC]; struct VT *pvt; char pad1[0x194 - 0x10]; ANIMATION *x194; struct X *p198; char pad2[0x1C8 - 0x19C];
    int x1C8; int x1CC; char pad3[0x204 - 0x1D0]; float g204; char pad4[0x210 - 0x208]; qword v210; };
struct MSG { ANIMATION *animation; float time; int kind; };
extern "C" void HandlePoMessage(struct O *p, int msg, struct MSG *pm);
extern "C" MATRIX3 func_0011B500(qword v, float g);
extern "C" qword func_00204E38(struct O *p, ANIMATION *animation, float time);
extern "C" qword D_002A2DC0;
extern "C" void func_00181060(struct O *p, int msg, struct MSG *pm);
void func_00181060(struct O *p, int msg, struct MSG *pm)
{
    HandlePoMessage(p, msg, pm);
    if (msg == 9) {
        if (pm->animation == p->x194 || (p->p198 && pm->animation == p->p198->x30)) {
            switch (pm->kind) {
            case 0x15E: {
                MATRIX3 m;
                qword v, r;
                m = func_0011B500(D_002A2DC0, p->g204);
                v = func_00204E38(p, pm->animation, pm->time);
                __asm__("vmulax.xyz ACC, %1, %4x\n\tvmadday.xyz ACC, %2, %4y\n\tvmaddz.xyz %0, %3, %4z"
                    : "=&j"(r) : "j"(m.r[0]), "j"(m.r[1]), "j"(m.r[2]), "j"(v));
                p->v210 = r;
                p->pvt->a[35].pfn((char *)p + p->pvt->a[35].d, p->v210);
                break;
            }
            case 0x25:
                p->x1C8 = 1;
                break;
            case 0x26:
                p->x1C8 = 0;
                break;
            case 0x27:
                p->x1CC = 1;
                break;
            case 0x28:
                p->x1CC = 0;
                break;
            }
        }
    }
}

typedef char layout_qword[(sizeof(qword)==16)?1:-1];
typedef char layout_pointer[(sizeof(void*)==4)?1:-1];
typedef char layout_MATRIX3[(sizeof(MATRIX3)==0x30)?1:-1];
typedef char layout_MSG[(sizeof(MSG)==12)?1:-1];
typedef char layout_VT_entry[(sizeof(((VT*)0)->a[0])==8)?1:-1];
typedef char layout_MSG_animation[((unsigned long)&((MSG*)0)->animation==0x0)?1:-1];
typedef char layout_MSG_time[((unsigned long)&((MSG*)0)->time==0x4)?1:-1];
typedef char layout_MSG_kind[((unsigned long)&((MSG*)0)->kind==0x8)?1:-1];
typedef char layout_X_x30[((unsigned long)&((X*)0)->x30==0x30)?1:-1];
typedef char layout_O_pvt[((unsigned long)&((O*)0)->pvt==0xc)?1:-1];
typedef char layout_O_x194[((unsigned long)&((O*)0)->x194==0x194)?1:-1];
typedef char layout_O_p198[((unsigned long)&((O*)0)->p198==0x198)?1:-1];
typedef char layout_O_x1C8[((unsigned long)&((O*)0)->x1C8==0x1c8)?1:-1];
typedef char layout_O_x1CC[((unsigned long)&((O*)0)->x1CC==0x1cc)?1:-1];
typedef char layout_O_g204[((unsigned long)&((O*)0)->g204==0x204)?1:-1];
typedef char layout_O_v210[((unsigned long)&((O*)0)->v210==0x210)?1:-1];
typedef char layout_VT_a_35_d[((unsigned long)&((VT*)0)->a[35].d==0x118)?1:-1];
typedef char layout_VT_a_35_pfn[((unsigned long)&((VT*)0)->a[35].pfn==0x11c)?1:-1];

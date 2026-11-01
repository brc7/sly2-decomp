struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
struct D { char pad[0x10]; int n10; };
struct C { char pad[0xDC]; struct D *pDC; };
struct O { char pad[0xE8]; struct C *pE8; };
struct T { char pad[0x120]; int n120; int pad2; int n128; };
struct S { char pad[0x10]; struct O *p10; int pad2; void *p18; char pad3[0x14]; struct T *p30; int pad4; int n38; int pad5; float g40; };
void UpdateBhv(struct S *p);
int FBhvpathlinkDone(struct S *p);
int func_0017EAE8(void *p);
int FBhvanimDone(void *p);
int FBhvpathlinkAtSegmentEnd(struct S *p);
void AdvanceBhvpathlink(struct S *p);
void SetBhvpathlinkState(struct S *p, int n);
int FBhvpathAtEnd(void *p);
void UpdateBhvpathlink(struct S *p) {
    UpdateBhv(p);
    if (FBhvpathlinkDone(p)) return;
    switch (p->n38) {
    case 1:
        if (func_0017EAE8(p->p18)) AdvanceBhvpathlink(p);
        break;
    case 0: {
        struct T *t = p->p30;
        switch (t->n120) {
        case 1:
            if (FBhvanimDone(p->p18)) {
                if (FBhvpathlinkAtSegmentEnd(p)) AdvanceBhvpathlink(p);
                else SetBhvpathlinkState(p, 1);
            }
            break;
        case 0:
            if (p->g40 < D_002DDED0.t) {
                SetBhvpathlinkState(p, 1);
            } else {
                struct C *c = p->p10->pE8;
                struct D *d = c ? c->pDC : 0;
                int n = d ? d->n10 : 0;
                if (n == t->n128 && FBhvpathAtEnd(p->p18)) AdvanceBhvpathlink(p);
            }
            break;
        }
        break;
    }
    }
}

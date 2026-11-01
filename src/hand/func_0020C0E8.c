struct O { char pad[0x208]; int n208; char pad2[0x498 - 0x20C]; void *p498; };
struct S { char pad[0x10]; int k; float t14; char pad2[0x34 - 0x18]; struct O *po; int h38; char pad3[0x50 - 0x3C]; float t50; float t54; char pad4[0x60 - 0x58]; int n60; };
extern struct { float t; float dt; } D_002DDED0;
extern float D_002C3DE0;
void func_0020C2F8(struct S *p, int k);
int func_00132B10(struct O *po, int n);
void func_0020D1A0(void *p);
int func_00131968(int h);

/** @sly2-annot addr=0x20C0E8
 * Purpose: leap state machine 1..3
 * Info: proposed name UpdateLeap (T1); TU tu_20B840
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/animation.md#tu_20b840-0x20b840-0x20ce28
 * @end-sly2-annot */
void func_0020C0E8(struct S *p)
{
    int k;
    do {
        k = p->k;
        switch (k) {
        case 1:
            if (p->po->n208 && p->t14 + 0.15f < D_002DDED0.t)
                func_0020C2F8(p, 3);
            else if (p->t54 <= D_002DDED0.t)
                func_0020C2F8(p, 2);
            break;
        case 2:
            if (p->po->n208 || D_002DDED0.t - p->t50 > D_002C3DE0)
                func_0020C2F8(p, 3);
            else if (p->t50 <= D_002DDED0.t) {
                if (p->n60)
                    func_0020C2F8(p, 3);
                else if (func_00132B10(p->po, 0x38))
                    func_0020D1A0(p->po->p498);
            }
            break;
        case 3:
            if (func_00131968(p->h38))
                func_0020C2F8(p, 4);
            break;
        }
    } while (k != p->k);
}

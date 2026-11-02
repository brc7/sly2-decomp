struct Y {
    int a0, a4, a8;
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual void v8();
    virtual void v9();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void v34();
    virtual void v35();
    virtual void v36();
    virtual void v37();
    virtual void v38();
    virtual void v39();
    virtual void v40();
    virtual void v41();
    virtual void v42();
    virtual void v43();
    virtual void v44();
    virtual void v45();
    virtual void v46();
    virtual void v47();
    virtual void v48();
    virtual void v49();
    virtual void v50();
    virtual void v51();
    virtual void v52();
    virtual void v53();
    virtual void v54();
    virtual void v55();
    virtual void v56();
    virtual void v57();
    virtual void v58();
    virtual void v59();
    virtual void v60();
    virtual void v61();
    virtual int F();
};
struct SL { Y *x8; char pad[0x3C]; };
struct X { int x0; int x4; SL a[2]; char pad[0x9C-0x88]; int x9c; char pad2[0xAC-0xA0]; X *xac; };
struct L { X *x0; L *x4; X **x8; };
struct EC { char pad[0x60]; Y *x60; L *x64; };
struct C4 { char pad[0xEC]; EC *xec; };
struct C8 { int x0; C4 *x4; };
struct P0 { char pad[0xEC]; EC *xec; };
struct P { P0 *x0; char pad[0x1C-4]; int x1c; char pad2[0xC8-0x20]; C8 *xc8; };
struct G;
extern G D_003DD490;
extern "C" void FreeSwXpList__FP2SWP2XP(G *g, X *x);
/** @sly2-annot addr=0x199298
 * Purpose: If kind 0x11, free the wall-run XP list (FreeSwXpList)
 * Info: proposed name FreeJtWallRunXps (T2); TU air
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638,
 *   docs/reference/functions/player.md#air-0x195950-0x19cbf8
 * @end-sly2-annot */
extern "C" void func_00199298(P *p)
{
    EC *ec;
    if (p->x1c != 0x11 || !(ec = p->xc8->x4->xec))
        return;
    X *head = 0;
    Y *y = ec->x60;
    int f = y->F();
    for (L *l = p->x0->xec->x64; l; l = l->x4) {
        X **pp = &l->x0;
        X *x;
        while ((x = *pp)) {
            Y *a;
            if (x->x9c == 11) {
                pp = &x->xac;
                continue;
            }
            { int i = p->x0 != (P0 *)x->a[0].x8; a = x->a[1 - i].x8; }
            if (f ? a->F() : a == y) {
                *pp = x->xac;
                x->xac = head;
                head = x;
            } else
                pp = &x->xac;
        }
        *l->x8 = l->x0;
    }
    FreeSwXpList__FP2SWP2XP(&D_003DD490, head);
}

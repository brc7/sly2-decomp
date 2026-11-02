// (Claude agent) func_0015CF30: UI menu state machine. func_002121E8 takes (p, r) with r = func_0015D5C8(p); the n12C==0 flag uses a shared "setf: f = 1" block (reorg fills both beql/bnel slots from it).
struct CRef { int m_tagk; int m_n; };
struct CRefG { int m_tagk; float m_g; };
struct X {
    char pad[0x110]; int n110; int n114; int n118; int n11C; int n120; int pad124; int n128; int n12C; float g130;
};
struct G { char pad[0x3B58]; void *a3B58[9]; void *a3B7C[9]; };
extern struct G D_003DD490;
extern char D_003D4A60[];
extern char D_003D9ED0[], D_002E6550[], D_003DA4F0[];
extern char D_002DFC00[];
extern char D_002F66B0[];
extern float D_002DDF0C;
extern int D_002BE248[];
extern int D_002BE26C;
extern int D_002BE270;
extern int D_002BE200[];
extern int D_002BE278[];
extern int D_002BE2A8[];
void func_00212C50(struct X *p);
int func_001D9880(void *p);
int func_0015D5C8(struct X *p);
int DySelectionJoy(void *p);
int GrfHandleJoyBtn(void *p, int n);
int DxSelectionJoy(void *p);
void func_0015D648(struct X *p, int a, int b);
void func_002581C0(void *pref, void *p, void *q);
void func_00129EC8(void *pref);
void func_0015D880(struct X *p, int n);
void func_001B3A48(void *p, int n);
void func_0022B490(struct G *pg, int a, int b, int c, int d, int e);
void *func_001FBB90(int n);
void func_001FB2F8(void *p, int n);
int func_0015D798(struct X *p);
int func_002121E8(struct X *p, int n);
void *PloFindSwObject__FP2SWi3OIDP2LO(struct G *pg, int kind, int oid, int n);
void SetExitExits(void *p, int n);
void func_002283D0(void *p);
int PrecWorldEntry(int n);

static __inline__ void PlaySound(int n)
{
    func_001FB2F8(func_001FBB90(n), 0);
}

/** @sly2-annot addr=0x15CF30
 * Purpose: Per-frame update of the WIDGET131 HUD element
 * Info: proposed name UpdateWidget131 (T2); class WIDGET131 slot16; TU tu_15CD68
 * Notes: Cid83->Widget131 (CID 0x83=131, vtable) | runtime5b: WIDGET131 episode menu mode 10 (S55) | T2:
 *   placeholder token 131
 * Confidence: low (draft) [T2 0.70] - evidence: runtime (save-state RAM); sources: r3_1539A8+runtime
 * See: docs/engine/game-progression.md#6-objective-counters-job-markers-and-the-episode-menu,
 *   docs/reference/functions/hud-ui.md#tu_15cd68-0x15cd68-0x15dc80
 * @end-sly2-annot */
void func_0015CF30(struct X *p)
{
    int r;
    func_00212C50(p);
    if (func_001D9880(D_003D9ED0))
        return;
    if (func_001D9880(D_002E6550))
        return;
    if (func_001D9880(D_003DA4F0))
        return;
    r = func_0015D5C8(p);
    switch (p->n11C) {
    case -1:
        return;
    case 2: {
        float g = DySelectionJoy(D_002DFC00);
        if (g < 0.0f && p->n110 <= 0) {
            PlaySound(0x36);
            return;
        }
        if (0.0f < g && p->n110 >= p->n118) {
            PlaySound(0x36);
            return;
        }
        if (g != 0.0f) {
            if (g < 0.0f)
                p->n114 = p->n110 - 1;
            else
                p->n114 = p->n110 + 1;
            func_0015D648(p, 0.0f < g ? 4 : 6, -1);
            return;
        }
        if (GrfHandleJoyBtn(D_002DFC00, 0x40)) {
            func_0015D648(p, 7, -1);
            return;
        }
        if (p->n110 && GrfHandleJoyBtn(D_002DFC00, 0x10)) {
            p->n114 = 0;
            func_0015D648(p, 6, -1);
            return;
        }
        switch (p->n12C) {
        case 0: {
            int c;
            int f = 0;
            int n = D_002BE248[p->n110];
            void *pv = D_003DD490.a3B7C[p->n110];
            if (pv) {
                struct CRef ref;
                func_002581C0(&ref, pv, D_003D4A60);
                c = ref.m_n;
                func_00129EC8(&ref);
                if (c >= 100)
                    goto setf;
            }
            if (D_002BE26C) {
            setf:
                f = 1;
            }
            if (n && f && 10.0f < D_002DDF0C - p->g130)
                func_0015D880(p, 1);
            return;
        }
        case 1:
            if (GrfHandleJoyBtn(D_002DFC00, 0x80)) {
                func_001B3A48(D_002F66B0, D_002BE248[p->n110]);
                func_0022B490(&D_003DD490, 0, 0x189, -1, 0, -1);
                PlaySound(0x34);
                return;
            }
            if (2.0f < D_002DDF0C - p->g130)
                func_0015D880(p, 0);
            return;
        }
        return;
    }
    case 8:
        switch (p->n120) {
        case -1:
            if (GrfHandleJoyBtn(D_002DFC00, 0x40))
                func_0015D648(p, 10, -1);
            else if (GrfHandleJoyBtn(D_002DFC00, 0x10))
                func_0015D648(p, 9, -1);
            return;
        case 0: {
            float g;
            if (GrfHandleJoyBtn(D_002DFC00, 0x40)) {
                D_002BE270 = p->n128;
                func_001B3A48(D_002F66B0, D_002BE200[p->n128]);
                func_0022B490(&D_003DD490, 0, 0x189, -1, 0, -1);
                PlaySound(0x34);
                return;
            }
            if (GrfHandleJoyBtn(D_002DFC00, 0x10)) {
                func_0015D648(p, 9, -1);
                return;
            }
            g = DxSelectionJoy(D_002DFC00);
            if (g < 0.0f && p->n128 <= 0) {
                PlaySound(0x36);
                return;
            }
            if (0.0f < g && p->n128 >= func_0015D798(p)) {
                PlaySound(0x36);
                return;
            }
            if (g != 0.0f) {
                if (g < 0.0f)
                    p->n128 = p->n128 - 1;
                else
                    p->n128 = p->n128 + 1;
                func_0015D648(p, 8, 0.0f < g ? 2 : 1);
            }
            return;
        }
        case 1:
        case 2:
            if (!func_002121E8(p, r))
                func_0015D648(p, 8, 0);
            return;
        }
        return;
    case 10:
        if (!func_002121E8(p, r)) {
            int f = 0;
            void *pv = D_003DD490.a3B58[p->n110];
            if (pv) {
                struct CRefG ref;
                float g;
                f = 1;
                func_002581C0(&ref, pv, D_003D4A60);
                g = ref.m_g;
                func_00129EC8(&ref);
                if (g != 0.0f)
                    f = 0;
            }
            if (f) {
                void *po = PloFindSwObject__FP2SWi3OIDP2LO(&D_003DD490, 5, D_002BE2A8[p->n110], 0);
                if (po) {
                    SetExitExits(po, 3);
                    func_002283D0(po);
                } else
                    f = 0;
            }
            if (!f) {
                int s = PrecWorldEntry(p->n110);
                func_0022B490(&D_003DD490, s, 0x189, -1, 0, 0x30A);
            }
            func_0015D648(p, -1, -1);
        }
        return;
    default:
        if (!func_002121E8(p, r)) {
            int k = D_002BE278[p->n11C];
            int n = -1;
            if (k == 8 && p->n110 == 0) {
                p->n128 = 0;
                n = 0;
            }
            func_0015D648(p, k, n);
        }
        return;
    }
}

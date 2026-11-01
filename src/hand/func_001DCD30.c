typedef struct VT { struct { short d; short pad; void (*pfn)(void *); } a[32]; } VT;
typedef struct { char pad[0xC]; VT *vt; char pad10[0x58 - 0x10]; int x58; } W;
typedef struct {
    char pad[0x10];
    void *x10;
    char pad14[0x24 - 0x14];
    W *pw;
    char pad28[0x40 - 0x28];
    int x40;
    char pad44[4];
    int c;
    char pad4c[0x114 - 0x4C];
    int x114;
} OBJ;
void func_001DA808(OBJ *);
int func_002127C8(void);
void func_00212A70(W *, int);
void func_00212A50(W *, int);
int func_00187D80(void *, int);
void func_001DD300(OBJ *, int);
void func_001DA858(OBJ *);
void func_001DD1F8(OBJ *);
/** @sly2-annot addr=0x1DCD30
 * Purpose: Job list per-frame (slot 7): wait for widget load; START/circle close; direction buttons toggle
 *   jobs page <-> tasks page
 * Info: proposed name UpdateJobList (T1); TU screen
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void func_001DCD30(OBJ *p)
{
    func_001DA808(p);
    if (p->x40 == -1)
        return;
    if (p->x10 == 0)
        return;
    if (p->pw->x58 == 0) {
        if (!func_002127C8())
            return;
        func_00212A70(p->pw, 1);
        func_00212A50(p->pw, 2);
        p->pw->vt->a[16].pfn((char *)p->pw + p->pw->vt->a[16].d);
    }
    if (func_00187D80(p->x10, 0x800)) {
        func_001DD300(p, -1);
        return;
    }
    switch (p->x40) {
    case 0:
        if (func_00187D80(p->x10, 0x10)) {
            func_001DD300(p, -1);
            return;
        }
        if (p->c && func_00187D80(p->x10, 0xF0)) {
            func_001DD300(p, 1);
            return;
        }
        break;
    case 1:
        if (!p->x114 && func_00187D80(p->x10, 0x10)) {
            func_001DD300(p, -1);
            return;
        }
        if (func_00187D80(p->x10, 0xF0)) {
            func_001DD300(p, 0);
            return;
        }
        break;
    }
    func_001DA858(p);
    func_001DD1F8(p);
}

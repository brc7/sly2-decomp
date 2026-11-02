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
void SetWidgetState(W *, int);
void func_00212A50(W *, int);
int GrfHandleJoyBtn(void *, int);
void SetJobListPage(OBJ *, int);
void UpdateScreenStickDir(OBJ *);
void UpdateJobListWidgets(OBJ *);
/** @sly2-annot addr=0x1DCD30
 * Purpose: Job list per-frame (slot 7): wait for widget load; START/circle close; direction buttons toggle
 *   jobs page <-> tasks page
 * Info: proposed name UpdateJobList (T1); TU screen
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void UpdateJobList(OBJ *p)
{
    func_001DA808(p);
    if (p->x40 == -1)
        return;
    if (p->x10 == 0)
        return;
    if (p->pw->x58 == 0) {
        if (!func_002127C8())
            return;
        SetWidgetState(p->pw, 1);
        func_00212A50(p->pw, 2);
        p->pw->vt->a[16].pfn((char *)p->pw + p->pw->vt->a[16].d);
    }
    if (GrfHandleJoyBtn(p->x10, 0x800)) {
        SetJobListPage(p, -1);
        return;
    }
    switch (p->x40) {
    case 0:
        if (GrfHandleJoyBtn(p->x10, 0x10)) {
            SetJobListPage(p, -1);
            return;
        }
        if (p->c && GrfHandleJoyBtn(p->x10, 0xF0)) {
            SetJobListPage(p, 1);
            return;
        }
        break;
    case 1:
        if (!p->x114 && GrfHandleJoyBtn(p->x10, 0x10)) {
            SetJobListPage(p, -1);
            return;
        }
        if (GrfHandleJoyBtn(p->x10, 0xF0)) {
            SetJobListPage(p, 0);
            return;
        }
        break;
    }
    UpdateScreenStickDir(p);
    UpdateJobListWidgets(p);
}

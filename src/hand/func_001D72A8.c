typedef struct { void *p; int fl; int n; } ENT;
typedef struct OBJ {
    char pad[0x180];
    int x180;
    int pad184;
    void *x188;
    int c;
    ENT ae[8];
} OBJ;
extern struct G D_003DD490;
extern char D_003DA150[];
extern char D_002DE2E0[];
extern int D_002DFBC0;
extern char D_002DFC00[];
extern char D_002E0D00[];
void UpdateSo(OBJ *);
int func_0014FD50(void *);
int DxSelectionJoy(void *);
int GrfHandleJoyBtn(void *, int);
void *func_001F07E8(void *);
OBJ *PloFindSwObject__FP2SWi3OIDP2LO(struct G *, int, int, OBJ *);
void func_001DC260(void *, int);
void func_001DFD58(OBJ *, int);
void func_001D77D0(OBJ *, int);
void func_001D72A8(OBJ *p)
{
    char *pad;
    int n;
    int i;
    ENT *pe;
    UpdateSo(p);
    if (p->x180 == 0)
        return;
    if (p->x180 != 1)
        return;
    if (p->x188 == 0)
        return;
    if (func_0014FD50(D_002DE2E0))
        return;
    if (D_002DFBC0 & 2)
        pad = D_002DFC00;
    else
        pad = D_002E0D00;
    n = DxSelectionJoy(pad);
    if (n == 0) {
        for (i = 0; ; i++) {
            OBJ *q;
            pe = &p->ae[i];
            if (i >= p->c)
                return;
            if (pe->p != func_001F07E8(p->x188))
                continue;
            if (!GrfHandleJoyBtn(pad, pe->fl))
                continue;
            if (pe->n == -1)
                continue;
            if (pe->n == 0x378) {
                func_001DC260(D_003DA150, 0);
                break;
            }
            q = PloFindSwObject__FP2SWi3OIDP2LO(&D_003DD490, 4, pe->n, p);
            if (q) {
                func_001DFD58(q, 0);
                return;
            }
        }
    } else {
        func_001D77D0(p, n);
    }
}

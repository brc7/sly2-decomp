struct FRAME {
    int a;
    FRAME();
    ~FRAME();
};
struct SO {
    char pad0[0x54];
    int x54;
    char pad58[0x5C - 0x58];
    int x5C;
    char pad60[0x6C - 0x60];
    int x6C;
    char pad70[0x7C - 0x70];
    SO *x7C;
};
struct E { SO *o; int a; int b; };
struct OBJ {
    char pad0[0x24];
    void *x24;
    char pad28[0x40 - 0x28];
    int x40;
    char pad44[0x48 - 0x44];
    int n;
    E ae[0x10];
    int x10C;
    SO *x110;
    int x114;
    int x118;
};
extern "C" {
void func_00213928(void *, int);
void func_002333B4(void *, int, unsigned int);
void SortTasksTopologically(int *, SO ***);
void func_002338C0(void *, int, int, int (*)(const void *, const void *));
int func_00165C28(const void *, const void *);
int func_00163430(SO *);
void func_002332B8(void *, void *, unsigned int);
void func_001DCE90(OBJ *, int);
}
/** @sly2-annot addr=0x1DCED8
 * Purpose: Sort tasks topologically then by CompareTasks; page 0 = jobs with state > 0, page 1 = tasks of the
 *   selected job up to the first unfinished one; keep the last 10/4
 * Info: proposed name RefreshJobListEntries (T1); TU screen
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
extern "C" void func_001DCED8(OBJ *p)
{
    for (int i = 0; i < p->n; i++) {
        E *e = &p->ae[i];
        if (e->a != -1) {
            func_00213928(p->x24, e->a);
            e->a = -1;
        }
        if (e->b != -1) {
            func_00213928(p->x24, e->b);
            e->b = -1;
        }
    }
    p->x118 = 0;
    p->n = 0;
    func_002333B4(p->ae, 0, p->n * sizeof(E));
    FRAME frame;
    int c;
    SO **aso;
    SortTasksTopologically(&c, &aso);
    func_002338C0(aso, c, 4, func_00165C28);
    switch (p->x40) {
    case 0: {
        SO *last = 0;
        for (int i = 0; i < c; i++) {
            SO *so = aso[i]->x7C;
            if (so && last != so) {
                last = so;
                if (func_00163430(so) > 0 && so->x6C != -1) {
                    E *e = &p->ae[p->n++];
                    e->o = so;
                    e->b = -1;
                    e->a = -1;
                    if (p->x110 == so)
                        p->x118 = e - p->ae;
                }
            }
        }
        break;
    }
    case 1: {
        int flag = 0;
        for (int i = 0; i < c; i++) {
            SO *so = aso[i];
            if (p->x110 == so->x7C && so->x5C != -1) {
                if (flag && !so->x54)
                    break;
                E *e = &p->ae[p->n++];
                e->o = so;
                e->b = -1;
                e->a = -1;
                if (so->x54 == 1)
                    flag = 1;
                if (!so->x54)
                    break;
            }
        }
        break;
    }
    }
    if (p->n > p->x10C) {
        func_002332B8(p->ae, &p->ae[p->n - p->x10C], p->x10C * sizeof(E));
        int d = p->n - p->x10C;
        if (d < p->x118)
            p->x118 -= d;
        else
            p->x118 = 0;
        p->n = p->x10C;
    }
    func_001DCE90(p, p->x118);
}

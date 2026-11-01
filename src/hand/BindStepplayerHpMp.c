struct VTE { short d; short pad; void (*pfn)(); };
struct P { char pad[0xC]; struct VTE *vt; char pad2[0xDFC - 0x10]; int nDFC; int *pnE00; char pad3[0x12FC - 0xE04]; int *pn12FC; int n1300; int pad4; int n1308; };
extern char D_002A4E18[];
extern char D_002A4E20[];
int *func_001D8C10(void *p, char *name, int n);
void SetStepplayerHp(struct P *p, int n);
void SetStepplayerMp(struct P *p, int n);
void BindStepplayerHpMp(struct P *p, int f)
{
    struct VTE *e = &p->vt[103];
    p->pnE00 = func_001D8C10(((void *(*)(void *))e->pfn)((char *)p + e->d), D_002A4E18, 0);
    {
        struct VTE *e2 = &p->vt[103];
        p->pn12FC = func_001D8C10(((void *(*)(void *))e2->pfn)((char *)p + e2->d), D_002A4E20, 0);
    }
    if (*p->pnE00 == 0 || f) {
        SetStepplayerHp(p, p->nDFC);
        SetStepplayerMp(p, p->n1300);
        p->n1308 = 0;
    }
}

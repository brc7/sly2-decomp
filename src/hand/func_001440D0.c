struct VTE { short d; short pad; void (*pfn)(); };
struct P { char pad[0xC]; struct VTE *vt; char pad2[0xDFC - 0x10]; int nDFC; int *pnE00; char pad3[0x12FC - 0xE04]; int *pn12FC; int n1300; int pad4; int n1308; };
extern char D_002A4E18[];
extern char D_002A4E20[];
int *func_001D8C10(void *p, char *name, int n);
void func_00144188(struct P *p, int n);
void func_001441C0(struct P *p, int n);
/** @sly2-annot addr=0x1440D0
 * Purpose: Bind hp/mp to save vars hpCur/mpCur under vt103 root (+0xE00, +0x12FC); if unset or forced, reset
 *   to max
 * Info: proposed name BindStepplayerHpMp (T1); TU stepplayer
 * Notes: runtime3: php -> save var hpCur
 * Confidence: high [T1 0.80] - evidence: runtime (save-state RAM); sources: r2b_rest+runtime
 * See: docs/engine/player.md#11-key-functions,
 *   docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638
 * @end-sly2-annot */
void func_001440D0(struct P *p, int f)
{
    struct VTE *e = &p->vt[103];
    p->pnE00 = func_001D8C10(((void *(*)(void *))e->pfn)((char *)p + e->d), D_002A4E18, 0);
    {
        struct VTE *e2 = &p->vt[103];
        p->pn12FC = func_001D8C10(((void *(*)(void *))e2->pfn)((char *)p + e2->d), D_002A4E20, 0);
    }
    if (*p->pnE00 == 0 || f) {
        func_00144188(p, p->nDFC);
        func_001441C0(p, p->n1300);
        p->n1308 = 0;
    }
}

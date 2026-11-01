struct VTE { short d; short pad; void (*pfn)(); };
struct O { char pad[0xC]; struct VTE *vt; };
struct CRef { int tagk; int tag; };
extern char D_002A4DB0[];
extern int D_003E1084;
void func_001D8A38(struct CRef *r, void *a, void *b, int c);
void func_00129EC8(struct CRef *r);
void func_00143AF0(struct O *p, int n);
void func_001432E0(struct O *p, int n);
/** @sly2-annot addr=0x143BD8
 * Purpose: On load: read oidBooty; if set either clear it (flag D_003E1084&0x40) or respawn the carried booty
 *   marker
 * Info: proposed name RestoreStepplayerBooty (T2); TU stepplayer
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638
 * @end-sly2-annot */
void func_00143BD8(struct O *p)
{
    struct CRef r;
    int n;
    struct VTE *e = &p->vt[103];
    func_001D8A38(&r, ((void *(*)(void *))e->pfn)((char *)p + e->d), D_002A4DB0, 0);
    n = r.tag;
    func_00129EC8(&r);
    if ((unsigned int)(n + 1) < 2) return;
    if (D_003E1084 & 0x40) func_00143AF0(p, n);
    else func_001432E0(p, n);
}

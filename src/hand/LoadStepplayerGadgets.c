struct VTE { short d; short pad; void (*pfn)(); };
struct E { int id; int a, b; };
struct P { char pad[0xC]; struct VTE *vt; char pad2[0x1180 - 0x10]; struct E ae[3]; };
struct CRef { int tagk; int tag; };
extern char D_002A4D50[];
extern char D_002A4D58[];
int sprintf(char *, const char *, ...);
void func_001D8A38(struct CRef *r, void *a, void *b, char *c, int d);
void func_00129EC8(struct CRef *r);
/** @sly2-annot addr=0x141EF8
 * Purpose: For i<3: GetSaveVar(vt103 root, 'apukCur', '%d' i) -> slot id; then vt105
 * Info: proposed name LoadStepplayerGadgets (T1); TU stepplayer
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638
 * @end-sly2-annot */
void LoadStepplayerGadgets(struct P *p)
{
    int i;
    for (i = 0; i < 3; i++) {
        char ach[16];
        struct CRef r;
        struct VTE *e;
        sprintf(ach, D_002A4D50, i);
        e = &p->vt[103];
        func_001D8A38(&r, ((void *(*)(void *))e->pfn)((char *)p + e->d), D_002A4D58, ach, 0);
        p->ae[i].id = r.tag;
        func_00129EC8(&r);
    }
    {
        struct VTE *e = &p->vt[105];
        ((void (*)(void *))e->pfn)((char *)p + e->d);
    }
}

struct Y { char pad[0x30]; void *x30; int x34; char pad2[0x38]; };
struct F { int c; struct Y *ay; char pad[0x10]; };
struct Q { char pad[8]; struct F *af; };
struct S { char pad[0xC]; char *pc; struct Q *pq; };
void func_00215960(void *p, struct Y *y, int a);
void func_0011A098(void *p, struct Y *y);
static inline void Draw(struct S *p, int n, struct Y *ay, int a)
{
    int k;
    for (k = n - 1; k >= 0; k--) {
        struct Y *y = &ay[k];
        if (y->x30)
            func_00215960(y->x30, y, a);
        else if (y->x34 == 4)
            func_0011A098(p->pc + 0xFC, y);
    }
}
/** @sly2-annot addr=0x21A998
 * Purpose: queue items in reverse order
 * Info: proposed name DrawSpriteItems (T2); TU hud
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
void func_0021A998(struct S *p, int i, int a)
{
    Draw(p, p->pq->af[i].c, p->pq->af[i].ay, a);
}

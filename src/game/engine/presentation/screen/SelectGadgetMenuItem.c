typedef struct {
    int key;
    int a;
    int b;
    int c;
    int cur;
    int pad[2];
    int flags;
} E;
typedef struct {
    char pad0[0x24];
    void *x24;
    char pad28[0x48 - 0x28];
    int key;
    int n;
    E *ae;
} OBJ;
void StopWidgetAnim(void *, int);
void PlayWidgetAnim(void *, int);
/** @sly2-annot addr=0x1DB390
 * Purpose: Select item key and switch each item's highlight anim (selected / acquired / unlocked)
 * Info: proposed name SelectGadgetMenuItem (T1); TU screen
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void SelectGadgetMenuItem(OBJ *p, int key)
{
    int i;
    p->key = key;
    for (i = 0; i < p->n; i++) {
        E *e = &p->ae[i];
        int v;
        if (e->key == key)
            v = e->c;
        else if (e->flags & 4)
            v = e->b;
        else if (e->flags & 2)
            v = e->a;
        else
            v = -1;
        if (v != e->cur) {
            if (p->x24 && e->cur != -1)
                StopWidgetAnim(p->x24, e->cur);
            e->cur = v;
            if (p->x24 && v != -1)
                PlayWidgetAnim(p->x24, v);
        }
    }
}

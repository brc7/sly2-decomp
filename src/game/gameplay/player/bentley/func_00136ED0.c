struct L { char pad[0x10b8]; void *f10b8; };
struct S { struct L *pl; int pad[2]; int fc; int f10; char pad2[0xe0 - 0x14]; int fe0; };
struct M { int id; int f4; int f8; };
void func_00131B10(int, struct L *, void *);
/** @sly2-annot addr=0x136ED0
 * Purpose: msg 9 kinds 0x16E/0x242 for the helper struct
 * Info: proposed name HandleBentleyHelperMessage (T2); TU bentley
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/player.md#bentley-0x134648-0x1378d8
 * @end-sly2-annot */
void func_00136ED0(struct S *p, int msg, struct M *pm) {
    if (msg == 9 && pm->id == p->fe0) {
        switch (pm->f8) {
        case 0x16e: func_00131B10(pm->id, p->pl, p->pl->f10b8); p->f10 = 1; break;
        case 0x242: p->fc = 1; break;
        }
    }
}

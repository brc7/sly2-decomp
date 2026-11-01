struct N { float t; int f4; int *pc; int fc; struct N *pnext; };
struct G { char pad[0x3d6c]; int c3d6c; };
struct S { char pad[0x39fc]; int pool39fc; };
extern struct G D_003DD490;
struct DL { struct N *head; struct N *tail; int ibDle; };
extern struct DL D_003E0E80;
struct N *PvAllocSlotheapClear(void *);
void InsertSlEntryAfter(struct DL *, struct N *, struct N *);
/** @sly2-annot addr=0x133040
 * Purpose: Allocate a scheduled-callback node (pool g_sw+0x39FC) with id (++g_sw+0x3D6C if anonymous), time
 *   t, insert into time-sorted list D_003E0E80
 * Info: proposed name ScheduleSwSpliceCallback (T1); TU basic
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r2b_rest
 * See: docs/engine/splice-scripting.md#5-objects-and-scripts,
 *   docs/engine/world-sw.md#6-deferred-callbacks-and-the-splice-schedule,
 *   docs/reference/functions/object-model.md#basic-0x132b00-0x1336f0
 * @end-sly2-annot */
struct N *ScheduleSwSpliceCallback(struct S *p, int a, int *pc, float t) {
    struct N *pn;
    struct N *pnPrev;
    struct N *pnCur;
    (*pc)++;
    pn = PvAllocSlotheapClear(&p->pool39fc);
    pn->f4 = a;
    if (!a) pn->fc = ++D_003DD490.c3d6c;
    else pn->fc = -1;
    pn->pc = pc;
    pn->t = t;
    pn->pnext = 0;
    pnPrev = 0;
    for (pnCur = D_003E0E80.head; pnCur; pnCur = pnCur->pnext) {
        if (pnCur->t > pn->t) break;
        pnPrev = pnCur;
    }
    InsertSlEntryAfter(&D_003E0E80, pnPrev, pn);
    return pn;
}

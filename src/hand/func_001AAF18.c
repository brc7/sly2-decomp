struct PMF { short delta; short pad; int (*pfn)(void *); };
struct S { int x0; int x4; struct PMF pmf; int x10; };
extern char D_002E65E0[];
extern int D_004425D8[];
void func_00234A60(int);
void func_00234A40(int);
void func_00234860(void);
void *func_002333B4(void *, int, unsigned int);
/** @sly2-annot addr=0x1AAF18
 * Purpose: Memcard worker thread body: WaitSema, call the queued SAVE member-function job on g_save, clear
 *   job, signal, ExitThread
 * Info: proposed name MemcardThreadMain (T1); TU memcard
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r4_17F090
 * See: docs/engine/main-loop-startup.md#6-threads,
 *   docs/engine/save-memcard.md#2-memory-card-memcardcpp-0x1aa978-0x1ae130,
 *   docs/engine/save-memcard.md#5-key-functions
 * @end-sly2-annot */
void func_001AAF18(struct S *s)
{
    func_00234A60(s->x0);
    s->x10 = s->pmf.pfn(D_002E65E0 + s->pmf.delta);
    func_002333B4(&s->pmf, 0, sizeof(s->pmf));
    s->x4 = -1;
    D_004425D8[0] = -1;
    func_00234A40(s->x0);
    func_00234860();
}

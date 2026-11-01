typedef int qword __attribute__((mode(TI)));
struct O { char pad[0x110]; int k; };
struct CM { char pad[0x90]; qword pos; };
extern char D_003DD490[];
int func_0015E9C8(void *g, int a, int b, int c, int n, struct O **ap);
void func_002273F0(struct O *po, qword pos);
void func_002338C0(void *base, int n, int cb, int (*cmp)(const void *, const void *));
int func_0014E000(const void *a, const void *b);
void func_0014E148(struct CM *p, struct O *po);
/** @sly2-annot addr=0x14E078
 * Purpose: Gathers up to 32 zone objects (0x15E9C8 query), scores each against cm pos (0x2273F0), sorts, and
 *   selects the best via SetCmVisZone if its kind==1
 * Info: proposed name UpdateCmVisZone (T1); TU cm
 * Confidence: high [T1 0.80] - evidence: semantic review; sources: r2_131F70
 * See: docs/engine/camera.md#5-key-functions, docs/reference/functions/camera.md#cm-0x14d3d8-0x14fac8
 * @end-sly2-annot */
void func_0014E078(struct CM *p)
{
    struct O *apo[32];
    qword pos = p->pos;
    int n = func_0015E9C8(D_003DD490, 0x105, 5, 0, 0x20, apo);
    int i;
    for (i = 0; i < n; i++)
        func_002273F0(apo[i], pos);
    func_002338C0(apo, n, 4, func_0014E000);
    if (n > 0 && apo[0]->k == 1)
        func_0014E148(p, apo[0]);
}

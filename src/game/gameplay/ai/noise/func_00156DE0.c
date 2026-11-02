struct VTE { short d; short pad; void (*pfn)(); };
struct O { int a[3]; struct VTE *vt; };
struct S { char pad[0xE0]; int c; struct O *ao[1]; };
int func_00156870(struct O *o);
void *memmove(void *dst, void *src, int cb);
/** @sly2-annot addr=0x156DE0
 * Purpose: Releases removed (or, if f, all) noises from a listener's array
 * Info: proposed name DropCid47Noises (T2); class CID47; TU noise
 * Notes: Cid2F->Cid47 (CID 0x2F=47, hex-literal)
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r3b_rest
 * See: docs/reference/functions/guards-ai.md#noise-0x1564e8-0x157200
 * @end-sly2-annot */
void func_00156DE0(struct S *p, int f)
{
    int i;
    for (i = 0; i < p->c;) {
        struct O **ppo = &p->ao[i];
        struct O *o = *ppo;
        if (f || func_00156870(o)) {
            struct VTE *e = &o->vt[2];
            e->pfn((char *)o + e->d);
            memmove(ppo, ppo + 1, (p->c - i) * sizeof(struct O *) - sizeof(struct O *));
            p->c--;
        } else {
            i++;
        }
    }
}

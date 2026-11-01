struct VTE { short d; short p; void *pfn; };
struct VT { struct VTE a[9]; };
struct P { char pad[0xC]; struct VT *pvt; };
void func_001FB658(struct P *, int, int);
/** @sly2-annot addr=0x1FC018
 * Purpose: vt15: on becoming audible restart (vt7); on silence request stop (loopers keep params)
 * Info: proposed name SetBanksndCulled (T2); class BANKSND; TU sound
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void func_001FC018(struct P *p, int f)
{
    if (!f) ((void (*)(void *))p->pvt->a[7].pfn)((char *)p + p->pvt->a[7].d);
    else {
        int n = 0x1FF;
        func_001FB658(p, ((int (*)(void *))p->pvt->a[8].pfn)((char *)p + p->pvt->a[8].d) == 2 ? n : 0x3FF, 8);
    }
}

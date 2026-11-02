struct VTE { short d; short pad; void (*pfn)(void *); };
struct S { int a[3]; struct VTE *vt; char pad[0x30]; int x40; char pad2[0xE]; short x52; };
/** @sly2-annot addr=0x1FBAF8
 * Purpose: 989snd async callback: store voice handle (+0x40), clear busy (+0x52), Decref
 * Info: proposed name OnBanksndPlayStarted (T2); TU sound
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void func_001FBAF8(int x, long l) { struct S *p = (struct S *)(int)l; p->x40 = x; p->x52 = 0; p->vt[2].pfn((char *)p + p->vt[2].d); }

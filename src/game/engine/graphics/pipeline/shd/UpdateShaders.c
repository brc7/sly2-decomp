struct VTE { short d; short pad; void (*pf)(void *); };
struct O { char pad[0xC]; struct VTE *vt; char pad2[0x30-0x10]; void *pChild; char pad3[0x40-0x34]; struct O *pNext; };
extern struct O *D_003E159C;
int FUpdatableSaa__FP3SAA(struct O *p);
/** @sly2-annot addr=0x1EC968
 * Purpose: For each SAA in D_003E159C: if FUpdatableSaa call its vt8 (Update)
 * Info: proposed name UpdateShaders (T1); TU shd
 * Confidence: mid (draft) [T1 0.65] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/rendering.md#shd-0x1ea568-0x1ed7a8
 * @end-sly2-annot */
void UpdateShaders(void)
{
    int unused[4];
    struct O *p;
    for (p = D_003E159C; p && !p->pChild; p = p->pNext) {
        if (FUpdatableSaa__FP3SAA(p)) p->vt[8].pf((char *)p + p->vt[8].d);
    }
}

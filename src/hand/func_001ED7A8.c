struct VTE { short d; short pad; void (*pfn)(void *, char *); };
struct O { int pad[3]; struct VTE *vt; char pad2[0x30 - 0x10]; void *p30; };
struct REG { int a[4]; struct O *(*pfnNew)(void); };
extern struct REG D_002BAAE8[];
int func_00117C20(void *pbis);
void func_00117968(void *pbis, int cb, char *buf);
/** @sly2-annot addr=0x1ED7A8
 * Purpose: Read an SAA class index, construct it via the class table factory (D_002BAAE8[cid].pfnNew), set
 *   owner (+0x30), read 0x20-byte SAAF and call vt7 Init(psaa, psaaf)
 * Info: proposed name PsaaLoadFromBrx (T1); TU shdanim
 * Notes: level_parser CONFIRMED-parse: payload layout followed by the parser, 100%% of bytes in 4 levels |
 *   textures format doc cites this function by this name (CONFIRMED(code) in the doc)
 * Confidence: high [T1 0.85] - evidence: static analysis; sources:
 *   r6b_rest+parse-verified+format-doc:textures+verdicts_followups
 * See: docs/engine/rendering.md#12-source-files, docs/engine/rendering.md#13-key-functions,
 *   docs/formats/level-objects.md#2-per-world-entry-header-loop-body-of-loadswfrombrx
 * @end-sly2-annot */
struct O *func_001ED7A8(void *pv, void *pbis)
{
    char buf[0x20];
    int i = func_00117C20(pbis);
    struct O *p;
    if (i == -1) return 0;
    p = (D_002BAAE8 + i)->pfnNew();
    p->p30 = pv;
    func_00117968(pbis, 0x20, buf);
    p->vt[7].pfn((char *)p + p->vt[7].d, buf);
    return p;
}

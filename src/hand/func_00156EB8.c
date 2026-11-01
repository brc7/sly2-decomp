struct VTE { short d; short pad; void (*pfn)(); };
struct O { int a[3]; struct VTE *vt; char pad[0x58 - 0x10]; struct O *pNext; };
struct S { int a[3]; struct VTE *vt; char pad[0xE0 - 0x10]; unsigned int c; struct O *ao[8]; };
extern struct O *D_003E148C;
void func_001A5E28(struct S *p);
void func_00156DE0(struct S *p, int f);
int func_00156FE8(struct S *p, struct O *o);
/** @sly2-annot addr=0x156EB8
 * Purpose: LO::Update then 0x156DE0(p,0) and scans list D_003E148C for objects not in its own set
 * Info: proposed name UpdateCid47 (T2); class CID47 slot16; TU noise
 * Notes: Cid2F->Cid47 (CID 0x2F=47, hex-literal)
 * Confidence: low (draft) [T2 0.65] - evidence: static analysis; sources: r3_1539A8
 * See: docs/reference/functions/guards-ai.md#noise-0x1564e8-0x157200
 * @end-sly2-annot */
void func_00156EB8(struct S *p)
{
    struct O *o;
    func_001A5E28(p);
    func_00156DE0(p, 0);
    for (o = D_003E148C; o; o = o->pNext) {
        int i;
        for (i = 0; i < (int)p->c; i++)
            if (o == p->ao[i])
                break;
        if (i < (int)p->c)
            continue;
        if (!func_00156FE8(p, o))
            continue;
        if (p->c < 8) {
            struct VTE *e = &o->vt[1];
            e->pfn((char *)o + e->d);
            p->ao[p->c++] = o;
            e = &p->vt[3];
            e->pfn((char *)p + e->d, 5, 0, 0, 0, 0);
        }
        return;
    }
}

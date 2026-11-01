struct Q { int a; int b; unsigned short c; unsigned short d; };
struct S { char pad[0x14]; int a; int b; int d; int c; char pad2[0x30 - 0x24]; int x30; int dl34; };
extern struct DL D_003E159C;
void func_0011A050(void *pdl, int ib);
void PrependSlEntry(struct DL *pdl, struct S *p);
void func_0011A070(struct DL *pdl, struct S *p);
/** @sly2-annot addr=0x1ED8E0
 * Purpose: SAA (CID 0x8A) vt7 Init
 * Info: proposed name InitSaa (T1); class SAA slot7; TU shdanim
 * Notes: names_v2 v1 proposed PsaaLoadFromBrx__FP18CBinaryInputStream (0.001)
 * Confidence: mid (draft) [T1 0.60] - evidence: static naming pass; sources: r6_1D5150
 * See: docs/reference/functions/rendering.md#shdanim-0x1ed7a8-0x1ee988
 * @end-sly2-annot */
void func_001ED8E0(struct S *p, struct Q *q)
{
    p->a = q->a;
    p->b = q->b;
    p->c = q->c;
    p->d = q->d;
    func_0011A050(&p->dl34, 0x10);
    if (p->x30 == 0) PrependSlEntry(&D_003E159C, p);
    else func_0011A070(&D_003E159C, p);
}

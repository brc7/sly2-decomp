typedef struct VT { struct { short d; short pad; int (*pfn)(void *); } a[64]; } VT;
typedef struct { char pad[0xC]; VT *vt; } VOBJ;
typedef struct { char pad[0x60]; VOBJ *p; } C;
typedef struct { char pad[0xEC]; C *pc; } B;
typedef struct { int pad; B *pb; } A;
typedef struct { char a[8]; char b[0x58]; } ENT;
typedef struct {
    char pad[0x190];
    int c;
    char pad2[0xC];
    ENT ae[8];
} OBJ;
void func_001F5990(void);
void func_001D3450(OBJ *, ENT *, A *, int, int, int, void *);
/** @sly2-annot addr=0x1D3E48
 * Purpose: CID 77 vt60: SO base (no-op) then wheel contacts (0x1D3450) for every wheel if the other object
 *   passes its vt62 test
 * Info: proposed name AddRobottankCustomXps (T1); class ROBOTTANK slot60; TU tu_1D1370
 * Notes: class identity from whole_game WE names/object strings: AddRcCarCustomXps -> AddRobottankCustomXps
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources:
 *   r5b_rest+verdicts_B+whole_game(class identity)
 * See: docs/engine/gameplay-objects.md#key-functions,
 *   docs/reference/functions/gameplay-objects.md#tu_1d1370-0x1d1370-0x1d5150
 * @end-sly2-annot */
void func_001D3E48(OBJ *p, A *pa, int x, int y, int z)
{
    int i;
    VOBJ *po;
    func_001F5990();
    po = pa->pb->pc->p;
    if (po->vt->a[62].pfn((char *)po + po->vt->a[62].d)) {
        for (i = 0; i < p->c; i++)
            func_001D3450(p, &p->ae[i], pa, x, y, z, p->ae[i].b);
    }
}

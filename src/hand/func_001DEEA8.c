// Remove contacts linking the owner to the selected object.
typedef struct { short kind,index; union { short item; int value; } payload; } PAIR;
typedef struct OBJ OBJ;
typedef struct EP { void *p; char pad[0x3C]; } EP;
typedef struct LNK {
    char pad[8];
    EP aep[2];
    char pad88[0xAC - 0x88];
    struct LNK *pNext;
} LNK;
typedef struct { char pad[0x148]; LNK *pHead; char pad14c[4]; } ENT;
struct OBJ {
    char pad[0x198];
    int c;
    ENT *ae;
};
extern struct G D_003DD490;
extern PAIR D_002A7AC0;
void func_001F5988(void);
void func_001A7E10(struct G *, LNK *);
void func_00149AB8(struct G *, OBJ *, PAIR, int);
/** @sly2-annot addr=0x1DEEA8
 * Purpose: LASEN vt54: drop beam contact links that reference object x and free them
 * Info: proposed name RemoveLasenContacts (T2); class LASEN slot54; TU sensor
 * Notes: agent qualifier: (vt54)
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/gameplay-objects.md#sensor-0x1de2a0-0x1dfad8
 * @end-sly2-annot */
void func_001DEEA8(OBJ *p, void *x)
{
    LNK *list = 0;
    int i;
    func_001F5988();
    for (i = 0; i < p->c; i++) {
        LNK **pp = &p->ae[i].pHead;
        LNK *q;
        while ((q = *pp) != 0) {
            int side = q->aep[0].p == p;
            if (x == ((EP *)((char *)q+((unsigned int)side<<6)+8))->p) {
                *pp = q->pNext;
                q->pNext = list;
                list = q;
            } else {
                pp = &q->pNext;
            }
        }
    }
    func_001A7E10(&D_003DD490, list);
    func_00149AB8(&D_003DD490, p, D_002A7AC0, 0);
}

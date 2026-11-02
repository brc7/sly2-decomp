extern char D_003DD490[];
extern int D_002C3E70[4];
struct L { float a; int b; int c; float d; int e[4]; };
struct E { int x; int pad[2]; };
struct S { char pad[0x3BA0]; struct E ae[4]; char pad2[0x4764 - 0x3BD0]; int n4764; int n4768; };
void *func_002333B4(void *p, int c, int n);
int PloFindSwObject__FP2SWi3OIDP2LO(void *p, int a, int b, int c);
void func_00213C68(int x, struct L *pl);
int func_001C7D08(void);
int func_001C7D00(void);
/** @sly2-annot addr=0x20F710
 * Purpose: Find 4 HUD objects (kind 0x405, OIDs 0x371/0x36D/0x36E/0x36F), init them, record two counters
 * Info: proposed name BindSwTimerHud (T2); TU sw
 * Notes: names_v2 v1 proposed LoadBtn__FP3BTNP3ALO (0.003)
 * Confidence: low (draft) [T2 0.35] - evidence: static naming pass; sources: r7_1F8DD8
 * See: docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
void func_0020F710(struct S *p)
{
    struct L l;
    int i;
    int *pn = D_002C3E70;
    func_002333B4(&l, 0, sizeof(l));
    l.d = 1.0f;
    l.a = 1.0f;
    for (i = 0; i < 4; i++) {
        int x = PloFindSwObject__FP2SWi3OIDP2LO(D_003DD490, 0x405, D_002C3E70[i], 0);
        if (x) {
            func_00213C68(x, &l);
            p->ae[i].x = x;
        }
    }
    p->n4764 = func_001C7D08();
    p->n4768 = func_001C7D00();
}

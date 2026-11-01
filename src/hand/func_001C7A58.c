struct E { int id; char pad[0x18]; int c; void *a[3]; };
struct S { char pad[0x23C]; int c; struct E *ae; struct E *pe; };
void func_001F6A68(struct S *p);
void func_001F6978(struct S *p, void *pv);
/** @sly2-annot addr=0x1C7A58
 * Purpose: helper used by 0x1C2DC8 (calls SO 0x1F6A68/0x1F6978)
 * Info: proposed name Clue_7A58 (T2); TU prize
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r5_1AE130
 * See: docs/reference/functions/game-progression.md#prize-0x1c4e50-0x1c8cc0
 * @end-sly2-annot */
void func_001C7A58(struct S *p, int id)
{
    int i, n;
    {
    int j; struct E *pe;
    n = p->c;
    for (j = 0; ; j++) {
        if (j >= n) {
            p->pe = p->ae;
            break;
        }
        pe = &p->ae[j];
        if (id == pe->id) {
            p->pe = pe;
            break;
        }
    }
    }
    func_001F6A68(p);
    for (i = 0; i < p->pe->c; i++) func_001F6978(p, p->pe->a[i]);
}

struct E { int a; int b; void *o; int k; float ag[3]; };
struct S { char pad[0x288]; float g288; char pad2[0x2A8 - 0x28C]; int c; struct E ae[8]; };
int func_001F0638(void *o);
void func_001F1448(int a, int b, int c, int d, void **ppo);
int func_00152680(int j);
void func_001F0660(void *o, int n);
/** @sly2-annot addr=0x152B58
 * Purpose: Per bound grade object: if finished unbind; else show grade j (oid 0x626+j) for the smoothed score
 * Info: proposed name UpdateRhythmMgrGrades (T2); TU rhythm
 * Confidence: low (draft) [T2 0.40] - evidence: semantic review; sources: r2_131F70
 * See: docs/reference/functions/gameplay-objects.md#rhythm-0x1520c8-0x1539a8
 * @end-sly2-annot */
void func_00152B58(struct S *p)
{
    int i, j;
    for (i = 0; i < p->c; i++) {
        struct E *pe = &p->ae[i];
        if (!pe->o)
            continue;
        if (func_001F0638(pe->o)) {
            func_001F1448(0, 0, -1, 0, &pe->o);
            pe->k = -1;
        } else {
            for (j = 0; j < 3; j++) {
                if (pe->ag[j] <= p->g288) {
                    func_001F0660(pe->o, func_00152680(j));
                    pe->k = j;
                    break;
                }
            }
        }
    }
}

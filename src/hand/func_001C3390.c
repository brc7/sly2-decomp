typedef int qword __attribute__((mode(TI)));
struct S { char pad[0x1AC]; int n1ac; char pad2[0x10]; qword q1c0; int n1d0; int n1d4; };
extern qword D_002A2DA0;
void func_001E5D80(struct S *p);
void func_00182C70(void *p);
/** @sly2-annot addr=0x1C3390
 * Purpose: PIPE::Init (vt7): InitShape + GRABSPOT init (+0x190); defaults
 * Info: proposed name InitPipe (T1); class PIPE slot7; TU pipe
 * Notes: names_v2 v1 proposed FIsJtSoundBase__FP2JT (0.044)
 * Confidence: high [T1 0.85] - evidence: static naming pass; sources: r5_1AE130+verdicts_tu_r3r5
 * See: docs/engine/gameplay-objects.md#source-files, docs/engine/player.md#10-source-files,
 *   docs/engine/player.md#11-key-functions
 * @end-sly2-annot */
void func_001C3390(struct S *p)
{
    func_001E5D80(p);
    func_00182C70((char *)p + 0x190);
    /*B*/p->q1c0 = D_002A2DA0;
    p->n1d4 = 9;
    p->n1d0 = 8;
    p->n1ac = 1;/*E*/
}

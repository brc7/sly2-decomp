typedef int qword __attribute__((mode(TI)));
typedef struct { float x, y, z, w; } VEC;
extern float D_002BE910;
qword func_001A62E8(void *a, void *b);
/** @sly2-annot addr=0x184F98
 * Purpose: GRABSPOT slot C: owner position + D_002BE910 in z
 * Info: proposed name GetHndGrabPos (T2); class HND; TU hnd
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#hnd-0x184f50-0x185130
 * @end-sly2-annot */
void func_00184F98(void *a, void *b, VEC *p)
{
    *(qword *)p = func_001A62E8(a, b);
    p->z += D_002BE910;
}

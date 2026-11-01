typedef int qword __attribute__((mode(TI)));
typedef struct R { char pad[0x1C]; float g; qword v; qword w; } R;
R *func_0012DB50(int k, void *a, void *b, void *c, int d);
void func_00230018(void *a, void *b, void *p);
/** @sly2-annot addr=0x12DE58
 * Purpose: Create act variant and attach
 * Info: proposed name PactNewE (T2); TU act
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/animation.md#act-0x12b3b0-0x12e600,
 *   docs/reference/functions/player.md#murray-0x1b4728-0x1b9f80
 * @end-sly2-annot */
R *func_0012DE58(void *a, void *b, void *c, float g, qword v, qword w)
{
    R *p = func_0012DB50(8, a, b, c, 0x11);
    p->g = g;
    p->v = v;
    p->w = w;
    func_00230018(a, b, p);
    return p;
}

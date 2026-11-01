typedef struct T { char pad[0x70]; int a; int b; } T;
/** @sly2-annot addr=0x12D210
 * Purpose: vtable 0x2CA7F0 slot 1 (12-byte)
 * Info: proposed name ActUnk_2CA7F0_1 (T2); TU act
 * Confidence: low (draft) [T2 0.20] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/animation.md#act-0x12b3b0-0x12e600
 * @end-sly2-annot */
void func_0012D210(T *p)
{
    p->a = 0;
    p->b = 0;
}

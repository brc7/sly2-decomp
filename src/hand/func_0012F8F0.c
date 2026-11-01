typedef struct S8 { int a, b; } S8;
extern S8 D_002A46C8;
extern char D_003DD490[];
void func_001A5C48(void *p);
void func_00149A50(void *a, void *b, S8 c, int d);
typedef struct T { char pad[0xB0]; int f; } T;
/** @sly2-annot addr=0x12F8F0
 * Purpose: PostLoLoad (0x1A5C48) and, if flagged, registers the load callback (0x149A50)
 * Info: proposed name PostAsegLoad (T1); class ASEG slot15; TU aseg
 * Confidence: mid (draft) [T1 0.65] - evidence: static analysis; sources: r1_116918
 * See: docs/engine/animation.md#1-aseg-and-asega-asegcpp-0x12f248-asegacpp-0x1308a8,
 *   docs/reference/functions/animation.md#aseg-0x12f248-0x1308a8
 * @end-sly2-annot */
void func_0012F8F0(T *t)
{
    func_001A5C48(t);
    if (t->f)
        func_00149A50(D_003DD490, t, D_002A46C8, t->f);
}

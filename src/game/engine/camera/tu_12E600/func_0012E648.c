extern char D_002DDF40[];
void PostLoLoad(void *p);
void SetCmPolicy(void *a, int b, void *c, int d, void *e);
typedef struct T { char pad[0x28]; unsigned long fl; char pad2[0x124 - 0x30]; int f; } T;
/** @sly2-annot addr=0x12E648
 * Purpose: slot 15: PostLoLoad then SetCmPolicy (0x14E328) with its embedded camera policy
 * Info: proposed name PostCid44Load (T2); class CID44 slot15; TU tu_12E600
 * Confidence: low (draft) [T2 0.55] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/camera.md#tu_12e600-0x12e600-0x12eb48
 * @end-sly2-annot */
void func_0012E648(T *t)
{
    PostLoLoad(t);
    t->f = 1;
    if ((int)(t->fl >> 60) & 1)
        SetCmPolicy(D_002DDF40, 10, D_002DDF40 + 0x4A8, 0, t);
}

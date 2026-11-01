typedef unsigned long ulong;
struct Q { char pad[0x68]; ulong t; };
struct S { int c; struct Q *a[8]; };
struct P { char pad[0x204]; struct S s; };
/** @sly2-annot addr=0x1FE3F8
 * Purpose: Play time of top music stream in seconds (ticks/294912000)
 * Info: proposed name TMusicCur (T1); TU sound
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/engine/minigames.md#7-dance-dancer-0x44--rhythm-manager-cid121-0x79-rhythm-tu-0x1520c8-0x1539a8-ccode,
 *   docs/engine/sound.md#2-sound-instances, docs/guides/port-plan.md#64-sound--coreaudio-k12
 * @end-sly2-annot */
float func_001FE3F8(struct P *p)
{
    struct Q *q;
    if (p->s.c > 0) { q = *(p->s.a + p->s.c - 1); }
    else q = 0;
    if (!q) return 0.0f;
    return (float)q->t * (1.0f / 294912000.0f);
}

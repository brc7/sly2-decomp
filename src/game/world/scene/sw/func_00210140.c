struct E;
struct O { char pad[0x40]; struct E *e; };
struct E { unsigned int grf; struct O *o; };
struct G { char pad[0x3F14]; int count; int dead; int *index; struct E *array; };
/** @sly2-annot addr=0x210140
 * Purpose: Append LO to SW update array (back-pointer at LO+0x40), compacting when >=4096 or >10% dead
 * Info: proposed name AddSwUpdateLo (T2); TU sw
 * Notes: names_v2 v1 proposed TriggerBtn__FP3BTNii (0.001)
 * Confidence: low (draft) [T2 0.45] - evidence: static naming pass; sources: r7_1F8DD8
 * See: docs/engine/object-model.md#9-key-functions,
 *   docs/reference/functions/object-model.md#lo-0x1a5270-0x1a7518,
 *   docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
void func_00210140(struct G *g, struct O *o)
{
    struct E *e;
    if ((unsigned int)o->e & 0x40)
        return;
    if (g->count >= 0x1000 || (float)g->dead / (float)g->count > 0.1f) {
        struct E *end = g->array + g->count;
        struct E *tracked;
        struct E *write;
        struct E *read;
        if (g->index)
            tracked = g->array + *g->index;
        else
            tracked = 0;
        write = g->array;
        for (read = g->array; read < end; read++) {
            if (read == tracked)
                tracked = write;
            if (read->o) {
                read->o->e = write;
                *write++ = *read;
            }
        }
        g->dead = 0;
        g->count = write - g->array;
        if (g->index)
            *g->index = tracked - g->array;
    }
    e = &g->array[g->count++];
    e->o = o;
    e->grf = (unsigned int)o->e;
    o->e = e;
}

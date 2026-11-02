typedef struct N { int v; struct N *next; } N;
typedef struct O { char pad[0x64]; N *p; } O;
/** @sly2-annot addr=0x1974C8
 * Purpose: Walk a linked list and return the first non-null payload
 * Info: proposed name PvFirstNonNull (T2); TU air
 * Confidence: low (draft) [T2 0.20] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#air-0x195950-0x19cbf8
 * @end-sly2-annot */
int func_001974C8(O *o)
{
    N *p = o->p;
    while (p) {
        if (p->v)
            return p->v;
        p = p->next;
    }
    return 0;
}

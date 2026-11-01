typedef int qword __attribute__((mode(TI)));
typedef struct E { union { qword q[12]; struct { char pad[0xAC]; struct E *next; } s; } u; } E;
struct S { char pad[0x3D40]; char pool[1]; };
E *PvAllocSlotheapClear(void *pool);
/** @sly2-annot addr=0x1AA020
 * Purpose: Duplicate an XP list into new slotheap nodes
 * Info: proposed name PxpCloneList (T2); TU mark
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/physics-collision.md#mark-0x1a7c18-0x1aa978
 * @end-sly2-annot */
E *func_001AA020(struct S *s, E *src)
{
    E *head;
    E **pp;
    if (!src)
        return 0;
    head = 0;
    pp = &head;
    do {
        E *n = PvAllocSlotheapClear(s->pool);
        *n = *src;
        *pp = n;
        src = src->u.s.next;
        pp = &n->u.s.next;
    } while (src);
    return head;
}

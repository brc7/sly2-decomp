typedef struct T { char pad[0x18]; int x18; } T;
typedef struct M { int a; int b; int c; } M;
typedef struct S { char pad[0x30]; T *pt; char pad2[0x24]; int x58; int x5C; } S;
void HandleBasicMessage(S *p, int a, M *m);
/** @sly2-annot addr=0x1E1BC8
 * Purpose: BASIC HandleMessage then: msg 10 from psmaCur (+0x58) reaching the ApplySm target state ->
 *   fSmaReached(+0x5C)=1
 * Info: proposed name HandleJsgMessage (T1); class JSG; TU relay
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/cutscenes.md#relay-0x1dfad8-0x1e5d80
 * @end-sly2-annot */
void func_001E1BC8(S *p, int a, M *m)
{
    HandleBasicMessage(p, a, m);
    if (a == 10 && m->a == p->x58 && m->c == p->pt->x18) p->x5C = 1;
}

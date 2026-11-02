struct S { char pad[0xF80]; int state; int arg; };
void func_00204F78(struct S *p, int a, int b);
/** @sly2-annot addr=0x20B210
 * Purpose: Unlink attachment (LinkStepAttachment f=0) and clear +0xF80/+0xF84
 * Info: proposed name DetachStepguard (T2); TU stepguard
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r7b_guard
 * See: docs/engine/guards-ai.md#9-key-functions,
 *   docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
void func_0020B210(struct S *p)
{
    switch (p->state) {
    case 0:
        func_00204F78(p, p->arg, 0);
        break;
    case 1:
        func_00204F78(p, p->arg, 0);
        break;
    case 2:
        func_00204F78(p, p->arg, 0);
        break;
    case 3:
        func_00204F78(p, p->arg, 0);
        break;
    }
    p->arg = 0;
    p->state = -1;
}

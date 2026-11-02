struct T { char pad[0x28]; unsigned long fl; };
struct U { char pad[0xF68]; struct T *pt; };
struct S { char pad[0x10]; struct U *pu; };
/** @sly2-annot addr=0x177D48
 * Purpose: Escort object at guard +0xF68 exists and is in the world (bit 60)
 * Info: proposed name FBhvguardEscortActive (T1); TU guardbhv
 * Notes: runtime2/4 confirmed the BHVGUARD role on live guards (S02-S41)
 * Confidence: mid (draft) [T1 0.50] - evidence: runtime (save-state RAM); sources:
 *   r9_bhv_screen+runtime(class role)
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
int FBhvguardEscortActive(struct S *p) { struct T *t = p->pu->pt; int f = 0; if (t) f = (int)(t->fl >> 60) & 1; return f; }

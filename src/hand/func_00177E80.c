struct S { char pad[0x34]; float g34; char pad2[0x10]; float g48; int n4C; char pad3[0x8]; int n58; char pad4[0x8]; float g64; float g68; float g6C; char pad5[0x30]; int nA0; };
extern float D_002BE624, D_002BE628, D_002BE62C;
void func_001EEF68(struct S *p);
/** @sly2-annot addr=0x177E80
 * Purpose: BHVPATH Init: no path, default speeds/radii, state -1
 * Info: proposed name InitBhvpath (T1); class BHVPATH slot7; TU guardbhv
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090,
 *   docs/reference/functions/player.md#tu_144b28-0x144b28-0x1471c0
 * @end-sly2-annot */
void func_00177E80(struct S *p) { func_001EEF68(p); p->g34 = p->g48 = -1.0f; p->g64 = D_002BE62C; p->g68 = D_002BE624; p->g6C = D_002BE628; p->n4C = -1; p->n58 = 0; p->nA0 = 1; }

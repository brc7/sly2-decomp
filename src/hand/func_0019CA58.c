struct K { int a, b; };
struct S { char pad[0xE0]; struct K k; };
void func_002311B0(struct S *);
/** @sly2-annot addr=0x19CA58
 * Purpose: JMT vt41 Clone: CloneXfm keeping own DLE
 * Info: proposed name CloneJmt (T1); class CID40 slot41; TU air
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#air-0x195950-0x19cbf8
 * @end-sly2-annot */
void func_0019CA58(struct S *p) { struct K k = p->k; func_002311B0(p); p->k = k; }

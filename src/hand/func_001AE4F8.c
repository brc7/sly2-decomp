struct MSG { int cb; int nKind; };
void *func_001AE2A0(struct MSG *p);
/** @sly2-annot addr=0x1AE4F8
 * Purpose: operator new: PvAllocSwClearImpl({cb 16})
 * Info: proposed name __builtin_new (T1); TU memory
 * Notes: mangled dropped: __builtin_new
 * Confidence: high [T1 0.95] - evidence: static analysis; sources: r5_1AE130
 * See: docs/engine/core-library.md#7-key-functions,
 *   docs/reference/functions/core-library.md#memory-0x1ae150-0x1ae7d0
 * @end-sly2-annot */
void *func_001AE4F8(int cb) { struct MSG m; m.cb = cb; m.nKind = 0x10; return func_001AE2A0(&m); }

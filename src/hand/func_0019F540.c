typedef int qword __attribute__((mode(TI)));
struct F { int a[4]; qword q; };
void *func_002333B4(void *, int, int);
void func_0019F590(void *, struct F *);
/** @sly2-annot addr=0x19F540
 * Purpose: Build a place record from a world position and call ReturnJtToSafePlace with it
 * Info: proposed name ReturnJtToPos (T2); TU places
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#places-0x19f3e8-0x1a0300
 * @end-sly2-annot */
void func_0019F540(void *p, qword q) { struct F f; func_002333B4(&f, 0, sizeof f); f.q = q; func_0019F590(p, &f); }

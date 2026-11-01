typedef int qword __attribute__((mode(TI)));
struct G { int a; };
extern struct G D_003DD490;
struct H { char pad[0x90]; qword q90; };
extern struct H D_002DDF40;
void *func_001C4840(struct G *g);
qword func_001A62E8(void *p);
/** @sly2-annot addr=0x173A18
 * Purpose: Current player position (PpoCur) or the camera position
 * Info: proposed name PosCid119Focus (T2); class CID119; TU tu_173158
 * Notes: Cid77->Cid119 (CID 0x77=119, agent-default(hex))
 * Confidence: low (draft) [T2 0.50] - evidence: static analysis; sources: r3b_rest
 * See: docs/reference/functions/guards-ai.md#tu_173158-0x173158-0x174898
 * @end-sly2-annot */
qword func_00173A18(void) { struct G *g = &D_003DD490; if (func_001C4840(g)) return func_001A62E8(func_001C4840(g)); return D_002DDF40.q90; }

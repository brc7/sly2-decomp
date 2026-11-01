struct G { int a; };
extern struct G D_003DD490;
struct P { char pad[0x38]; int fOn; int id; };
void DecrementSwHandsOff(struct G *pg, int id);
void func_0020F0F0(struct G *pg);
void IncrementSwHandsOff(struct G *pg, int id);
void func_0020F0C0(struct G *pg);
/** @sly2-annot addr=0x1E50E0
 * Purpose: Track fHandsOff/grfHandsOff (+0x38/+0x3C) and apply it to g_sw via Increment/DecrementSwHandsOff
 *   (0x20EFC0/0x20F030) + 0x20F0C0/0x20F0F0
 * Info: proposed name SetJsgHandsOff (T1); TU relay
 * Notes: runtime6 C4: JSG +0x38/+0x3C 1/3 -> SW locks [1,1,1,1] (S59)
 * Confidence: mid (draft) [T1 0.75] - evidence: runtime (save-state RAM); sources: r6b_rest+runtime
 * See: docs/engine/cutscenes.md#1-relay-and-jsg-the-sequence-engine-tu-0x1dfad8-0x1e5d80-file-name-unknown-lilly-jsgc,
 *   docs/engine/world-sw.md#5-hands-off-and-locks,
 *   docs/reference/functions/cutscenes.md#relay-0x1dfad8-0x1e5d80
 * @end-sly2-annot */
void func_001E50E0(struct P *p, int fOn, int id)
{
if (!fOn) { if (!p->fOn) return; } else { if (p->fOn) { if (id == p->id) return; DecrementSwHandsOff(&D_003DD490, p->id); func_0020F0F0(&D_003DD490); } IncrementSwHandsOff(&D_003DD490, id); func_0020F0C0(&D_003DD490); p->fOn = fOn; p->id = id; return; } DecrementSwHandsOff(&D_003DD490, p->id); func_0020F0F0(&D_003DD490); p->fOn = fOn; p->id = id;
}

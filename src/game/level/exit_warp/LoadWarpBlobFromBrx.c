struct M { int a, b; };
struct B { int a; int b; char pad[0x34]; int c; };
void Read__18CBinaryInputStreamiPv(void *p, int cb, struct B *pb);
void *func_001AE238(struct M *pm, struct B *pb);
/** @sly2-annot addr=0x228250
 * Purpose: reads 0x40-byte header and allocates a copy if non-empty
 * Info: proposed name LoadWarpBlobFromBrx (T1); TU exit_warp
 * Notes: level_parser CONFIRMED-parse: payload layout followed by the parser, 100%% of bytes in 4 levels
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r8_21BB88+parse-verified
 * See: docs/engine/world-sw.md#10-key-functions,
 *   docs/formats/level-objects.md#5-per-class-payloads-vtable-slot-14,
 *   docs/reference/functions/world-sw.md#exit_warp-0x227fe8-0x228b48
 * @end-sly2-annot */
void LoadWarpBlobFromBrx(void *p, void **pq)
{
    struct B b;
    struct M m;
    Read__18CBinaryInputStreamiPv(p, 0x40, &b);
    if (b.b != b.c) {
        { struct M *pm = &m; pm->a = 0x40; pm->b = 0x10; }
        *pq = func_001AE238(&m, &b);
    } else *pq = 0;
}

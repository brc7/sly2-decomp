struct S { int x0; char pad[0x3C]; char a40[0x18]; char a58[0x10]; char a68[0x30]; char a98[0xC]; char aA4[0x18]; char aBC[4]; };
void EnsureAsegBlendDynamic__FP3ALOiiPviPiT5PP6ASEGBL(int a, int b, int c, void *d, int e, int f, int g, void *h);

/** @sly2-annot addr=0x21F388
 * Purpose: EnsureAsegBlendDynamic x3: 2-clip, 4-clip and 2-clip aim blends (ablB/ablC)
 * Info: proposed name PostJtthrowLoad (T1); TU throw
 * Notes: agent qualifier: ensure throw blends
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r8_21BB88
 * See: docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638,
 *   docs/reference/functions/player.md#murray-0x1b4728-0x1b9f80,
 *   docs/reference/functions/player.md#throw-0x21f2e0-0x220d40
 * @end-sly2-annot */
void PostJtthrowLoad(struct S *p)
{
    EnsureAsegBlendDynamic__FP3ALOiiPviPiT5PP6ASEGBL(p->x0, 0xC, 2, p->a40, 0, 0, 0, p->a58);
    EnsureAsegBlendDynamic__FP3ALOiiPviPiT5PP6ASEGBL(p->x0, 0xC, 4, p->a68, 0, 0, 0, p->a98);
    EnsureAsegBlendDynamic__FP3ALOiiPviPiT5PP6ASEGBL(p->x0, 0xC, 2, p->aA4, 0, 0, 0, p->aBC);
}

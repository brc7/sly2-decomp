extern char D_0025AFA0[], D_0025B870[], D_002DF800[];
extern char D_004421A0[], D_004424B0[], D_004424D0[], D_004424F0[], D_00442520[], D_004425A0[], D_00442560[], D_00442240[];
extern char D_000000C0[], D_00000100[];
void EnsureVu1Code__FP4VIFSPvT1(void *, void *, void *);
void AddDmaCnt__4DMAS(void *);
void AddVifFlush__4VIFS(void *);
void AddDmaCall__4DMASP2QW(void *, void *);
void AddVifBaseOffset__4VIFSii(void *, int, int);
void AddVifUnpack__4VIFS3UPKiPvi(void *, int, int, void *, int);
void AddVifMscal__4VIFSPv(void *, void *);
void AddVifFlusha__4VIFS(void *);
void AddVifDirect__4VIFSiP2QWi(void *, int, void *, int);
void EndDmaCnt__4DMAS(void *);
/** @sly2-annot addr=0x211A80
 * Purpose: Pass 5 begin (kind 21): HUD microprogram + constants + DIRECT HUD GS state
 * Info: proposed name BeginRpHud (T1); TU hud
 * Notes: superseded: SetupSpriteVu1 (r7c_hud)
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r7c_hud+rendering+verdicts_followups
 * See: docs/engine/hud-ui.md#5-key-functions,
 *   docs/engine/rendering.md#4-the-26-kinds-and-12-passes-cstatic-tables-dumped-from-the-elf-roles-cstatic-unless-marked,
 *   docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
void BeginRpHud(void *p)
{
    EnsureVu1Code__FP4VIFSPvT1(p, D_0025AFA0, D_0025B870);
    AddDmaCnt__4DMAS(p);
    AddVifFlush__4VIFS(p);
    AddDmaCall__4DMASP2QW(p, D_002DF800);
    AddVifBaseOffset__4VIFSii(p, 0, 0);
    AddVifUnpack__4VIFS3UPKiPvi(p, 0xC, 7, D_004421A0, 0xD);
    AddVifUnpack__4VIFS3UPKiPvi(p, 0xC, 2, D_004424B0, 0x15);
    AddVifUnpack__4VIFS3UPKiPvi(p, 0xC, 2, D_004424D0, 0x17);
    AddVifUnpack__4VIFS3UPKiPvi(p, 0xC, 3, D_004424F0, 0x19);
    AddVifUnpack__4VIFS3UPKiPvi(p, 0xC, 4, D_00442520, 0x1C);
    AddVifUnpack__4VIFS3UPKiPvi(p, 0xC, 3, D_004425A0, 0x20);
    AddVifUnpack__4VIFS3UPKiPvi(p, 0xC, 4, D_00442560, 0x23);
    AddVifBaseOffset__4VIFSii(p, 0x32, 0x1E7);
    AddVifMscal__4VIFSPv(p, D_000000C0);
    AddVifFlusha__4VIFS(p);
    AddVifDirect__4VIFSiP2QWi(p, 0x27, D_00442240, 0);
    AddVifFlusha__4VIFS(p);
    AddVifMscal__4VIFSPv(p, D_00000100);
    AddVifFlusha__4VIFS(p);
    EndDmaCnt__4DMAS(p);
}

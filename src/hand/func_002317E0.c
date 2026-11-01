typedef struct VT { short d; short pad; void *pfn; } VT;
typedef struct LO { char pad[0xC]; VT *vt; } LO;
typedef struct L { int x0, x4; int c; LO **ap; } L;
typedef struct X { char pad[0x10]; void *x10; char pad2[0x3C]; void *x50; char pad3[0xC]; char m60[0x40]; } X;
void func_0022D670(void *);
void func_0022D698(void *);
void func_0013FB40(void *, void *, L *);

/** @sly2-annot addr=0x2317E0
 * Purpose: pushes XF context, loads child objects (0x13FB40), pops, applies mat to each (vt37)
 * Info: proposed name LoadXfmChildObjectsFromBrx (T1); TU xform
 * Notes: names_v2 C proposed SetAloForceCameraFade__FP3ALOi (0.438) | level_parser CONFIRMED-parse: payload
 *   layout followed by the parser, 100%% of bytes in 4 levels
 * Confidence: mid (draft) [T1 0.75] - evidence: static naming pass; sources: r8_21BB88+parse-verified
 * See: docs/engine/object-model.md#9-key-functions,
 *   docs/formats/level-objects.md#5-per-class-payloads-vtable-slot-14,
 *   docs/reference/functions/object-model.md#xform-0x231100-0x2319e8
 * @end-sly2-annot */
void func_002317E0(X *p, void *pv, L *pl)
{
    int i;
    func_0022D670(p->x50);
    func_0013FB40(p->x10, pv, pl);
    func_0022D698(p->x50);
    for (i = 0; i < pl->c; i++) {
        LO *lo = pl->ap[i];
        ((void (*)(void *, void *))lo->vt[37].pfn)((char *)lo + lo->vt[37].d, p->m60);
    }
}

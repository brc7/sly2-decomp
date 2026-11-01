struct E { char pad[0x18]; int *ap; char pad2[0x50]; };
extern struct E *D_003E1398;
int func_001C9958(void *p);
int func_001C9950(void *p);
/** @sly2-annot addr=0x1C9968
 * Purpose: aproxysrc[isrc].aoid[icopy] or -1
 * Info: proposed name OidFromIdProxy (T1); TU proxy
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r5_1AE130
 * See: docs/reference/functions/object-model.md#proxy-0x1c9940-0x1cb908
 * @end-sly2-annot */
int func_001C9968(void *p)
{
    struct E *pe;
    if (func_001C9958(p) == -1) return -1;
    pe = &D_003E1398[func_001C9950(p)];
    return pe->ap[func_001C9958(p)];
}

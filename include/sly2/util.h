/* include/sly2/util.h -- small C helpers (min/max/clamp/abs...) shared by the game TUs, one definition each.
 *
 * The per-function decompilation had pasted these into many TUs under private names (Min, GMin, GMax, Max, GClamp,
 * GLimit, GClampR, FPtr...). Only spellings that compile to the same code at every folded site live here; see
 * docs/guides/tu-merging.md "Shared inline helpers" for which private spellings were folded and which stay private.
 * Names follow the Sly 1 prefix convention (G float, N int, F flag); they are placeholders (HYPOTHESIS: Sly 1's util.h
 * has ClampAbs/ClampToLimits-style helpers, none of these exact ones). */
#ifndef SLY2_UTIL_H
#define SLY2_UTIL_H

inline float GMin(float a, float b) { return a < b ? a : b; }
inline float GMax(float a, float b) { return a > b ? a : b; }
inline int NMin(int a, int b) { return a < b ? a : b; }

/* g limited to [gMin, gMax] (the early-return form) */
inline float GClamp(float g, float gMin, float gMax)
{
    if (g < gMin)
        return gMin;
    if (gMax < g)
        return gMax;
    return g;
}

inline float GAbs(float g) { return __builtin_fabsf(g); }

/* a word that holds either a pointer (low bit clear) or a tagged value (low bit set): is it the pointer? */
inline int FIsPtr(int n) { return !(n & 1); }

/* t in [0, 1] mapped onto the integers from nFrom to nTo inclusive (each value gets an equal share of t) */
inline int NLerp(float t, int nFrom, int nTo)
{
    if (nFrom == nTo)
        return nTo;
    if (nFrom < nTo)
        return nFrom + (int)(t * ((float)nTo + 0.999f - (float)nFrom));
    return nTo + (int)((1.0f - t) * ((float)nFrom + 0.999f - (float)nTo));
}

#endif /* SLY2_UTIL_H */

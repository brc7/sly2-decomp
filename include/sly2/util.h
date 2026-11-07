/* include/sly2/util.h -- small C helpers (min/max/clamp/abs...) shared by the game TUs, one definition each.
 *
 * The per-function decompilation had pasted these into many TUs under private names (Min, GMin, GMax, Max, GClamp,
 * GLimit, GClampR, FPtr...). Only spellings that compile to the same code at every folded site live here; see
 * docs/guides/tu-merging.md "Shared inline helpers" for which private spellings were folded and which stay private.
 * Names follow docs/guides/naming-workflow.md section 1 (no Hungarian prefixes); they are descriptive placeholders
 * (HYPOTHESIS: Sly 1's util.h has ClampAbs/ClampToLimits-style helpers, none of these exact ones).
 *
 * Two spellings of min and max compile differently, so both exist: Min/Max (`a < b ? a : b`, `a > b ? a : b`) and
 * StdMin/StdMax (std::min / std::max's spelling: compare with `<` only, `b < a ? b : a`, `a < b ? b : a`). They agree
 * on every value except ties of signed zeros and NaN; pick the one the site was written with, or the code changes. */
#ifndef SLY2_UTIL_H
#define SLY2_UTIL_H

// Smaller of a and b.
inline float Min(float a, float b) { return a < b ? a : b; }
// Larger of a and b.
inline float Max(float a, float b) { return a > b ? a : b; }
// Smaller of two ints.
inline int MinInt(int a, int b) { return a < b ? a : b; }

/* std::min / std::max's spelling (see above) */
inline float StdMin(float a, float b) { return b < a ? b : a; }
// Larger of a and b (std::max operand order).
inline float StdMax(float a, float b) { return a < b ? b : a; }
// Larger of two ints (std::max operand order).
inline int StdMaxInt(int a, int b) { return a < b ? b : a; }

/* value limited to [low, high] (the early-return form; the if/else-if and in-place spellings fold into it too) */
inline float Clamp(float value, float low, float high)
{
    if (value < low)
        return low;
    if (high < value)
        return high;
    return value;
}

/* the int clamp, written with `value > high` */
inline int ClampInt(int value, int low, int high)
{
    if (value < low)
        return low;
    if (value > high)
        return high;
    return value;
}

// Absolute value.
inline float Abs(float value) { return __builtin_fabsf(value); }

/* a word that holds either a pointer (low bit clear) or a tagged value (low bit set): is it the pointer? */
inline int IsPointer(int word) { return !(word & 1); }

/* t in [0, 1] mapped onto the integers from first to last inclusive (each value gets an equal share of t) */
inline int LerpInt(float t, int first, int last)
{
    if (first == last)
        return last;
    if (first < last)
        return first + (int)(t * ((float)last + 0.999f - (float)first));
    return last + (int)((1.0f - t) * ((float)first + 0.999f - (float)last));
}

#endif /* SLY2_UTIL_H */

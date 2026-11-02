/** @sly2-annot addr=0x22BE08
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/animation.md#wr-0x22b720-0x22c818
 * @end-sly2-annot */
float UNoise(int n)
{
    int x = (n << 13) ^ n;
    int y = x * x * 0x3d73 + 0xc0ae5;
    int z = x * y + 0x5208dd0d;
    x = z & 0x7fffffff;
    return (float)x / 2147483648.0f;
}

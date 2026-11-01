extern float D_002C0C00;
/** @sly2-annot addr=0x1BF398
 * Purpose: Plane side class of a distance: 0 front, 1 back, 2 on (epsilon D_002C0C00)
 * Info: proposed name CbskFromG (T1); TU path
 * Confidence: high [T1 0.80] - evidence: static analysis; sources: r5b_rest
 * See: docs/engine/physics-collision.md#7-key-functions,
 *   docs/reference/functions/physics-collision.md#path-0x1bf110-0x1c2920
 * @end-sly2-annot */
int CbskFromG__Ff(float x)
{
    if (D_002C0C00 < x) return 0;
    if (x < -D_002C0C00) return 1;
    return 2;
}

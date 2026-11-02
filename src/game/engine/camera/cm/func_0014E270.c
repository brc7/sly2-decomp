extern float D_002A4FF8;
void func_001A37D0(float a, float b, float c, float d, float *pa, float *pb);
struct P { char pad[0x240]; float g240, g244, g248; };
/** @sly2-annot addr=0x14E270
 * Purpose: Builds the GS fog function (FGFN) from sNearFog/sFarFog/uFogMax (+0x240..0x248): linear map to
 *   0..4080, +8417280 bias
 * Info: proposed name BuildCmFgfn (T1); TU cm
 * Confidence: high [T1 0.80] - evidence: semantic review; sources: r2_131F70
 * See: docs/engine/camera.md#5-key-functions, docs/reference/functions/camera.md#cm-0x14d3d8-0x14fac8
 * @end-sly2-annot */
void func_0014E270(struct P *p, float *af, float f)
{
    if (f == D_002A4FF8) f = 1.0f;
    func_001A37D0(1.0f, 4080.0f, p->g244 / p->g240, (1.0f - p->g248 * f) * 4080.0f, &af[0], &af[1]);
    af[0] += 8417280.0f;
    af[2] = p->g240;
    af[3] = af[0] + 8192.0f;
}

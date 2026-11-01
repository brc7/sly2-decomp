extern float D_002DDED0;
/** @sly2-annot addr=0x14F750
 * Purpose: tJolt(+0x618)=clock.t, nUnk61c=arg: starts the 0.5 s camera shake applied by UpdateCmMat4
 * Info: proposed name JoltCm (T1); TU cm
 * Notes: names_v2 v1 proposed FUN_00145dd8__FPvP2SO (0.000)
 * Confidence: high [T1 0.80] - evidence: semantic review; static naming pass; sources: r2_131F70
 * See: docs/engine/camera.md#5-key-functions, docs/reference/functions/camera.md#cm-0x14d3d8-0x14fac8
 * @end-sly2-annot */
void func_0014F750(char *p, int a)
{
    *(float *)(p + 0x618) = D_002DDED0;
    *(int *)(p + 0x61C) = a;
}

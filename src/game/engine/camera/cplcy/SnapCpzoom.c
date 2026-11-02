typedef int qword __attribute__((mode(TI)));
struct S {
    int pad[6]; float g18; int pad1c;
    qword q20; qword q30; qword q40; qword q50; qword q60; float g70; int pad74[3];
    qword q80; qword q90; float gA0; int padA4[2]; float gAC;
};
qword PosCpzoomCenter(struct S *p);
qword PosCpzoomEye(struct S *p);
/** @sly2-annot addr=0x150EF0
 * Purpose: Reset smoothed center/eye to current targets, uZoom=uZoomTarget (if >=0), pose = target pose
 * Info: proposed name SnapCpzoom (T1); TU cplcy
 * Confidence: high [T1 0.85] - evidence: semantic review; sources: r2_131F70+verdicts_tu_r1r2
 * See: docs/engine/camera.md#5-key-functions, docs/reference/functions/camera.md#cplcy-0x14fca8-0x1520c8
 * @end-sly2-annot */
void SnapCpzoom(struct S *p)
{
    p->q20 = PosCpzoomCenter(p);
    p->q30 = PosCpzoomEye(p);
    if (0.0f <= p->g18)
        p->gAC = p->g18;
    p->q50 = p->q80;
    p->q60 = p->q90;
    p->g70 = p->gA0;
}

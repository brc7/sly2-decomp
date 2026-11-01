struct BUF { int pad0[2]; char *p; int c; int pad1[2]; char *p2; int c2; int pad2[12]; };
extern struct BUF D_002DEA50[2];
extern int D_003D3980;
extern char D_003CF980[];
extern char D_002F7980[];
extern char D_00303980[];
void func_00234CA0(int n);
/** @sly2-annot addr=0x1C2A78
 * Purpose: if phase != g_phase(D_003D3980): FlushCache, set g_phase and (Sly2) re-point phase memory buffers
 * Info: proposed name SetPhase (T1); TU phasemem
 * Notes: names_v2 v1 proposed UpdateJtDrive__FP2JT (0.022)
 * Confidence: high [T1 0.85] - evidence: static naming pass; sources: r5_1AE130
 * See: docs/engine/world-sw.md#4-transitions-warps-exits-wipes-boot-queue,
 *   docs/engine/world-sw.md#9-source-files, docs/engine/world-sw.md#10-key-functions
 * @end-sly2-annot */
void func_001C2A78(int mode)
{
    struct BUF *pb;
    int i;
    if (mode == D_003D3980) return;
    func_00234CA0(0);
    D_003D3980 = mode;
    switch (mode) {
    case 3:
        for (i = 0; i < 2; i++) {
            pb = &D_002DEA50[i];
            pb->p = D_002F7980 + i * 0x6000;
            pb->p2 = D_00303980 + i * 0x58000;
            pb->c = 0x600;
            pb->c2 = 0x5800;
        }
        break;
    case 2:
        for (i = 0; i < 2; i++) {
            pb = &D_002DEA50[i];
            pb->p = D_003CF980 + i * 0x1800;
            pb->p2 = D_003CF980 + 0x3000 + i * 0x800;
            pb->c = 0x180;
            pb->c2 = 0x80;
        }
        break;
    default:
        for (i = 0; i < 2; i++) {
            pb = &D_002DEA50[i];
            pb->c = 0;
            pb->p = 0;
            pb->c2 = 0;
            pb->p2 = 0;
        }
        break;
    }
}

typedef int qword __attribute__((mode(TI)));
extern char D_003DD490[];
struct S { char pad[0x20]; qword v; char pad2[0xB8 - 0x30]; int f; };
int CploFindSwObjectsByClass(void *p, int a, int b, int c, int d, int *e);
int FCheckVolPoint(int a, qword v);
/** @sly2-annot addr=0x201F90
 * Purpose: flag +0xB8 = origin inside any class-0x19 VOL (FCheckVolPoint)
 * Info: proposed name UpdateReconSelInVolume (T2); TU tu_201D78
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/player.md#tu_201d78-0x201d78-0x203948
 * @end-sly2-annot */
void func_00201F90(struct S *p)
{
    int a[16];
    int n, i;
    int *pa;
    p->f = 0;
    n = CploFindSwObjectsByClass(D_003DD490, 0x505, 0x19, 0, 16, a);
    for (i = 0; i < n; i++) {
        if (FCheckVolPoint(a[i], p->v)) {
            p->f = 1;
            break;
        }
    }
}

typedef int qword __attribute__((mode(TI)));
struct M { float a[12]; };
struct S { char pad[0x17C]; int x17c; char p1[0xC]; int x18c; };
extern qword D_002A2DD0;
extern char D_002BE8D0[], D_002BE8E0[];
void func_001F9B20(struct S *p);
void *func_001A6318(struct S *p);
void func_0011B938(void *p, int n, struct M *pm);
void func_001F5C30(struct S *p, struct M *pm, void *a, qword q, void *b);
/** @sly2-annot addr=0x183B00
 * Purpose: vt70: pending velocity, and while free (x18C, state 0) spring the object upright
 * Info: proposed name PresetBarrelAccel (T2); class BARREL slot70; TU grabspot
 * Notes: v3 class identity: PresetCid88Accel -> PresetBarrelAccel
 * Confidence: low (draft) [T2 0.40] - evidence: static naming pass; sources:
 *   r4b_rest+class_review(master_names_v2_1)
 * See: docs/reference/functions/player.md#grabspot-0x182c70-0x1848a8
 * @end-sly2-annot */
void func_00183B00(struct S *p)
{
    func_001F9B20(p);
    if (p->x18c && !p->x17c) {
        struct M m;
        func_0011B938(func_001A6318(p), 0, &m);
        func_001F5C30(p, &m, D_002BE8D0, D_002A2DD0, D_002BE8E0);
    }
}

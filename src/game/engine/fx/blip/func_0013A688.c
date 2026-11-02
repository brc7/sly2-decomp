struct CLOCK { float t; char pad[0x3C]; float g40; };
struct CM { char pad[0x100]; float g100; char pad1[0x10]; float g114; };
struct P { char pad[0x110]; char a110[0x20]; char a130[0x10]; char a140[0x10]; char a150[0x10]; char pad1[0x1A0 - 0x160]; int x1A0;
    char pad2[0x1B0 - 0x1A4]; char a1B0[0x3B0 - 0x1B0]; int x3B0; char a3B4[0x434 - 0x3B4]; int x434; void *p438; int x43C; float g440; };
extern struct CLOCK D_002DDED0;
extern struct CM D_002DDF40;
extern char D_002DEA28[];
void func_00226E88(void *pdma, int a, int b, int c, int d, void *ppv);
void func_00226F28(void *pdma, int a, int b, void *pv, int d);
void func_00227010(void *pdma, int a, int b, void *pv, int d, int e);
/** @sly2-annot addr=0x13A688
 * Purpose: Append VIF unpacks of render constants (camera fields, group colour, time, flags, blip arrays) to
 *   the DMA list
 * Info: proposed name AddBlipgVu1Params (T2); TU blip
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/particles-effects.md#blip-0x138f08-0x13a9e8
 * @end-sly2-annot */
void func_0013A688(struct P *p)
{
    float *pg;
    int *pn;
    char *pdma = D_002DEA28;
    struct CLOCK *pclock;
    func_00226E88(pdma, 0xC, 1, 5, 0, &pg);
    pclock = &D_002DDED0;
    pg[0] = D_002DDF40.g100;
    pg[1] = D_002DDF40.g114;
    pg[2] = p->g440;
    pg[3] = D_002DDED0.t;
    func_00226E88(pdma, 0xC, 1, 6, 0, &pn);
    pn[0] = 0;
    if (p->x434)
        pn[0] |= 1;
    pn[1] = p->x3B0;
    pn[2] = p->x1A0;
    *(float *)&pn[3] = pclock->g40;
    func_00226F28(pdma, 0xE, p->x3B0, p->a3B4, 0x30 - p->x3B0);
    func_00226F28(pdma, 8, 1, p->a130, 1);
    func_00226F28(pdma, 8, 1, p->a110, 2);
    func_00226F28(pdma, 8, 1, p->a140, 3);
    func_00226F28(pdma, 8, 1, p->a150, 4);
    func_00226F28(pdma, 0xC, 4, p->p438, 0x30);
    func_00227010(pdma, 0xC, p->x1A0, p->a1B0, 0xF, 0);
}

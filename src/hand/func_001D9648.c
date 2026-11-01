extern struct CLOCK { float t; float dt; float g; } D_002DDED0;
typedef struct { char pad[0x100C]; int x100C; } PAD;
extern int D_002DFBC0;
extern int D_002DFBC4;
extern PAD D_002DFC00;
extern PAD D_002E0D00;
extern char D_003D9ED0[], D_003DA150[], D_003DA0F0[], D_002DDEC0[], D_00443740[], D_003D9E80[];
extern char D_003DA0C0[], D_003DA3C0[], D_002E6550[], D_002E65A0[], D_003DA4F0[], D_003DA530[];
void func_001870A0(int);
int func_00187D80(PAD *, int);
void func_00187E08(PAD *, int);
int func_001B0290(int);
int FIsDlEmpty__FP2DL(void *);
void func_001AEEE0(void *);
void func_001DB870(void *);
void func_001DAA90(void *);
void func_0014BFF8(void *);
void func_0022B198(void *);
void func_001D9DF8(void *);
void func_001DA350(void *);
void func_001DCD30(void *);
void func_001ACCC8(void *);
void func_001ADEE8(void *);
void func_001DA808(void *);
void func_001DD920(void *);
void func_001D9648(void *pdl)
{
    PAD *pad;
    if (D_002DFBC4 & 8) {
        if (D_002DDED0.g == 0.0f || 0.1f < D_002DDED0.t / D_002DDED0.g)
            func_001870A0(8);
    }
    if (D_002DFBC0 & 2)
        pad = &D_002DFC00;
    else
        pad = &D_002E0D00;
    if (func_00187D80(pad, 0x800)) {
        if (!func_001B0290(0))
            func_00187E08(pad, 0x800);
    } else if (D_002DFBC0 & 2) {
        int r = FIsDlEmpty__FP2DL(pdl);
        int f = 0;
        if (pad->x100C == 1 && 5.0f <= D_002DDED0.t)
            f = 1;
        if (r && f)
            func_001B0290(0);
    }
    func_001AEEE0(D_003D9ED0);
    func_001DB870(D_003DA150);
    func_001DAA90(D_003DA0F0);
    func_0014BFF8(D_002DDEC0);
    func_0022B198(D_00443740);
    func_001D9DF8(D_003D9E80);
    func_001DA350(D_003DA0C0);
    func_001DCD30(D_003DA3C0);
    func_001ACCC8(D_002E6550);
    func_001ADEE8(D_002E65A0);
    func_001DA808(D_003DA4F0);
    func_001DD920(D_003DA530);
}

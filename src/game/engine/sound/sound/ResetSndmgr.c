typedef int qword __attribute__((mode(TI)));
struct P {
    char pad0[0x14]; int n14; int n18; char s1C[0x18]; char s34[0x18]; char s4C[0x10]; int n5C; int n60; int pad64; int n68;
    int pad6C[2]; char s74[0xEC - 0x74]; int aEC[6]; int pad104; char s108[0xC];
    char pad114[0x138 - 0x114]; int n138; char pad13C[0x1D0 - 0x13C]; int n1D0; char pad1D4[0x200 - 0x1D4];
    int r200; int n204; char s208[0x20]; int r228; char s22C[0x18];
};
extern qword D_002A2DD0;
extern char D_004435F0[];
void snd_StopAllStreams(void);
void func_00233048(int, int, int, int, int, int, int);
int snd_FlushSoundCommands(void);
void func_00156BE0(int, qword, int, void *);
void *func_002333B4(void *, int, int);
void func_00227788(void *);
void func_00259B48(void *, void *);
void ResetSlotheap(void *);
void ClearDl__FP2DL(void *);
void ResetSndmgrVolumes(struct P *);
/** @sly2-annot addr=0x1FD700
 * Purpose: Stop all sounds/streams (989 cmd 0x18, SUKR cmd 10), flush, clear state, free dirty list, reset
 *   volumes
 * Info: proposed name ResetSndmgr (T1); TU sound
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void ResetSndmgr(struct P *p)
{
    snd_StopAllStreams();
    func_00233048(0x524B5553, 10, 0, 0, 0, 0, 0);
    while (snd_FlushSoundCommands())
        ;
    p->n14 = 0;
    p->n18 = -1;
    p->n1D0 = 0;
    p->n5C = 0;
    p->n60 = 0;
    p->n138 = 0;
    p->aEC[0] = 0;
    p->aEC[1] = 0;
    p->aEC[2] = 0;
    p->aEC[3] = 0;
    p->aEC[4] = 0;
    p->aEC[5] = 0;
    func_00156BE0(0, D_002A2DD0, 0, p->s74);
    func_002333B4(p->s108, 0, 0xC);
    func_00227788(D_004435F0);
    func_00259B48(&p->r200, 0);
    p->n204 = 0;
    func_002333B4(p->s208, 0, 0x20);
    func_00259B48(&p->r228, 0);
    func_002333B4(p->s22C, 0, 0x18);
    p->n68 = 3;
    ResetSlotheap(p->s1C);
    ResetSlotheap(p->s34);
    ClearDl__FP2DL(p->s4C);
    ResetSndmgrVolumes(p);
}

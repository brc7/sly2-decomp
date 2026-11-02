typedef int qword __attribute__((mode(TI)));
struct X { char pad[0xA0]; void *xA0; int pA4; float xA8; int pAC; qword xB0; qword xC0; qword xD0; };
struct S {
    char pad[0x188]; int x188; float x18C; void *x190; int x194; int p198[2];
    char x1A0[8]; void *a1A8[9]; void *a1CC[14]; struct X *x204; void *x208;
};
struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
extern qword D_002C1080;
extern qword D_002C1090;
void func_001306A8(void *p);
void func_00142140(void *p, void *q);
qword ConvertAloVec(void *p, void *frame, qword q);
qword ConvertXfVec(void *frame, void *p, qword q);
qword func_001A62E8(void *p);
qword func_0022FC48(void *frame, void *p, qword q);
void func_002303A0(struct S *p, struct X *px, int n);
void func_001305B8(void *clip, struct S *owner, float position, float speed, int flags, void *output);
static inline void Selection(const struct S*p,int*index,struct X**timer){
 register int selected __asm__("$2");register struct X*receiver __asm__("s0");
 __asm__("lw %0,0x194(%2)\n\tlw %1,0x204(%2)":"=&r"(selected),"=r"(receiver):"r"(p),"m"(p->x194),"m"(p->x204));
 *index=selected;*timer=receiver;
}
/** @sly2-annot addr=0x1C8918
 * Purpose: dial/handle orientation update
 * Info: proposed name UpdateVaultDial (T2); TU prize
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r5_1AE130
 * See: docs/reference/functions/game-progression.md#prize-0x1c4e50-0x1c8cc0
 * @end-sly2-annot */
void func_001C8918(struct S *p, int n)
{
    struct X *px;
    qword q;
    if (n == p->x188) return;
    func_001306A8(p->x1A0);
    p->x188 = n;
    p->x18C = D_002DDED0.t;
    switch (n) {
    case -1:
        p->x194 = -1;
        func_00142140(p->x190, 0);
        break;
    case 0:
        func_00142140(p->x190, p->a1A8[p->x194]);
        break;
    case 1:
        func_00142140(p->x190, p->x208);
        p->x204->xA8 = D_002DDED0.t + 0.25f;
        px = p->x204;
        q = ConvertXfVec(0, px->xA0, ConvertAloVec(p->x190, 0, D_002C1080));
        px->xB0 = q;
        {int selected;Selection(p,&selected,&px);
        q=func_0022FC48(0,px->xA0,func_001A62E8(p->a1A8[selected]));}
        px->xC0 = q;
        px = p->x204;
        q = ConvertXfVec(0, px->xA0, ConvertAloVec(p->x190, 0, D_002C1090));
        px->xD0 = q;
        func_002303A0(p, p->x204, 0x11);
        break;
    case 2:
        func_00142140(p->x190, p->a1A8[p->x194]);
        func_001305B8(p->a1CC[p->x194], p, 0.0f, 1.0f, 0, p->x1A0);
        break;
    }
}

#define OFF(t,m) ((int)&((t*)0)->m)
typedef char transition_layout[(sizeof(qword)==16 && OFF(struct CLOCK,t)==0 && OFF(struct CLOCK,dt)==4 && OFF(struct X,xA0)==0xA0 && OFF(struct X,xA8)==0xA8 && OFF(struct X,xB0)==0xB0 && OFF(struct X,xC0)==0xC0 && OFF(struct X,xD0)==0xD0 && OFF(struct S,x188)==0x188 && OFF(struct S,x18C)==0x18C && OFF(struct S,x190)==0x190 && OFF(struct S,x194)==0x194 && OFF(struct S,x1A0)==0x1A0 && OFF(struct S,a1A8)==0x1A8 && OFF(struct S,a1CC)==0x1CC && OFF(struct S,x204)==0x204 && OFF(struct S,x208)==0x208)?1:-1];

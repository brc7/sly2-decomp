// Update attachment state and projected velocity.
typedef int qword __attribute__((mode(TI)));
struct VT { short d; short pad; void *pfn; };
struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
extern char D_003DD490[];
extern qword D_002A2DC0;
union WORD{int bits;float value;};extern union WORD D_002C09A8;
struct SIDE { int x0; int x4; void *x8; void *xC; qword v10; char pad[0x30 - 0x20]; qword v30; };
struct LINK { struct SIDE as[2]; char pad[0xAC - 0x80]; struct LINK *pnext; };
struct MSG { void *x0; int x4; void *x8; int xC; int x10; char pad[0x20 - 0x14]; qword v20; qword v30; char pad2[0x60 - 0x40]; };
struct Q { char pad[0x14]; void *x14; };
struct V3 { float x, y, z; };
struct P {
    char pad0[0xC]; struct VT *vt; char pad1[0x190 - 0x10]; struct V3 v190; char pad2[0x1030 - 0x19C]; struct Q *p1030;
    char pad3[0x131C - 0x1034]; float g131C; char pad4[0x13D8 - 0x1320]; int x13D8; float g13DC; char pad5[0x13E4 - 0x13E0];
    struct LINK *plink;
};
void func_001A7E10(void *pg, struct LINK *pl);
void *func_001F7530(void *p, int n);
void *func_002333B4(void *p, int c, int cb);
void *func_001F7598(struct MSG *pmsg);
float *func_0012F938(void *p, int n);
void func_00130D30(struct Q *p, int n, float g, float h);
void func_00130B18(struct Q *p);
static inline void Zero(struct V3 *pv) { pv->z = 0.0f; pv->y = 0.0f; pv->x = 0.0f; }
#define FIELD_OFFSET(t,m) ((unsigned int)&(((struct t*)0x10000)->m)-0x10000)
typedef char qword_Size[sizeof(qword)==16?1:-1];
typedef char WORD_Size[sizeof(union WORD)==4?1:-1];
typedef char VT_Size[sizeof(struct VT)==8?1:-1];
typedef char CLOCK_Size[sizeof(struct CLOCK)==8?1:-1];
typedef char SIDE_Size[sizeof(struct SIDE)==64?1:-1];
typedef char LINK_Size[sizeof(struct LINK)==176?1:-1];
typedef char MSG_Size[sizeof(struct MSG)==96?1:-1];
typedef char Q_Size[sizeof(struct Q)==24?1:-1];
typedef char V3_Size[sizeof(struct V3)==12?1:-1];
typedef char P_Size[sizeof(struct P)==5096?1:-1];
typedef char VT_d_Offset[FIELD_OFFSET(VT,d)==0?1:-1];
typedef char VT_pfn_Offset[FIELD_OFFSET(VT,pfn)==4?1:-1];
typedef char CLOCK_t_Offset[FIELD_OFFSET(CLOCK,t)==0?1:-1];
typedef char CLOCK_dt_Offset[FIELD_OFFSET(CLOCK,dt)==4?1:-1];
typedef char SIDE_x0_Offset[FIELD_OFFSET(SIDE,x0)==0?1:-1];
typedef char SIDE_x4_Offset[FIELD_OFFSET(SIDE,x4)==4?1:-1];
typedef char SIDE_x8_Offset[FIELD_OFFSET(SIDE,x8)==8?1:-1];
typedef char SIDE_xC_Offset[FIELD_OFFSET(SIDE,xC)==12?1:-1];
typedef char SIDE_v10_Offset[FIELD_OFFSET(SIDE,v10)==16?1:-1];
typedef char SIDE_v30_Offset[FIELD_OFFSET(SIDE,v30)==48?1:-1];
typedef char LINK_as_Offset[FIELD_OFFSET(LINK,as)==0?1:-1];
typedef char LINK_pnext_Offset[FIELD_OFFSET(LINK,pnext)==172?1:-1];
typedef char MSG_x0_Offset[FIELD_OFFSET(MSG,x0)==0?1:-1];
typedef char MSG_x4_Offset[FIELD_OFFSET(MSG,x4)==4?1:-1];
typedef char MSG_x8_Offset[FIELD_OFFSET(MSG,x8)==8?1:-1];
typedef char MSG_xC_Offset[FIELD_OFFSET(MSG,xC)==12?1:-1];
typedef char MSG_x10_Offset[FIELD_OFFSET(MSG,x10)==16?1:-1];
typedef char MSG_v20_Offset[FIELD_OFFSET(MSG,v20)==32?1:-1];
typedef char MSG_v30_Offset[FIELD_OFFSET(MSG,v30)==48?1:-1];
typedef char Q_x14_Offset[FIELD_OFFSET(Q,x14)==20?1:-1];
typedef char V3_x_Offset[FIELD_OFFSET(V3,x)==0?1:-1];
typedef char V3_y_Offset[FIELD_OFFSET(V3,y)==4?1:-1];
typedef char V3_z_Offset[FIELD_OFFSET(V3,z)==8?1:-1];
typedef char P_vt_Offset[FIELD_OFFSET(P,vt)==12?1:-1];
typedef char P_v190_Offset[FIELD_OFFSET(P,v190)==400?1:-1];
typedef char P_p1030_Offset[FIELD_OFFSET(P,p1030)==4144?1:-1];
typedef char P_g131C_Offset[FIELD_OFFSET(P,g131C)==4892?1:-1];
typedef char P_x13D8_Offset[FIELD_OFFSET(P,x13D8)==5080?1:-1];
typedef char P_g13DC_Offset[FIELD_OFFSET(P,g13DC)==5084?1:-1];
typedef char P_plink_Offset[FIELD_OFFSET(P,plink)==5092?1:-1];
/** @sly2-annot addr=0x1B6DA8
 * Purpose: Flop substates: 1 = dive (sets downward velocity via vt35), 2 = impact: zap every contact link
 *   (InflictZap kind 4 damage -20), free contact list, play landing ASEG event 0x1A5; -1 cleanup
 * Info: proposed name SetMurrayFlopSubstate (T1); TU murray
 * Notes: runtime5a #27/#28: substate 1 dive (vz -880), 2 impact -> InflictZap(grfzpk 4, dhp -20) per contact
 * Confidence: mid (draft) [T1 0.55] - evidence: runtime (save-state RAM); sources: r5b_rest+runtime
 * See: docs/reference/functions/player.md#murray-0x1b4728-0x1b9f80
 * @end-sly2-annot */
void func_001B6DA8(struct P *p, int s)
{
    if (p->x13D8 == s)
        return;
    if (p->x13D8 == -1)
        Zero(&p->v190);
    p->x13D8 = s;
    p->g13DC = D_002DDED0.t;
    switch (s) {
    case 0:
        break;
    case -1:
        func_001A7E10(D_003DD490, p->plink);
        p->plink = 0;
        break;
    case 1: {
        union { qword q; float f[4]; } u;
        register qword v __asm__("vf3"),up __asm__("vf2"),d __asm__("vf1"),r __asm__("vf1"),k __asm__("vf4");
        register unsigned int bits __asm__("$2");
        register struct VT*vt __asm__("$5");
        register int delta __asm__("$4");
        register void(*setter)(void*,qword) __asm__("$2");
        register qword argument __asm__("$5");
        v=((qword(*)(void*))p->vt[33].pfn)((char*)p+p->vt[33].d);
        __asm__("lw %0,%3\n\tlqc2 %1,%4\n\tlui %2,%5":"=&r"(vt),"=j"(up),"=r"(bits):"m"(p->vt),"m"(D_002A2DC0),"I"(0x2C));
        __asm__("vmul.xyz %0,%3,%4\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z\n\tlw %2,%5":"=&j"(d),"=&j"(k),"+r"(bits):"j"(v),"j"(up),"m"(((int*)bits)[0x9A8/4]));
        __asm__("vmulx.xyzw %0,%2,%0x\n\tlh %1,%3":"+j"(d),"=r"(delta):"j"(up),"m"(vt[35].d));
        __asm__("qmtc2.ni %5,%1\n\tvsub.xyzw %0,%0,%6\n\tlw %2,%8\n\tvadda.xyzw $ACC,%0,$vf0\n\tvmaddx.xyzw %3,%7,%1x\n\tqmfc2.ni %4,%3":"+&j"(v),"=&j"(k),"=r"(setter),"=j"(r),"=r"(argument):"r"(bits),"j"(d),"j"(up),"m"(vt[35].pfn));
        u.q=k;
        setter((char*)p+delta,argument);
        p->g131C = D_002DDED0.t;
        break;
    }
    case 2: {
        struct LINK *pl;
        Zero(&p->v190);
        for (pl = p->plink; pl; pl = pl->pnext) {
            int i = pl->as[0].x8 != p;
            struct SIDE *pso = &pl->as[1 - i];
            struct SIDE *psm = &pl->as[i];
            void *po = func_001F7530(pso->xC, 4);
            if (po) {
                struct MSG msg;
                func_002333B4(&msg, 0, sizeof(msg));
                msg.x0 = p;
                msg.x4 = psm->x0;
                msg.x8 = po;
                msg.xC = 4;
                msg.x10 = -20;
                msg.v20 = pso->v10;
                msg.v30 = psm->v30;
                (void)func_001F7598(&msg);
            }
        }
        func_001A7E10(D_003DD490, p->plink);
        p->plink = 0;
        {
            float *pg = func_0012F938(p->p1030->x14, 0x1A5);
            if (pg)
                func_00130D30(p->p1030, 0, *pg, 1.0f);
            else
                func_00130B18(p->p1030);
        }
        break;
    }
    }
}

typedef char int_Width[sizeof(int)==4?1:-1];
typedef char float_Width[sizeof(float)==4?1:-1];
typedef char pointer_Width[sizeof(void*)==4?1:-1];
typedef char qword_Alignment[__alignof__(qword)==16?1:-1];
typedef char WORD_bits_Offset[((unsigned int)&(((union WORD*)0)->bits))==0?1:-1];
typedef char WORD_value_Offset[((unsigned int)&(((union WORD*)0)->value))==0?1:-1];

typedef int qword __attribute__((mode(TI)));
struct CLOCK { float t,dt; };
struct CLIP; struct ANIMATION;
struct COMPONENT_DATA;
struct COMPONENT { struct COMPONENT_DATA *value; };
struct SCRIPT { int opaque; struct CLIP *clip; char rest[0x14]; };
struct M3 { qword row[3]; };
extern struct CLOCK D_002DDED0;
extern qword D_002A2DD0;
struct VE { short d; short pad; void (*pfn)(); };
struct O { char pad[0xC]; struct VE *vt; char pad1[0x40 - 0x10]; qword v40; char rest[0x20]; };
union V3 { qword q; struct { float x,y,z,w; } value; };
static __inline__ void Zero(union V3 *pv) { pv->value.z = 0; pv->value.y = 0; pv->value.x = 0; }
struct P { char pad[0x190]; union V3 v190; char pad1[0x1030 - 0x1A0]; struct ANIMATION *x1030; struct COMPONENT x1034;
    char pad2[0x1840 - 0x1038]; int x1840; float f1844; struct CLIP *x1848; struct CLIP *x184C; struct SCRIPT x1850; float f186C;
    char pad3[0x1890 - 0x1870]; struct O x1890; struct O *x1900; struct CLIP *x1904; int x1908; };
void func_0020C080(struct O *o);
void StartLeap(struct SCRIPT *a, struct P *p, int n, struct O *b);
qword func_001A62E8(struct P *p);
struct M3 *func_001A6318(struct P *p);
void ReleaseSma(struct COMPONENT *p);
void func_001305B8(struct CLIP *a, struct P *p, float time, float rate, int n, struct ANIMATION **q);
#define FIELD_OFFSET(t,m) ((unsigned int)&(((struct t*)0x10000)->m)-0x10000)
typedef char CLOCK_t_Offset[FIELD_OFFSET(CLOCK,t)==0?1:-1];
typedef char CLOCK_dt_Offset[FIELD_OFFSET(CLOCK,dt)==4?1:-1];
typedef char COMPONENT_value_Offset[FIELD_OFFSET(COMPONENT,value)==0?1:-1];
typedef char SCRIPT_clip_Offset[FIELD_OFFSET(SCRIPT,clip)==4?1:-1];
typedef char M3_row_Offset[FIELD_OFFSET(M3,row)==0?1:-1];
typedef char VE_d_Offset[FIELD_OFFSET(VE,d)==0?1:-1];
typedef char VE_pfn_Offset[FIELD_OFFSET(VE,pfn)==4?1:-1];
typedef char O_vt_Offset[FIELD_OFFSET(O,vt)==12?1:-1];
typedef char O_v40_Offset[FIELD_OFFSET(O,v40)==64?1:-1];
typedef char P_v190_Offset[FIELD_OFFSET(P,v190)==400?1:-1];
typedef char P_x1030_Offset[FIELD_OFFSET(P,x1030)==4144?1:-1];
typedef char P_x1034_Offset[FIELD_OFFSET(P,x1034)==4148?1:-1];
typedef char P_x1840_Offset[FIELD_OFFSET(P,x1840)==6208?1:-1];
typedef char P_f1844_Offset[FIELD_OFFSET(P,f1844)==6212?1:-1];
typedef char P_x1848_Offset[FIELD_OFFSET(P,x1848)==6216?1:-1];
typedef char P_x184C_Offset[FIELD_OFFSET(P,x184C)==6220?1:-1];
typedef char P_x1850_Offset[FIELD_OFFSET(P,x1850)==6224?1:-1];
typedef char P_f186C_Offset[FIELD_OFFSET(P,f186C)==6252?1:-1];
typedef char P_x1890_Offset[FIELD_OFFSET(P,x1890)==6288?1:-1];
typedef char P_x1900_Offset[FIELD_OFFSET(P,x1900)==6400?1:-1];
typedef char P_x1904_Offset[FIELD_OFFSET(P,x1904)==6404?1:-1];
typedef char P_x1908_Offset[FIELD_OFFSET(P,x1908)==6408?1:-1];
typedef char CLOCK_Size[sizeof(struct CLOCK)==8?1:-1];
typedef char COMPONENT_Size[sizeof(struct COMPONENT)==4?1:-1];
typedef char SCRIPT_Size[sizeof(struct SCRIPT)==28?1:-1];
typedef char M3_Size[sizeof(struct M3)==48?1:-1];
typedef char VE_Size[sizeof(struct VE)==8?1:-1];
typedef char O_Size[sizeof(struct O)==112?1:-1];
typedef char P_Size[sizeof(struct P)==6416?1:-1];
typedef char qword_Size[sizeof(qword)==16?1:-1];
typedef char V3_Size[sizeof(union V3)==16?1:-1];
void func_001B8D30(struct P *p, int st)
{
    struct CLIP *o;
    int n;
    if (st == p->x1840) return;
    if (p->x1840 == 2) {
        func_0020C080(p->x1900);
        ((void (*)())p->x1900->vt[2].pfn)((char *)p->x1900 + p->x1900->vt[2].d);
        p->x1900 = 0;
    }
    p->x1840 = st;
    p->f1844 = D_002DDED0.t;
    o = 0;
    n = 0;
    switch (st) {
    case 0:
        o = p->x1848;
        Zero(&p->v190);
        n = 1;
        break;
    case 1:
        o = p->x184C;
        n = 9;
        break;
    case 2: {
        struct O *q;
        qword b;
        struct M3 *px;
        union { qword q; float f[4]; } u, uv, u2;
        p->x1908 = 0;
        p->x1900 = &p->x1890;
        StartLeap(&p->x1850, p, 1, &p->x1890);
        q = p->x1900;
        uv.q = func_001A62E8(p);
        __asm__("mfc1 %0, %1" : "=r"(b) : "f"(-p->f186C));
        u.q = b;
        u2.q = u.q;
        px = func_001A6318(p);
        {
            register qword axis __asm__("vf1") = px->row[0];
            register qword origin __asm__("vf2") = uv.q;
            __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddx.xyzw %0, %0, %2x"
                    : "+j"(axis) : "j"(origin), "j"(u2.q));
            q->v40 = axis;
        }
        p->v190.q = D_002A2DD0;
        break;
    }
    case 3:
        o = p->x1904;
        n = 1;
        break;
    case -1:
        p->x1908 = 0;
        return;
    }
    ReleaseSma(&p->x1034);
    func_001305B8(o, p, 0.0f, 1.0f, n, &p->x1030);
}

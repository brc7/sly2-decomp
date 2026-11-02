typedef unsigned long ulong;
typedef unsigned int uint;
typedef int qword __attribute__((mode(TI)));
struct GEO {int count;qword *points;int planeCount;void *planes;int edgeCount;void *edges;};
struct TREE {int count,a,b;void *nodes;};
struct M4 {qword rows[4];};
struct E;
struct P;
struct VE {short delta,pad;void (*fn)();};
union FLAGS {ulong raw;struct {void *mesh;short state;unsigned short high;} fields;};
struct E {void *object;P *owner;float radius;int tag;FLAGS flags;int id,pinned;M4 matrix;char pad60[8];TREE tree;GEO local,world;int state,stamp;};
struct STATE {qword rows[3];char pad30[0xA0];float mass;char padD4[0xC];};
struct DATA {char pad0[0x38];void *guard;char pad3C[0x24];};
struct P {char pad0[0xC];VE *vt;char pad10[0x30];uint flags;char pad44[0x3C];DATA data;int count;E *entries;STATE *state;};
struct MSG {int bytes,alignment;};
extern char D_002C44F0[];
extern "C" {
void LoadSoXfsFromBrx(P *,void *);
int func_00117C80(void *);
int func_00117D50(void *);
int func_00117DB8(void *);
int func_00117D00(void *);
float func_00117E38(void *);
void ReadGeom__18CBinaryInputStreamP4GEOM(void *,GEO *);
void CBinaryInputStream_ReadBspc(void *,GEO *,TREE *);
void func_00117ED0(void *,STATE *);
void LoadGlobsetFromBrx__FP7GLOBSETP2SOP18CBinaryInputStream(DATA *,P *,void *);
void *func_001AE2A0(MSG *);
void CloneGeom__FP4GEOMP7MATRIX4T0(GEO *,const M4 *,GEO *);
void AddSoParts(P *);
void AdjustSoMomint(P *,float);
void LoadOptionsFromBrx__FPvP18CBinaryInputStream(P *,void *);
void EnsureSoBounds(P *);
void LoadSwObjectsFromBrx__FP2SOP18CBinaryInputStreamP14CLoadCollector(P *,void *,void *);
void func_001A6D28(P *,int);
}
static inline int Extra(P *p){VE *v=&p->vt[63];return ((int (*)(P *))v->fn)((P *)((char *)p+v->delta));}
static inline void *Lookup(P *p,int id){VE *v=&p->vt[51];return ((void *(*)(P *,int))v->fn)((P *)((char *)p+v->delta),id);}
static inline void Post(P *p){VE *v=&p->vt[64];((void (*)(P *))v->fn)((P *)((char *)p+v->delta));}
static inline void SetActive(P *p){uint f=p->flags;if((f^1)&1)*(uint *)f|=8;else p->flags=f|8;}
/** @sly2-annot addr=0x1F31E0
 * Purpose: SO vt14: LoadLo base, read model/globset, SOPART array (XF id, flags, radius, local geom, BSP...),
 *   physics block (matrix, mass), options, bounds
 * Info: proposed name LoadSoFromBrx (T1); class SO slot14; TU so
 * Notes: level_parser CONFIRMED-parse: payload layout followed by the parser, 100%% of bytes in 4 levels
 * Confidence: high [T1 0.90] - evidence: static analysis; sources: r6_1D5150+verdicts_tu_r4r6+parse-verified
 * See: docs/engine/gameplay-objects.md#key-functions, docs/engine/physics-collision.md#7-key-functions,
 *   docs/engine/player.md#11-key-functions
 * @end-sly2-annot */
extern "C" void LoadSoFromBrx(P *p,void *stream)
{
    int i=0;E *e;
    LoadSoXfsFromBrx(p,stream);
    int metadata=func_00117C80(stream);
    LoadGlobsetFromBrx__FP7GLOBSETP2SOP18CBinaryInputStream(&p->data,p,stream);
    int extra=Extra(p);
    p->count=func_00117D50(stream);
    MSG request;
    request.bytes=(p->count+extra)*sizeof(E);request.alignment=16;
    p->entries=(E *)func_001AE2A0(&request);
    for(;i<p->count;i++){
        e=&p->entries[i];
        GEO *local=&e->local;
        e->owner=p;
        e->object=Lookup(p,func_00117D50(stream));
        e->tag=func_00117DB8(stream);
        int f=func_00117D00(stream);
        e->flags.raw=(e->flags.raw&~(1UL<<48))|((ulong)(f&1)<<48);
        f=func_00117D00(stream);
        e->flags.raw=(e->flags.raw&~(1UL<<49))|((ulong)(f&1)<<49);
        e->radius=func_00117E38(stream);
        ReadGeom__18CBinaryInputStreamP4GEOM(stream,local);
        if(!e->object){e->object=D_002C44F0;e->world=e->local;}
        else CloneGeom__FP4GEOMP7MATRIX4T0(local,0,&e->world);
        CBinaryInputStream_ReadBspc(stream,&e->world,&e->tree);
        e->id=func_00117C80(stream);
        e->flags.fields.state=func_00117D50(stream);
        e->pinned=func_00117D50(stream);
        f=func_00117D00(stream);
        e->flags.raw=(e->flags.raw&~(1UL<<50))|((ulong)(f&1)<<50);
    }
    for(i=p->count;i<p->count+extra;i++){e=&p->entries[i];e->flags.fields.state=-10;}
    AddSoParts(p);
    if(p->count>0){
        request.bytes=sizeof(STATE);request.alignment=16;
        STATE *state=(STATE *)func_001AE2A0(&request);
        p->state=state;
        state->mass=func_00117E38(stream);
        func_00117ED0(stream,state);
        AdjustSoMomint(p,1.0f);
        Post(p);
    }
    LoadOptionsFromBrx__FPvP18CBinaryInputStream(p,stream);
    if(p->data.guard)SetActive(p);
    EnsureSoBounds(p);
    LoadSwObjectsFromBrx__FP2SOP18CBinaryInputStreamP14CLoadCollector(p,stream,0);
    func_001A6D28(p,metadata);
}

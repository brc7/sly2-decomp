/* Pick the best supporting contact of a probe object: walk its contact groups, skip contacts
   whose other body sits under a kind-2 object, test the callback (func_001999D0), then probe a
   point offset along the contact direction and reject it if it lies inside the other body's BSP.
   Returns the contact whose other endpoint is highest in Z; *outSide / *outNormal describe it. */
#define FIELD_OFFSET(t,m) ((uint)&(((t*)0)->m))
typedef int qword __attribute__((mode(TI)));
typedef unsigned int uint;
typedef unsigned long ulong;
typedef union { qword q; float f[4]; int i[4]; } U;
typedef struct CONTACT CONTACT;
typedef struct GROUP { CONTACT *first; struct GROUP *next; } GROUP;
typedef struct { char pad[0x64]; GROUP *groups; char pad68[8]; ulong flags; } EC;
typedef struct { ulong low:16;signed int kind:4;ulong high:44; } FLAG;
typedef struct LO {
    char pad[0x10]; struct LO *parent;
    char pad14[0xEC-0x14]; EC *ec;
    char padF0[0x148-0xF0];
    FLAG flags;
} LO;
typedef struct BSPNODE { struct BSPNODE *left,*right; qword *plane; } BSPNODE;
typedef struct { int pad0; LO *owner; char pad8[0x74-8]; BSPNODE *bsp; } SHAPE;
typedef struct { SHAPE *shape; void *spool; LO *object; LO *lo; U position,local,world; } ENDPOINT;
struct CONTACT { ENDPOINT endpoints[2]; U normal; float distance,friction,response; int kind; void *info[2]; int tag; CONTACT *next; U difference; };
typedef struct { LO *self; int state; char pad8[0x1C-8]; int mode; } P;
typedef char U_Size[sizeof(U)==16?1:-1];
typedef char FLAG_Size[sizeof(FLAG)==8?1:-1];
typedef char GROUP_Size[sizeof(GROUP)==8?1:-1];
typedef char ENDPOINT_Size[sizeof(ENDPOINT)==64?1:-1];
typedef char CONTACT_Size[sizeof(CONTACT)==192?1:-1];
typedef char BSPNODE_Size[sizeof(BSPNODE)==12?1:-1];
typedef char P_Size[sizeof(P)==32?1:-1];
typedef char P_self_Offset[FIELD_OFFSET(P,self)==0?1:-1];
typedef char P_state_Offset[FIELD_OFFSET(P,state)==4?1:-1];
typedef char P_mode_Offset[FIELD_OFFSET(P,mode)==28?1:-1];
typedef char LO_parent_Offset[FIELD_OFFSET(LO,parent)==16?1:-1];
typedef char LO_ec_Offset[FIELD_OFFSET(LO,ec)==236?1:-1];
typedef char LO_flags_Offset[FIELD_OFFSET(LO,flags)==328?1:-1];
typedef char EC_groups_Offset[FIELD_OFFSET(EC,groups)==100?1:-1];
typedef char EC_flags_Offset[FIELD_OFFSET(EC,flags)==112?1:-1];
typedef char GROUP_first_Offset[FIELD_OFFSET(GROUP,first)==0?1:-1];
typedef char GROUP_next_Offset[FIELD_OFFSET(GROUP,next)==4?1:-1];
typedef char SHAPE_owner_Offset[FIELD_OFFSET(SHAPE,owner)==4?1:-1];
typedef char SHAPE_bsp_Offset[FIELD_OFFSET(SHAPE,bsp)==116?1:-1];
typedef char ENDPOINT_shape_Offset[FIELD_OFFSET(ENDPOINT,shape)==0?1:-1];
typedef char ENDPOINT_object_Offset[FIELD_OFFSET(ENDPOINT,object)==8?1:-1];
typedef char ENDPOINT_position_Offset[FIELD_OFFSET(ENDPOINT,position)==16?1:-1];
typedef char CONTACT_endpoints_Offset[FIELD_OFFSET(CONTACT,endpoints)==0?1:-1];
typedef char CONTACT_kind_Offset[FIELD_OFFSET(CONTACT,kind)==156?1:-1];
typedef char CONTACT_info_Offset[FIELD_OFFSET(CONTACT,info)==160?1:-1];
typedef char CONTACT_next_Offset[FIELD_OFFSET(CONTACT,next)==172?1:-1];
typedef char CONTACT_difference_Offset[FIELD_OFFSET(CONTACT,difference)==176?1:-1];
extern float D_002C3C60[],D_002BF8A4;
extern int D_002BF8A8;
extern qword D_002A2DC0;
extern char D_003DD490[];
extern int func_00144068(LO *);
extern int func_001999D0(P *,SHAPE *,qword,int,void *,U *);
extern void func_001A7FC0(void *,int,CONTACT **);
extern BSPNODE *func_00118FB0(BSPNODE *,BSPNODE *,qword);
static inline qword Scalar(float f)
{
    qword q;
    __asm__("qmtc2.ni %1,%0":"=j"(q):"r"(f));
    return q;
}
static inline qword Project(qword position,qword normal,qword back,qword axis,qword lift)
{
    register qword n __asm__("vf5")=normal;     /* the one pin: direction in vf5, lift in vf4 as in the original */
    __asm__("vadda.xyzw $ACC,%0,$vf0\n\tvmaddax.xyzw $ACC,%1,%2x\n\tvmaddx.xyzw %0,%3,%4x":"+j"(position):"j"(n),"j"(back),"j"(axis),"j"(lift));
    return position;
}
static inline int Extract(FLAG flags) { return flags.kind; }
static inline int Kind(LO *lo) { return Extract(lo->flags); }
static inline int Side(LO *owner,CONTACT *contact)
{
    uint different=(uint)owner ^ (uint)contact->endpoints[0].object;
    return different!=0;
}
static inline qword InputPosition(CONTACT *c,int side) { return ((ENDPOINT *)(((1-side)<<6)+(uint)c))->position.q; }
static inline ENDPOINT *Opposite(CONTACT *contact,int side)
{
    int one=1;
    return (ENDPOINT *)((char *)contact+((one-side)<<6));
}
/* same endpoint with the offset as the first addend (fixes the addu operand order) */
static inline ENDPOINT *Opposite2(CONTACT *contact,int side)
{
    int one=1;
    return (ENDPOINT *)(((one-side)<<6)+(uint)contact);
}
/** @sly2-annot addr=0x199D48
 * Purpose: Pick the best supporting ledge contact of the player (walk contact groups, FCheckJtEdgeGrab, BSP
 *   point test), returning side and normal
 * Info: proposed name PxpFindJtBestEdge (T1); TU air
 * Confidence: mid (draft) [T1 0.70] - evidence: static naming pass; sources:
 *   r4b_rest+verdicts_A2+names_v2-signature(verified)
 * See: docs/engine/player.md#11-key-functions, docs/reference/functions/player.md#air-0x195950-0x19cbf8
 * @end-sly2-annot */
CONTACT *func_00199D48(P *p,int *outSide,qword *outNormal)
{
    U direction,point,scalar;
    CONTACT *contact;
    GROUP *group;
    CONTACT *best;
    float bestHeight;
    if (func_00144068(p->self)) return 0;
    switch(p->mode) {
    case 1:case 3:case 4:case 5:break;
    default:return 0;
    }
    if(p->state<=16) { if(p->state>=15)return 0; }
    group=p->self->ec->groups;
    best=0;
    for (;group;group=group->next) {
        contact=group->first;
        for (;contact;contact=contact->next) {
            int side=Side(p->self,contact);
            SHAPE *shape=Opposite(contact,side)->shape;
            LO *lo;
            void *info;
            for (lo=shape->owner;lo;lo=lo->parent)
                if (Kind(lo)==2) break;
            if (lo) continue;
            switch (contact->kind) {
            case 1: if (side==0) continue; info=contact->info[0]; break;
            case 3: if (side==0) info=contact->info[1]; else info=contact->info[0]; break;
            case 2: if (side==1) continue; info=contact->info[1]; break;
            default: continue;
            }
            if (!func_001999D0(p,shape,InputPosition(contact,side),contact->kind,info,&direction)) continue;
            if ((p->self->ec->flags&2) || (shape->owner->ec->flags&2))
                func_001A7FC0(D_003DD490,1,&contact);
            if (0.0f < D_002C3C60[side]*contact->difference.f[2]) continue;
            {
                qword back; qword lift; qword value; U *selected;
                selected=&Opposite2(contact,side)->position;
                back=Scalar(-D_002BF8A4);
                /* "rV": the word is a memory operand that fails 'V' at reload, so reload loads it at the asm through the
                   dying %hi register (lui v1 / lw v1,%lo(v1)) instead of a separate pseudo; allocation only */
                __asm__("qmtc2.ni %1,%0":"=j"(lift):"rV"(D_002BF8A8));
                value=Project(selected->q,direction.q,back,D_002A2DC0,lift);
                scalar=(U){back};       /* the original keeps both scalars in one 16-byte stack temporary */
                point.q=value;
                scalar=(U){lift};
                if (func_00118FB0(shape->bsp,(BSPNODE *)0x7FFFFFFF,value)) continue;
                if (best && Opposite(contact,side)->position.f[2] < bestHeight) continue;
                best=contact;
                bestHeight=Opposite(best,side)->position.f[2];
                *outSide=side;
                *outNormal=direction.q;
            }
        }
    }
    return best;
}

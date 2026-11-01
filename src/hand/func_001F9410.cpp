// func_001F9410: solve the contacts of a group of touching objects (Sly 1 SolveSwGroup).
// Builds a DZG over the contacts (func_001585D8), turns every contact into a DZ row with its limits,
// sets each row's target (bounce or separating speed), solves (func_00158B20) and applies the
// impulses (func_00158E20), then tells the objects (vtable callbacks 65/66/67).
// Args: (game, flags, objectCount, objects, contactCount, contacts). flags&1 = resting contacts,
// flags&4 = position pass first (returns early when flags == 4).
typedef int qword __attribute__((mode(TI)));
union U {qword q;float f[4];};
struct XP;
struct VT {short delta,unused;void (*function)(void *);};
struct BODY {char pad0[0x98];short kind;char pad9A[0x16];U velocity;};
struct MOTION {char pad0[0x60];U linear,angular;};
struct P {char pad0[0xC];VT *vtable;char pad10[0x48];BODY *first,*second;char pad60[0x88];MOTION *motion;};
struct XP {char pad0[0x80];U normal;float value;int unused94;float elasticity;int kind;float start,end;int index;int unusedAC;U velocity;};
struct DLE {void *next,*previous;};
struct DL {void *first,*last;int offset;};
union FLOAT {float value;int bits;};
struct DZ {int kind;float lower,upper,value;FLOAT target;int unused14;float *coefficients;int flags;DLE link;};
struct DZS {char pad0[0x40];U normal;XP *source;float *coefficients;char pad58[8];};
struct DZG {int capacity,count;DZ *rows;DL solved,pending,unused,active;int solvedCount;float *matrix,*inverse,*temporary,*results;int contactCount;XP **contacts;int objectCount;P **objects;DZS *states;};
struct CLOCK {float time,dt;};
struct GAME;
struct FRAME;
extern CLOCK D_002DDED0;
extern GAME D_003DD490;
extern "C" {
FRAME *func_001AE478(FRAME *);
void func_001AE4A0(FRAME *,int);
DZG *func_001585D8(DZG *,int,XP **,int,P **);
void func_00158668(DZG *,DZ *,DZS *);
void func_00158988(DZG *,int);
void func_00158E20(DZG *,float,float);
void func_0011A6B8(DZG *);
void func_001A7E60(GAME *,int,XP **);
void func_001A7FC0(GAME *,int,XP **);
float GRandInRange__Fff(float,float);
void func_00158B20(DZG *,int);
void func_001F9C48(P *);
}
// Stack frame mark: ctor 0x1AE478, dtor 0x1AE4A0 (called with the 2.95 in-charge flag 2).
struct FRAME {FRAME(){func_001AE478(this);}~FRAME(){func_001AE4A0(this,2);}};

#define FLT_MAX 3.40282347e+38f

// The second "j"(b) operand is allocation-only: it puts b in vf1 like the game.
static inline int DotQ(qword a,qword b){qword one;int r;__asm__("vmul.xyz %0,%0,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"+j"(a),"=&j"(one):"j"(b),"j"(b));__asm__("qmfc2.ni %0,%1":"=r"(r):"j"(a));return r;}
static inline float Dot(qword a,qword b){qword one;float f;__asm__("vmul.xyz %0,%0,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"+j"(a),"=&j"(one):"j"(b));__asm__("mtc1 %1,%0":"=f"(f):"r"(a));return f;}
// "memory": the game re-reads p->second after this dot product.
static inline float LengthSquared(qword v){qword one;float f;__asm__("vmul.xyz %0,%0,%0\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"+j"(v),"=&j"(one): :"memory");__asm__("mtc1 %1,%0":"=f"(f):"r"(v));return f;}
static inline float Sqrt(float f){float r;__asm__ volatile("sqrt.s %0,%1":"=f"(r):"f"(f));return r;}
static inline float ClockDt(){return D_002DDED0.dt;}
static inline void Callback(P *p,int slot){VT *vt=p->vtable;vt[slot].function((char *)p+vt[slot].delta);}

extern "C" void func_001F9410(GAME *game,int flags,int objectCount,P **objects,int contactCount,XP **contacts)
{
    FRAME frame;
    XP *contact;DZ *row;int i;
    DZG group;
    func_001585D8(&group,contactCount,contacts,objectCount,objects);
    for(i=0;i<contactCount;i++){
        contact=contacts[i];
        row=&group.rows[i];
        DZS *state=&group.states[i];
        state->source=contact;
        state->normal=contact->normal;
        switch(contact->kind){
        case 11:
            row->kind=1;
            row->lower=-FLT_MAX;
            // Shares the default's upper limit (the game's code jumps into it; a separate store does not match).
            goto unbounded;
        case 14:
            if(flags&1){
                row->lower=contact->start*ClockDt();
                row->upper=contact->end*ClockDt();
                row->kind=contact->start<0.0f;
            }else{
                row->lower=0.0f;
                row->upper=FLT_MAX;
                row->kind=0;
            }
            break;
        default:
            row->kind=0;
            row->lower=0.0f;
        unbounded:
            row->upper=FLT_MAX;
            break;
        }
        func_00158668(&group,row,state);
    }
    float maximum;
    if(flags&4){
        maximum=0.0f;
        for(i=0;i<contactCount;i++){
            contact=contacts[i];
            row=&group.rows[i];
            if(row->kind==1)row->target.value=contact->value;else row->target.value=contact->value+1.0f;
            float magnitude=__builtin_fabsf(contact->value);
            if(maximum<magnitude)maximum=magnitude;
        }
        func_00158988(&group,4);
        func_00158E20(&group,maximum+maximum,6.2831855f);
        {int j;for(j=0;j<objectCount;j++){}
        for(j=0;j<objectCount;j++)Callback(objects[j],67);}
        if(flags==4)return;
        func_0011A6B8(&group);
    }
    maximum=0.0f;
    // The else loop starts its counter from the flag (known 0 there): the game tests `flag < contactCount`.
    int fResting=flags&1;
    int j=fResting;
    if(fResting){
        func_001A7E60(game,contactCount,contacts);
        func_001A7FC0(game,contactCount,contacts);
        int n;for(n=0;n<contactCount;n++){
            contact=contacts[n];
            row=&group.rows[n];
            row->target.bits=DotQ(contact->velocity.q,contact->normal.q);
            float magnitude=LengthSquared(contact->velocity.q);
            if(maximum<magnitude)maximum=magnitude;
        }
    }else{
        for(;j<contactCount;j++){
            contact=contacts[j];
            row=&group.rows[j];
            float factor=1.0f+contact->elasticity*GRandInRange__Fff(0.9f,1.1f);
            row->target.value=Dot(contact->velocity.q,contact->normal.q)*factor;
            float magnitude=LengthSquared(contact->velocity.q);
            if(maximum<magnitude)maximum=magnitude;
        }
    }
    func_00158B20(&group,flags);
    float magnitude=Sqrt(maximum);
    func_00158E20(&group,magnitude+magnitude,50.0f);
    int k;for(k=0;k<objectCount;k++){}
    if(fResting){
        for(k=0;k<objectCount;k++)Callback(objects[k],65);
        for(k=0;k<objectCount;k++){
            P *p=objects[k];
            if(p->first->kind==2||p->second->kind==2)break;
            BODY *second=p->second;
            MOTION *motion=p->motion;
            int moving=LengthSquared(p->first->velocity.q)>25.0f || Dot(second->velocity.q,p->second->velocity.q)>0.0025000002f || LengthSquared(motion->linear.q)>25.0f || LengthSquared(motion->angular.q)>0.0025000002f;
            if(moving)break;
        }
        if(k>=objectCount){
            for(k=0;k<objectCount;k++)func_001F9C48(objects[k]);
            func_001A7E60(&D_003DD490,contactCount,contacts);
            func_001A7FC0(&D_003DD490,contactCount,contacts);
        }
    }else{
        for(k=0;k<objectCount;k++)Callback(objects[k],66);
    }
}

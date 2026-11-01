typedef int qword __attribute__((mode(TI)));
union U {qword q;float f[4];};
struct M3 {U rows[3];};
struct M4 {M3 basis;U position;};
struct FRAME {M4 matrix,world;};
struct VENTRY {short delta,pad;void (*fn)();};
struct REF {char pad0[0x70];U position;char pad80[0x20];FRAME *parent;};
struct POLY {float a,b,c;};
struct LIMITS {float low,high;};
struct P;
struct ATTACH {char pad0[0x10];P *owner;int index,id;void *object;void *binding;char pad24[8];int meshId;REF *mesh;void *meshBinding;int anchorId;P *anchor;LIMITS angleLimits,speedLimits;POLY flightTime;int unused5C;LIMITS flightLimits;float gravity;int stateIndex,state;float angle,speed;char pad7C[0x24];int active;char padA4[0xC];};
struct ENTRY {int id;REF *reference;int unknown;float steering;char pad10[4];float tilt;char pad18[0x48];};
struct SIDE {int id;REF *reference;void *binding;LIMITS angleLimits;};
struct SOUND {char pad0[0x4E];short references;};
struct P {char pad0[0xC];VENTRY *vt;char pad10[0x50];FRAME *space;char pad64[0x11C];int mode;char pad184[0xC];int count;char pad194[0xC];ENTRY entries[8];float width;int sideCount;SIDE sides[4];int attachCount;ATTACH *attachments[8];char pad51C[0x24];float heading,goalHeading,steering,wheelAngle,drive,turn,acceleration;char pad55C[0x3C];SOUND *sound;int unused59C;float soundRate;P *target;char pad5A8[0x18];float lastCorrection;};
struct CLOCK {float time,dt;};
extern CLOCK D_002DDED0;
extern const qword D_002A2DC0;
extern POLY D_002C2040,D_002C2060,D_002C2080;
extern LIMITS D_002C2050,D_002C2070,D_002C2090;
extern float D_002C2030,D_002C20A4;
extern char D_002C2098[],D_002C20A8[];
extern "C" {
void func_001F36E0(P *);
float func_002458F0(float,float);
int func_001D4E08(ATTACH *);
void func_001F0660(void *,int);
void func_001D2150(P *);
void func_001D2610(P *);
float func_0011CF60(float);
qword func_0011E1A0(float,float,float);
qword func_0022FCC8(FRAME *,FRAME *,qword);
float func_0011D948(const LIMITS *,float);
void func_0012D108(void *,qword);
float func_0011D2D0(float,float,float,void *,float *);
qword func_001A62E8(P *);
int func_0011D4D0(float,float,float);
float func_0011D018(float,float,float,void *,float *);
void func_001FB1F8(SOUND *,float);
void func_001FB2C0(SOUND *,float);
void func_001FB060(SOUND *);
void func_001F5078(P *);
}
static inline float X(qword q){float f;__asm__("mtc1 %1,%0":"=f"(f):"r"(q));return f;}
static inline float Y(qword q){qword r;__asm__("prot3w %0,%1":"=r"(r):"r"(q));return X(r);}
static inline qword Pack(float x,float y,float z){qword a,b;__asm__("mfc1 %0,%2\n\tmfc1 %1,%3\n\tpextlw %0,%1,%0\n\tmfc1 %1,%4\n\tpcpyld %0,%1,%0":"=&r"(a),"=&r"(b):"f"(x),"f"(y),"f"(z));return a;}
static inline qword PackMesh(float x,float y,float z){register qword a __asm__("$2"),b __asm__("$3");__asm__("mfc1 %0,%2\n\tmfc1 %1,%3\n\tpextlw %0,%1,%0\n\tmfc1 %1,%4\n\tpcpyld %0,%1,%0":"=&r"(a),"=&r"(b):"f"(x),"f"(y),"f"(z));return a;}
static inline qword Dot(qword a,qword b){qword r,one;__asm__("vmul.xyz %0,%2,%3\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"=j"(r),"=&j"(one):"j"(a),"j"(b));return r;}
static inline float ShotLength(qword a){qword r=Dot(a,a);__asm__("vsqrt Q,%0x\n\tvwaitq\n\tvaddq.x %0,$vf0,Q":"+j"(r));register float value __asm__("$3");__asm__("qmfc2.ni %0,%1":"=r"(value):"j"(r));return value;}
static inline float DistanceAndVelocity(qword a,qword velocity,U&home){qword r=a;asm("vmul.xy %0,%0,%0\n\tvaddy.x %0,%0,%0y\n\tsq %2,%1":"+j"(r),"=m"(home.q):"r"(velocity));asm("vsqrt Q,%0x\n\tvwaitq\n\tvaddq.x %0,$vf0,Q":"+j"(r));register float value __asm__("$2");asm("qmfc2.ni %0,%1":"=r"(value):"j"(r));return value;}
static inline float Evaluate(const POLY &p,float t){return p.a+t*(p.b+t*p.c);}
static inline float Clamp(float v,float low,float high){if(v<low)return low;else if(high<v)return high;else return v;}
static inline float Abs(float f){return __builtin_fabsf(f);}
static inline void ResetDrive(P *p){
 register float zero __asm__("$f1"),heading __asm__("$f0");
 __asm__("mtc1 $zero,%0\n\t"
         "lwc1 %1,%2"
         : "=f"(zero),"=f"(heading)
         : "m"(p->heading));
 p->turn=zero;
 p->drive=zero;
 p->goalHeading=heading;
}
static inline M4*GetMatrixFrom(P*p,VENTRY*vt){VENTRY&e=vt[31];return ((M4*(*)(void*))e.fn)((char*)p+e.delta);}
static inline qword GetVelocity(P *p){VENTRY &e=p->vt[33];return ((qword (*)(void *))e.fn)((char *)p+e.delta);}
/** @sly2-annot addr=0x1D19A8
 * Purpose: CID 77 vt16 Update: UpdateSo; heading from world matrix; weapon ready SMAs; mode 2 player input /
 *   mode 1 AI / else idle; steering and throttle from polynomial tables of heading error; turret/actuator
 *   goal; engine sound volume/pitch
 * Info: proposed name UpdateRobottank (T1); class ROBOTTANK slot16; TU tu_1D1370
 * Notes: names_v2 C proposed RenderAloGlobset__FP3ALOP2CMP2RO (0.638) | class identity from whole_game WE
 *   names/object strings: UpdateRcCar -> UpdateRobottank
 * Confidence: mid (draft) [T1 0.75] - evidence: static naming pass; sources: r5b_rest+whole_game(class
 *   identity)
 * See: docs/reference/functions/gameplay-objects.md#tu_1d1370-0x1d1370-0x1d5150
 * @end-sly2-annot */
extern "C" void func_001D19A8(P *p)
{
    U scalar,position,targetPosition,targetVelocity,shot;
    int i=0;
    func_001F36E0(p);
    p->heading=func_002458F0(p->space->world.basis.rows[0].f[1],p->space->world.basis.rows[0].f[0]);
    if(p->attachCount>0){
        ATTACH **current=p->attachments;
        do {
            ATTACH *attach=*current;
            if(!func_001D4E08(attach))func_001F0660(attach->binding,0x65);
            ++i;++current;
        }while(i<p->attachCount);
    }
    switch(p->mode){
    case 0:case 3:ResetDrive(p);break;
    case 2:func_001D2150(p);break;
    case 1:func_001D2610(p);break;
    }
    register float angle __asm__("$f0")=func_0011CF60(p->goalHeading-p->heading);
    p->steering=Clamp(Evaluate(D_002C2040,angle),D_002C2050.low,D_002C2050.high);
    angle=Abs(func_0011CF60(p->goalHeading-p->heading));
    p->acceleration=Clamp(Evaluate(D_002C2060,angle),D_002C2070.low,D_002C2070.high);
    angle=func_0011CF60(p->goalHeading-p->heading);
    struct LOWER_PAGE {char before[0x2090];LIMITS limits;};
    typedef char lower_page_low_offset[((unsigned)&((LOWER_PAGE*)0)->limits.low==0x2090)?1:-1];
    typedef char lower_page_view_size[(sizeof(LOWER_PAGE)==0x2098)?1:-1];
    register LOWER_PAGE *lowerPage __asm__("$2");
    float turnFactor;register unsigned boundsPage __asm__("$4");
    asm("lui %0,%%hi(%1)":"=r"(lowerPage):"i"(&D_002C2090));
    asm volatile("abs.s %0,%0\n\tlwc1 %1,%3\n\tlui %2,%%hi(%4)"
        :"+f"(angle),"=f"(turnFactor),"=r"(boundsPage):"m"(lowerPage->limits.low),"i"(&D_002C2090));
    register float response __asm__("$f0")=Evaluate(D_002C2080,angle);
    register LIMITS*bounds __asm__("$3");
    asm("addiu %0,%1,%%lo(%2)":"=r"(bounds):"r"(boundsPage),"i"(&D_002C2090));
    float high=bounds->high;
    if(response<turnFactor)goto response_done;
    if(high<response)turnFactor=high;
    else turnFactor=response;
 response_done:;
    float limited=D_002C2030;VENTRY*table;
    asm("lwc1 $f3,%4\n\tmin.s %0,%0,%2\n\tlwc1 $f2,%5\n\tmul.s %0,%3,%0\n\tlw %1,%6\n\tmul.s %0,%0,$f3\n\tdiv.s %0,%0,$f2"
        :"+f"(limited),"=r"(table):"f"(p->drive),"f"(turnFactor),"m"(p->steering),"m"(p->width),"m"(p->vt):"$f2","$f3");
    p->turn=limited;
    if(GetMatrixFrom(p,table)->basis.rows[2].f[2]>0.5f)p->lastCorrection=D_002DDED0.time;
    i=0;
    if(p->sideCount>0){
        LIMITS*limits=&p->sides[0].angleLimits;SIDE*side=p->sides;
        do {
            ++i;
            qword direction=func_0011E1A0(p->wheelAngle,1.0f,0.0f);
            direction=func_0022FCC8(0,side->reference->parent,direction);
            float orientation=func_002458F0(Y(direction),X(direction));
            scalar.q=Pack(0.0f,0.0f,func_0011D948(limits,orientation));
            limits=(LIMITS*)((char*)limits+sizeof(SIDE));
            func_0012D108(side->binding,scalar.q);
            ++side;
        }while(i<p->sideCount);
    }
    for(int k=0;k<p->count;k++){
        ENTRY *entry=&p->entries[k];
        float desired;
        if(p->drive!=0.0f)desired=entry->steering*p->steering;else desired=0.0f;
        entry->tilt=func_0011D2D0(entry->tilt,desired,D_002DDED0.dt,D_002C2098,0);
    }
    i=0;
    qword currentPosition=func_001A62E8(p);
    asm("sq %1,%0":"=m"(position.q):"r"(currentPosition):"memory");
    targetPosition.q=func_001A62E8(p->target);
    qword returnedSpeed=GetVelocity(p->target);
    register qword targetSpeed __asm__("$3");int attachmentCount;
    {register int loadedCount __asm__("$2");
    asm("por %0,$zero,%2\n\tlw %1,%3":"=&r"(targetSpeed),"=r"(loadedCount):"r"(returnedSpeed),"m"(p->attachCount));
    attachmentCount=loadedCount;}
    if(attachmentCount>0){
        register qword targetVector __asm__("vf2")=targetPosition.q;
        const qword *up=&D_002A2DC0;
        register qword originVector __asm__("vf3")=position.q;
        register qword difference __asm__("vf1");
        asm("vsub.xyzw %0,%1,%2":"=j"(difference):"j"(targetVector),"j"(originVector));
        float distance=DistanceAndVelocity(difference,targetSpeed,targetVelocity);
        do {
            P*slotBase;
            asm("addu %0,%1,%2":"=r"(slotBase):"r"(p),"r"(i<<2));
            ATTACH *attach=slotBase->attachments[0];
            if(!attach->mesh)goto aim_next;
            if(p->drive!=0.0f || !func_001D4E08(attach)){
                attach->active=0;attach->speed=attach->speedLimits.low;attach->angle=0.0f;
            }else{
                float time=Clamp(Evaluate(attach->flightTime,distance),attach->flightLimits.low,attach->flightLimits.high);
                register qword origin __asm__("vf1");
                if(attach->anchor)origin=func_001A62E8(attach->anchor);else origin=attach->mesh->position.q;
                register float half __asm__("$f2")=-0.5f,inverse __asm__("$f1")=1.0f,gravity __asm__("$f0");
                register qword target __asm__("vf2"),axis __asm__("vf3"),delta __asm__("vf5"),velocity __asm__("vf4");
                asm("mul.s %0,%4,%0\n\tlwc1 %2,%5\n\tdiv.s %1,%1,%4\n\tlqc2 %3,%6"
                    :"+f"(half),"+f"(inverse),"=&f"(gravity),"=j"(target)
                    :"f"(time),"m"(attach->gravity),"m"(targetPosition.q));
                asm("lqc2 %0,%4\n\tvsub.xyzw %1,%5,%6\n\tlqc2 %2,%7\n\tmul.s %3,%3,%8"
                    :"=&j"(axis),"=j"(delta),"=j"(velocity),"+f"(gravity)
                    :"m"(*up),"j"(target),"j"(origin),"m"(targetVelocity.q),"f"(half));
                register qword ray __asm__("vf1");
                asm("vmul.xyz %0,%1,%2\n\tvaddw.x $vf2,$vf0,$vf0w\n\tvadday.x ACC,%0,%0y\n\tvmaddz.x %0,$vf2,%0z\n\tvmulx.xyzw %0,%2,%0x\n\tvsub.xyzw %0,%1,%0"
                    :"=&j"(ray):"j"(velocity),"j"(axis):"vf2");
                register qword gravityScalar __asm__("vf4"),inverseScalar __asm__("vf2");
                register int gravityWord __asm__("$2"),inverseWord __asm__("$3");
                asm("mfc1 %2,%4\n\tmfc1 %3,%5\n\tqmtc2.ni %2,%0\n\tqmtc2.ni %3,%1"
                    :"=j"(gravityScalar),"=j"(inverseScalar),"=&r"(gravityWord),"=&r"(inverseWord):"f"(gravity),"f"(inverse));
                asm("vadda.xyzw ACC,%0,$vf0\n\tvmaddax.xyzw ACC,%5,%4x\n\tvmaddx.xyzw %0,%1,%3x\n\tsqc2 %4,%2\n\tvmul.xy %1,%0,%0\n\tvaddy.x %1,%1,%1y"
                    :"+j"(ray),"+j"(axis),"=m"(scalar.q):"j"(gravityScalar),"j"(inverseScalar),"j"(delta));
                register qword pitchWord __asm__("$4"),lengthWord __asm__("$2");
                register float pitchZ __asm__("$f12"),planarLength __asm__("$f13");
                asm("qmfc2.ni %1,%5\n\tvsqrt Q,%0x\n\tvwaitq\n\tvaddq.x %0,$vf0,Q\n\tpextuw %1,$zero,%1\n\tqmfc2.ni %2,%0\n\tmtc1 %1,%3\n\tmtc1 %2,%4"
                    :"+j"(axis),"=&r"(pitchWord),"=r"(lengthWord),"=f"(pitchZ),"=f"(planarLength):"j"(ray));
                shot.q=ray;
                scalar.q=gravityScalar;
                float pitch=func_002458F0(pitchZ,planarLength);
                float speed=ShotLength(shot.q);
                attach->angle=func_0011D948(&attach->angleLimits,pitch);
                attach->speed=func_0011D948(&attach->speedLimits,speed);
                int active=func_0011D4D0(attach->angle,pitch,0.01f) && func_0011D4D0(attach->speed,speed,0.01f);
                attach->active=active;
                if(!active){attach->angle=0.0f;attach->speed=attach->speedLimits.low;}
            }
            {register void*binding __asm__("$4");float angle;
            asm("lwc1 %0,%2\n\tlw %1,%3":"=f"(angle),"=r"(binding):"m"(attach->angle),"m"(attach->meshBinding));
            scalar.q=PackMesh(0.0f,-angle,0.0f);
            func_0012D108(binding,scalar.q);}
        aim_next:
            ++i;
        }while(i<p->attachCount);
    }
    if(p->sound){
        p->soundRate=func_0011D018(p->soundRate,p->drive/D_002C20A4,D_002DDED0.dt,D_002C20A8,0);
        float rate=Clamp(p->soundRate,0.2f,1.0f);
        func_001FB1F8(p->sound,rate);
        func_001FB2C0(p->sound,rate*0.2f);
        if(p->sound->references>0)func_001FB060(p->sound);
    }
    if(p->drive!=0.0f || p->turn!=0.0f)func_001F5078(p);
}

#define OFF(T,m) ((unsigned long)&((T*)0)->m)
typedef char guard_0[(OFF(FRAME,world)==64)?1:-1];
typedef char guard_1[(OFF(REF,position)==112)?1:-1];
typedef char guard_2[(OFF(REF,parent)==160)?1:-1];
typedef char guard_3[(OFF(P,vt)==12)?1:-1];
typedef char guard_4[(OFF(P,space)==96)?1:-1];
typedef char guard_5[(OFF(P,mode)==384)?1:-1];
typedef char guard_6[(OFF(P,count)==400)?1:-1];
typedef char guard_7[(OFF(P,entries)==416)?1:-1];
typedef char guard_8[(OFF(P,width)==1184)?1:-1];
typedef char guard_9[(OFF(P,sideCount)==1188)?1:-1];
typedef char guard_10[(OFF(P,sides)==1192)?1:-1];
typedef char guard_11[(OFF(P,attachCount)==1272)?1:-1];
typedef char guard_12[(OFF(P,attachments)==1276)?1:-1];
typedef char guard_13[(OFF(P,heading)==1344)?1:-1];
typedef char guard_14[(OFF(P,goalHeading)==1348)?1:-1];
typedef char guard_15[(OFF(P,steering)==1352)?1:-1];
typedef char guard_16[(OFF(P,wheelAngle)==1356)?1:-1];
typedef char guard_17[(OFF(P,drive)==1360)?1:-1];
typedef char guard_18[(OFF(P,turn)==1364)?1:-1];
typedef char guard_19[(OFF(P,acceleration)==1368)?1:-1];
typedef char guard_20[(OFF(P,sound)==1432)?1:-1];
typedef char guard_21[(OFF(P,soundRate)==1440)?1:-1];
typedef char guard_22[(OFF(P,target)==1444)?1:-1];
typedef char guard_23[(OFF(P,lastCorrection)==1472)?1:-1];
typedef char guard_24[(OFF(ENTRY,steering)==12)?1:-1];
typedef char guard_25[(OFF(ENTRY,tilt)==20)?1:-1];
typedef char guard_26[(OFF(SIDE,reference)==4)?1:-1];
typedef char guard_27[(OFF(SIDE,binding)==8)?1:-1];
typedef char guard_28[(OFF(SIDE,angleLimits)==12)?1:-1];
typedef char guard_29[(OFF(ATTACH,owner)==16)?1:-1];
typedef char guard_30[(OFF(ATTACH,id)==24)?1:-1];
typedef char guard_31[(OFF(ATTACH,binding)==32)?1:-1];
typedef char guard_32[(OFF(ATTACH,meshId)==44)?1:-1];
typedef char guard_33[(OFF(ATTACH,mesh)==48)?1:-1];
typedef char guard_34[(OFF(ATTACH,meshBinding)==52)?1:-1];
typedef char guard_35[(OFF(ATTACH,anchor)==60)?1:-1];
typedef char guard_36[(OFF(ATTACH,angleLimits)==64)?1:-1];
typedef char guard_37[(OFF(ATTACH,speedLimits)==72)?1:-1];
typedef char guard_38[(OFF(ATTACH,flightTime)==80)?1:-1];
typedef char guard_39[(OFF(ATTACH,flightLimits)==96)?1:-1];
typedef char guard_40[(OFF(ATTACH,gravity)==104)?1:-1];
typedef char guard_41[(OFF(ATTACH,stateIndex)==108)?1:-1];
typedef char guard_42[(OFF(ATTACH,state)==112)?1:-1];
typedef char guard_43[(OFF(ATTACH,angle)==116)?1:-1];
typedef char guard_44[(OFF(ATTACH,speed)==120)?1:-1];
typedef char guard_45[(OFF(ATTACH,active)==160)?1:-1];
typedef char guard_46[(OFF(SOUND,references)==78)?1:-1];
typedef char guard_47[(OFF(CLOCK,dt)==4)?1:-1];
typedef char guard_48[(sizeof(U)==16)?1:-1];
typedef char guard_49[(sizeof(M3)==48)?1:-1];
typedef char guard_50[(sizeof(M4)==64)?1:-1];
typedef char guard_51[(sizeof(FRAME)==128)?1:-1];
typedef char guard_52[(sizeof(REF)==176)?1:-1];
typedef char guard_53[(sizeof(ENTRY)==96)?1:-1];
typedef char guard_54[(sizeof(SIDE)==20)?1:-1];
typedef char guard_55[(sizeof(ATTACH)==176)?1:-1];
typedef char guard_56[(sizeof(VENTRY)==8)?1:-1];
typedef char guard_57[(sizeof(POLY)==12)?1:-1];
typedef char guard_58[(sizeof(LIMITS)==8)?1:-1];
typedef char guard_59[(sizeof(int)==4)?1:-1];
typedef char guard_60[(sizeof(short)==2)?1:-1];
typedef char guard_61[(sizeof(float)==4)?1:-1];
typedef char guard_62[(sizeof(void*)==4)?1:-1];
typedef char guard_63[(sizeof(qword)==16)?1:-1];
typedef char guard_64[(__alignof__(U)==16)?1:-1];
typedef char guard_65[(sizeof(((P*)0)->attachments)==32)?1:-1];
typedef char guard_66[(OFF(P,attachments)+sizeof(((P*)0)->attachments)==0x51C)?1:-1];
typedef char guard_67[(sizeof(long)==8)?1:-1];
typedef char guard_68[(OFF(POLY,a)==0)?1:-1];
typedef char guard_69[(OFF(POLY,b)==4)?1:-1];
typedef char guard_70[(OFF(POLY,c)==8)?1:-1];
typedef char guard_71[(OFF(LIMITS,low)==0)?1:-1];
typedef char guard_72[(OFF(LIMITS,high)==4)?1:-1];
typedef char guard_73[(OFF(VENTRY,delta)==0)?1:-1];
typedef char guard_74[(OFF(VENTRY,fn)==4)?1:-1];
typedef char guard_75[(OFF(M4,basis)==0)?1:-1];
typedef char guard_76[(OFF(M4,position)==0x30)?1:-1];
typedef char guard_77[(OFF(CLOCK,time)==0)?1:-1];
typedef char guard_78[(OFF(P,entries[1])-OFF(P,entries[0])==0x60)?1:-1];
typedef char guard_79[(OFF(P,sides[1])-OFF(P,sides[0])==0x14)?1:-1];
typedef char guard_80[(OFF(P,attachments[1])-OFF(P,attachments[0])==4)?1:-1];

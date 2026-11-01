// Select and normalize three animation blend weights.
extern "C" {
struct REF {char pad50[0x50];unsigned short flags,state;};
struct COEF {float a,b,c;};
struct PLAYER {char pad4C8[0x4C8];int action;char pad4CC[0xBFC];int mode;char pad10CC[0x20];int state;};
struct P {char pad5C[0x5C];int modeA,modeB,selected;char pad68[0xA0];float weights[3];struct COEF coefficients[3];char pad138[0xCC];int count;struct REF *refs[8];};
extern char D_003DD490[];
extern float D_002DDF10;
struct PLAYER *PpoCur(void *);
int FIsBasicDerivedFrom__FP5BASIC3CID(void *,int);
float GSmooth__FfffP3SMPPf(float,float,float,struct COEF *,float *);
void func_001FC2E0(struct REF *,int,float);
static __inline__ float Sqrt(float f){float r;__asm__ __volatile__("sqrt.s %0,%1":"=f"(r):"f"(f));return r;}
void func_001FDC08(struct P *input)
{
    struct P *p=input;
    struct REF *ref;
    float *weights;
    int i;
    float sum,selectedSquare;
    p->selected=1;
    if(p->modeA)p->selected=2;
    else if(p->modeB)p->selected=0;
    else if(PpoCur(D_003DD490) && FIsBasicDerivedFrom__FP5BASIC3CID(PpoCur(D_003DD490),0x34)){
        struct PLAYER *player=PpoCur(D_003DD490);
        if(player->state==3)p->selected=0;
        else if(player->state==4)p->selected=2;
        else if(player->mode!=-1)p->selected=0;
        else if(player->action>=0){if(player->action<3 || player->action==13)p->selected=0;}
    }
    if(p->count>0)ref=((struct REF **)&p->count)[p->count];else ref=0;
    if(ref){
        int locked=0;
        if(ref->state || (ref->flags&1))locked=1;
        if(!locked)goto smooth;
    }
    p->weights[p->selected]=1.0f;
    goto norm;
smooth:
    p->weights[p->selected]=GSmooth__FfffP3SMPPf(p->weights[p->selected],1.0f,D_002DDF10,&p->coefficients[p->selected],0);
norm:
    sum=0.0f;selectedSquare=p->weights[p->selected]*p->weights[p->selected];
    for(i=0;i<3;i++)if(i!=p->selected)sum+=p->weights[i]*p->weights[i];
    if(sum<0.0001f){
        for(i=0;i<3;i++){
            p->weights[i]=(i==p->selected)?1.0f:0.0f;
        }
    }else{
        float gain=Sqrt((1.0f-selectedSquare)/sum);
        for(i=0;i<3;i++)if(i!=p->selected)p->weights[i]*=gain;
    }
    if(ref){func_001FC2E0(ref,0,p->weights[0]);func_001FC2E0(ref,1,p->weights[1]);func_001FC2E0(ref,2,p->weights[2]);}
}

}

class X;
typedef void (X::*PMF)();
struct P {char pad0[0x13C];float weights[8][4];int pad1BC;float outputs[4];char pad1D0[0x30];X *object;int count;X *refs[8];X *component;};
struct LM {float low,high;};
extern LM D_002BBB60;
extern PMF D_002A8298,D_002A82A0;
extern "C" float GLimitLm__FP2LMf(LM *,float);
extern "C" void *func_002333B4(void *,int,unsigned int);
extern "C" void snd_SetMasterVolume(int,int);
static inline X *ReadReceiver(X **pp)
{
    register X *receiver __asm__("$4");
    __asm__("lw %0,%1":"=r"(receiver):"m"(*pp));
    return receiver;
}
/** @sly2-annot addr=0x1FEA60
 * Purpose: Set weight[category<8][channel<4], recompute channel output and send snd_SetMasterVolume; update
 *   live streams
 * Info: proposed name SetSndmgrChannelVolume (T1); TU sound
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
extern "C" void SetSndmgrChannelVolume(P *p,int channel,int category,float input)
{
    if((unsigned int)category>=8)return;
    if(channel<0)return;
    if(channel>=4)return;
    input=GLimitLm__FP2LMf(&D_002BBB60,input);
    float (*w)[4]=p->weights;
    if(p->weights[category][channel]==input)return;
    if(category==2){
        PMF action;
        func_002333B4(&action,0,8);
        if(input==0.0f)action=D_002A8298;
        else if(w[2][channel]==0.0f)action=D_002A82A0;
        if(action!=0){
            switch(channel){
            case 0:if(p->component)(p->component->*action)();break;
            case 1:for(int i=0;i<p->count;i++){X **pp=&p->refs[i];X *o=ReadReceiver(pp);if(o){union {X *p;unsigned w;} r;r.p=o;(((X *)r.w)->*action)();}}break;
            case 2:if(p->object)(p->object->*action)();break;
            }
        }
    }
    p->weights[category][channel]=input;
    input=1.0f;
    for(int i=0;i<8;i++)input*=p->weights[i][channel];
    int next=(int)(input*1024.0f);
    if((int)(p->outputs[channel]*1024.0f)!=next){
        p->outputs[channel]=input;
        snd_SetMasterVolume(channel,next);
    }
}

typedef char layout_pointer[(sizeof(void*)==4)?1:-1];
typedef char layout_int[(sizeof(int)==4)?1:-1];
typedef char layout_float[(sizeof(float)==4)?1:-1];
typedef char layout_member_pointer[(sizeof(PMF)==8)?1:-1];
typedef char layout_limit[(sizeof(LM)==8)?1:-1];
typedef char layout_limit_low[((unsigned)&((LM*)0)->low==0)?1:-1];
typedef char layout_limit_high[((unsigned)&((LM*)0)->high==4)?1:-1];
typedef char layout_weights[((unsigned)&((P*)0)->weights==0x13C)?1:-1];
typedef char layout_weight_stride[(sizeof(((P*)0)->weights[0])==0x10)?1:-1];
typedef char layout_outputs[((unsigned)&((P*)0)->outputs==0x1C0)?1:-1];
typedef char layout_object[((unsigned)&((P*)0)->object==0x200)?1:-1];
typedef char layout_count[((unsigned)&((P*)0)->count==0x204)?1:-1];
typedef char layout_references[((unsigned)&((P*)0)->refs==0x208)?1:-1];
typedef char layout_component[((unsigned)&((P*)0)->component==0x228)?1:-1];
typedef char layout_size[(sizeof(P)==0x22C)?1:-1];

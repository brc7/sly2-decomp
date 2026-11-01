// Flush the sprite render queue and pair begin/end VU microprogram packets.
typedef unsigned int uint;
typedef unsigned char uchar;
struct V4 {float x,y,z,w;} __attribute__((aligned(16)));
struct PARAM {
    int mode,pad4;float priority;int padC;V4 offset,scale;
    PARAM(float p){offset.x=0.0f;offset.y=0.0f;offset.z=0.0f;offset.w=0.0f;scale.x=0.0f;scale.y=0.0f;scale.z=0.0f;scale.w=0.0f;priority=p;mode=1;} PARAM(){}
};
struct Q {uchar color0[4],color1[4];char pad8[8];char matrix[0x20];void *payload;int kind;char pad38[0x29];uchar channel;char pad62[6];Q *next;};
struct LIST {Q *first,*last;};
struct S {char pad[0x84];void *sprites;char pad88[0x74];LIST queue;};
struct R {unsigned char *base;int pad[2];unsigned char *cur;};
extern R D_002DEA28;
extern "C" {
void func_001A5ED8(S *);
int func_002132D0(S *);
void func_002185D0(void *);
void func_002130C0(S *);
void func_00217298(S *,int,float);
void func_002159E0(void *,void *,PARAM *,int);
void func_0011A060(LIST *);
}
static inline void Align(R *r){while((uint)r->cur&63){*(uint *)r->cur=0;r->cur+=4;}}
static inline float Min(float x,float y){return y<x?y:x;}
static inline float Max(float x,float y){return x<y?y:x;}
static inline void Offset(V4 &v,float x,float y,float z,float w){v.x=x-127.0f;v.y=y-127.0f;v.z=z-127.0f;v.w=w-127.0f;}
static inline void SetOffset(V4 &v,const uchar *c){Offset(v,c[0],c[1],c[2],c[3]);}
static inline float ColorScale(int c,float upper){return Max(Min((c-127)*0.007874016f,upper),-1.0f);}
static inline void Scale(V4 &v,int a,int b,int d,int e,float upper){float x=ColorScale(a,upper),y=ColorScale(b,upper),z=ColorScale(d,upper),w=ColorScale(e,upper);v.x=x;v.y=y;v.z=z;v.w=w;}
static inline void SetScale(V4 &v,const Q *q,float upper){Scale(v,q->color1[0],q->color1[1],q->color1[2],q->color1[3],upper);}
static inline Q *Begin(S *p,R *r){Align(r);uint c=(uint)r->cur,b=(uint)r->base;c|=0x30000000;b|=0x30000000;r->cur=(unsigned char *)c;r->base=(unsigned char *)b;Q *pending=0;func_002130C0(p);return pending;}
static inline void Finish(R *r,S *p){Align(r);uint c=(uint)r->cur,b=(uint)r->base;c&=0x0FFFFFFF;b&=0x0FFFFFFF;r->cur=(unsigned char *)c;r->base=(unsigned char *)b;Q *q=p->queue.first;while(q){Q *old=q;q=q->next;old->next=0;}func_0011A060(&p->queue);}
/** @sly2-annot addr=0x213300
 * Purpose: WIDGET RenderSelf: flush the sprite render queue with VU microprogram packets
 * Info: proposed name RenderWidget (T1); class WIDGET slot19; TU hud
 * Confidence: high [T1 0.80] - evidence: static analysis; sources: r7c_hud
 * See: docs/engine/hud-ui.md#5-key-functions, docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
extern "C" void func_00213300(S *p){
    func_001A5ED8(p);
    if(!func_002132D0(p))return;
    func_002185D0(p->sprites);
    float priority=16.0f;
    Q *pending=Begin(p,&D_002DEA28);
    Q *q=p->queue.first;while(q){
        if(q->kind==1){
            PARAM param(priority);
            func_00217298(p,1,priority-1.0f);
            pending=q;
            func_002159E0(q->payload,q->matrix,&param,q->channel);
            func_00217298(p,3,param.priority+1.0f);
        }else if(q->kind==4){
            float one=1.0f;
            PARAM param(priority);
            func_00217298(p,2,priority-one);
            func_002159E0(pending->payload,pending->matrix,&param,pending->channel);
            pending=0;
            func_00217298(p,4,param.priority+one);
            if(q->payload){
                PARAM second;
                SetOffset(second.offset,q->color0);
                SetScale(second.scale,q,one);second.priority=priority+=1024.0f;second.mode=1;
                func_002159E0(q->payload,q->matrix,&second,q->channel);
            }
        }else if(q->payload){
            PARAM param;
            SetOffset(param.offset,q->color0);
            SetScale(param.scale,q,1.0f);param.priority=priority;param.mode=1;
            func_002159E0(q->payload,q->matrix,&param,q->channel);
        }
    q=q->next;priority+=1024.0f;
    }
    Finish(&D_002DEA28,p);
}

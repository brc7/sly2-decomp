// Render generated sprite text, wrapped speech, or the scrolling ticker.
typedef unsigned int uint;
typedef int qword __attribute__((mode(TI)));
typedef struct {unsigned char r,g,b,a;} RGBA;
typedef struct {float a[8];} __attribute__((aligned(16))) T;
typedef union {qword q[3];struct {RGBA color;int mode;float priority;uint limit;char pad10[0x1C];float fade;};} DATA;
typedef struct {int pad[2];void *owner;char padc[8];int kind,id;int color;float scale,width,height;int pad2c,pad30;} S;
typedef struct {char *text,*cur;void *font,*basefont,*altfont;RGBA color,basecolor,altcolor;char pad20[0x20];qword q40,q50;int changed;float x,y;int hAlign,vAlign;float width,height,sx,sy,dy,fontScale;int field8C,field90,field94;} RT;
typedef struct {char pad[0x3BE8];void *font;} WORLD;
typedef struct {int id;void (*get)(char *,void *,void *);void *context;} STRINGGEN;
typedef struct {char pad10[0x10];int state;float time;int pad18;float finish;} SCROLL;
typedef struct {float t,dt;} CLOCK;
extern WORLD D_003DD490;
extern STRINGGEN D_003E1C88[];
extern const char D_002A8B80[],D_002A8B88[];
extern char D_004435F0[];
extern RGBA D_004435F4;
extern SCROLL D_003DA0C0;
extern CLOCK D_002DDED0;
extern const float D_002C40B8;
extern "C" void *PchzFindSwLangstr__FP2SWi(WORLD *,int);
extern "C" char *func_0021ED58(char *,const char *);
extern "C" char *func_0021EDA0(char *,const char *);
extern "C" RT *func_00215BA8(RT *,char *,S *,DATA *);
extern "C" int func_00215F40(RT *);
extern "C" void func_00227820(void *,RT *);
extern "C" void func_00215CA8(RT *,char *);
extern "C" int func_00215D40(RT *);
extern "C" void func_00215D00(RT *,RGBA *);
extern "C" float func_00215ED8(RT *);
extern "C" float GModPositive__Fff(float,float);
extern "C" void func_001DA5C8(SCROLL *,int);
extern "C" void func_002166B0(RT *,T *,DATA *);
static inline float Max(float a,float b){return a<b?b:a;}

extern "C" void func_00216BC8(S *p,T *t,DATA *data,int unused){
    char text[0x400];
    RT rt;
    DATA style;
    char marquee[2*sizeof(text)+3*5];
    if(!D_003DD490.font)return;
    text[0]=0;
    if(p->kind==1){
        uint i;
        for(i=0;i<108;i++)if(D_003E1C88[i].id==p->id)break;
        if(i>=108)*(long *)text=*(const long *)D_002A8B80;
        else D_003E1C88[i].get(text,D_003E1C88[i].context,p->owner);
    }else if(p->kind==0){
        func_0021ED58(text,(const char *)PchzFindSwLangstr__FP2SWi(&D_003DD490,p->id));
    }
    if(p->kind==1 && p->id==0x2EF){
        int lines=1,ch,visible;
        unsigned char *first;
        style=*data;
        func_00215BA8(&rt,text,p,&style);
        func_00215F40(&rt);
        func_00227820(D_004435F0,&rt);
        func_00215CA8(&rt,0);
        while((ch=func_00215D40(&rt))!=0)if(ch=='\n')lines++;
        visible=(int)(p->height/p->scale);
        first=(unsigned char *)rt.text;
        while(visible<lines && *first){
            while(*first && *first!='\n')first++;
            lines--;
            if(*first)first++;
        }
        func_00215CA8(&rt,(char *)first);
        style.color=D_004435F4;
        func_00215D00(&rt,&style.color);
        func_002166B0(&rt,t,data);
    }else if(p->kind==1 && p->id==0x2F1 && D_003DA0C0.state==2){
        float scale,paddingWidth,textWidth,elapsed,period,position;
        style=*data;
        func_00215BA8(&rt,text,p,&style);
        func_0021ED58(marquee,D_002A8B88);
        func_00215CA8(&rt,marquee);
        scale=t->a[0];
        paddingWidth=func_00215ED8(&rt)*scale;
        func_0021EDA0(marquee,text);
        func_00215CA8(&rt,marquee);
        textWidth=func_00215ED8(&rt)*scale;
        func_0021EDA0(marquee,D_002A8B88);
        func_0021EDA0(marquee,text);
        func_0021EDA0(marquee,D_002A8B88);
        elapsed=D_002DDED0.t-D_003DA0C0.time;
        if(elapsed<D_002C40B8){
            data->fade*=elapsed/D_002C40B8;
        }else{
            elapsed-=D_002C40B8;
            period=textWidth*0.01f;
            position=GModPositive__Fff(elapsed,period);
            if(D_003DA0C0.finish>=0.0f){
                float finish=Max(D_003DA0C0.finish,(textWidth+Max(0.0f,textWidth-p->width*scale)+paddingWidth)*0.01f);
                if(elapsed>=finish)func_001DA5C8(&D_003DA0C0,3);
            }
            t->a[4]-=position/period*textWidth;
        }
        func_002166B0(&rt,t,data);
    }else{
        style=*data;
        func_00215BA8(&rt,text,p,&style);
        func_00215F40(&rt);
        func_002166B0(&rt,t,data);
    }
}

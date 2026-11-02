// Project a world label, fit its rectangle, and render it in the supplied color.
typedef int qword __attribute__((mode(TI)));
typedef unsigned int uint;
struct RGBA {unsigned char r,g,b,a;RGBA(){}RGBA(uint bits){*(uint*)this=bits;}};
struct CM {char pad40[0x40];float sx,sy,unknown,nearDepth;char pad50[0x10];qword matrix[4];};
struct LM {float low,high;};
struct RT {const char *text;char *current;void *font,*baseFont,*alternateFont;RGBA color,baseColor,alternateColor;char pad20[0x20];qword color0,color1;int changed;float cameraX,cameraY,offsetX,offsetY,width,height,sx,sy,lineHeight,fontScale;int vAlign,hAlign,priority;};
struct CONFIG;
extern "C" void func_00258438(CONFIG *,float,float);
extern "C" void func_00258448(CONFIG *,float,float);
extern "C" void func_00258458(CONFIG *,const RGBA *);
extern "C" void func_00258470(CONFIG *,int);
extern "C" void func_00258478(CONFIG *,int);
struct CONFIG {float x,y,width,height;RGBA color;int hAlign,vAlign;CONFIG(uint bits=0xFF808080){func_00258438(this,0.0f,0.0f);func_00258448(this,640.0f,492.8000183f);RGBA gray(bits);func_00258458(this,&gray);func_00258470(this,0);func_00258478(this,0);}};
extern CM D_002DDF40;
extern float D_002C1F20;
extern LM D_002C1F28;
extern int D_002C1F30,D_002C1F34;
extern "C" qword func_0014F308(CM *,qword);
extern "C" float func_0011D948(LM *,float);
extern "C" RT *func_00215AC0(RT *,const char *,float);
extern "C" void func_00216260(RT *,float *,float *);
extern "C" void func_002163B0(RT *,CONFIG *,int);
static inline float SavedHeight(float value){register float height __asm__("$f20");__asm__("mov.s %0,%1":"=f"(height):"f"(value));return height;}
static inline float X(qword q){float f;__asm__("mtc1 %1,%0":"=f"(f):"r"(q));return f;}
static inline float Y(qword q){float f;__asm__("prot3w %0,%1":"=r"(f):"r"(q));return f;}
static inline float Z(qword q){float f;__asm__("pextuw %0,$zero,%1":"=r"(f):"r"(q));return f;}
/** @sly2-annot addr=0x1CD780
 * Purpose: project a world-space label, fit its rect and draw text in a color
 * Info: proposed name RenderWorldLabel (T2); TU render
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r5_1AE130
 * See: docs/reference/functions/rendering.md#render-0x1cd5f0-0x1cdcd0
 * @end-sly2-annot */
extern "C" void func_001CD780(qword position,const char *text,const RGBA *unusedColor,const RGBA *color,float inputHeight)
{
    float height=SavedHeight(inputHeight);
    CM *camera=&D_002DDF40;
    qword screen=func_0014F308(camera,position);
    float depth=Z(screen);
    if(depth<camera->nearDepth)return;
    float scale=func_0011D948(&D_002C1F28,height*(D_002C1F20/(camera->sx*depth)));
    RT rt;func_00215AC0(&rt,text,scale);
    CONFIG config;
    float w,h;
    func_00216260(&rt,&w,&h);
    float x=(X(screen)*0.5f+0.5f)*640.0f;
    float y=(0.5f-Y(screen)*0.5f)*492.8000183f;
    int marginX=D_002C1F30,marginY=D_002C1F34;
    config.x=x-w*0.5f-marginX;
    config.y=y-h-2*marginY;
    config.width=w+2*marginX;
    config.height=h+2*marginY;
    config.x+=marginX;config.y+=marginY;
    RGBA rgba=*color;config.color=rgba;
    func_002163B0(&rt,&config,6);
}

#define OFF(T,F) ((unsigned long)&((T*)0)->F)
typedef char projected_text_layout[(sizeof(void*)==4 && sizeof(qword)==16 && sizeof(RGBA)==4 && __alignof__(RGBA)==1 && sizeof(CM)==0xA0 && sizeof(LM)==8 && sizeof(RT)==0xA0 && sizeof(CONFIG)==0x1C && OFF(RGBA,r)==0x0 && OFF(RGBA,g)==0x1 && OFF(RGBA,b)==0x2 && OFF(RGBA,a)==0x3 && OFF(CM,sx)==0x40 && OFF(CM,nearDepth)==0x4C && OFF(CM,matrix)==0x60 && OFF(LM,low)==0x0 && OFF(LM,high)==0x4 && OFF(CONFIG,x)==0x0 && OFF(CONFIG,y)==0x4 && OFF(CONFIG,width)==0x8 && OFF(CONFIG,height)==0xC && OFF(CONFIG,color)==0x10 && OFF(CONFIG,hAlign)==0x14 && OFF(CONFIG,vAlign)==0x18 && OFF(RT,text)==0x0 && OFF(RT,current)==0x4 && OFF(RT,color)==0x14 && OFF(RT,baseColor)==0x18 && OFF(RT,alternateColor)==0x1C && OFF(RT,color0)==0x40 && OFF(RT,color1)==0x50 && OFF(RT,changed)==0x60 && OFF(RT,fontScale)==0x88 && OFF(RT,vAlign)==0x8C && OFF(RT,hAlign)==0x90 && OFF(RT,priority)==0x94)?1:-1];

typedef char scalar_argument_layout[(sizeof(float)==4 && sizeof(uint)==4)?1:-1];

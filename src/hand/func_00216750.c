// Align, clip, and queue rich-text glyphs with their current colors.
typedef unsigned int uint;
typedef unsigned long ulong;
typedef int qword __attribute__((mode(TI)));
struct RT {char pad[8];void *font;char padc[0x14];qword color0,color1;char pad40[0x20];int changed;float cameraX,cameraY,offsetX,offsetY,width,height,sx,sy,lineHeight,fontScale;int vAlign,hAlign,priority;};
union M4 {qword r[4];float a[16];};
struct DATA {uint color;int mode;float priority;uint limit;qword color0,color1;};
struct GLYPH {int a;void *data;char pad8[0xA];unsigned short mode;char pad14[8];void *address;char pad20[0x20];};
struct R {char pad[0xC];unsigned char *cur;};
struct E {int kind;float priority;uint address;int tag;};
extern struct R D_002DEA28;
extern float D_002C40A8,D_002C40AC,D_002C40B0,D_002C40B4;
void *func_002333B4(void *,int,int);
void func_00215CA8(struct RT *,char *);
int func_00215D40(struct RT *);
float func_00216B80(void *,int);
struct GLYPH *func_00216B40(void *,int);
void func_00217058(struct GLYPH *,union M4 *,struct DATA *,int);
struct E *func_001CD998(void);
static inline void Word(struct R *r,uint word){*(uint *)r->cur=word;r->cur+=4;}
static inline void Align(struct R *r,int word){while(((uint)r->cur&15)!=4*word)Word(r,0);}
static inline int Visible(union M4 *matrix,const float *left){int visible=0;if(*left<=matrix->a[8] && matrix->a[8]<=D_002C40AC && D_002C40B0<=matrix->a[9] && matrix->a[9]<=D_002C40B4)visible=1;return visible;}
/** @sly2-annot addr=0x216750
 * Purpose: align, clip and queue glyphs with colours
 * Info: proposed name CRichText_DrawGlyphs (T2); class CRichText; TU hud
 * Notes: names_v2 C proposed EdgeRect__8CFontBrxP9CTextEdgeP8CTextBoxP4GIFS (0.638)
 * Confidence: low (draft) [T2 0.45] - evidence: static naming pass; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
void func_00216750(struct RT *p,union M4 *matrix,struct DATA *style){
    float widths[32];
    unsigned char *tag=D_002DEA28.cur;
    int line=0;
    float lineHeight=p->lineHeight*p->sy*0.45454547f;
    float startX=matrix->a[8];
    style->color=0;
    func_002333B4(widths,0,sizeof(widths));
    func_00215CA8(p,0);
    {int ch;
    while((ch=func_00215D40(p))!=0){
        if(ch=='\n')line++;
        else widths[line]+=func_00216B80(p->font,ch)*p->fontScale;
    }
    }
    if(p->vAlign){
        float extra=p->height*p->sy*0.45454547f-(line+1)*lineHeight;
        if(p->vAlign==2)matrix->a[9]+=extra;
        else matrix->a[9]+=extra*0.5f;
    }
    func_00215CA8(p,0);
    {
        int flags=3,newLine=1,drawCount=0,until=2,ch;
        float *width=widths;
        while((ch=func_00215D40(p))!=0){
            if(p->changed){until=drawCount+3;flags|=2;}
            if(ch=='\n'){
                matrix->a[9]+=lineHeight;newLine=1;width++;
            }else{
                struct GLYPH *glyph;
                if(newLine){
                    if(!p->hAlign)matrix->a[8]=startX;
                    else{
                        float lineWidth=*width*p->sx*0.8f;
                        float extent=p->width*p->sx*0.8f;
                        if(p->hAlign==2)matrix->a[8]=startX+(extent-lineWidth);
                        else matrix->a[8]=startX+(extent-lineWidth)*0.5f;
                    }
                    newLine=0;
                }
                glyph=func_00216B40(p->font,ch);
                if(glyph){
                    int visible=Visible(matrix,&D_002C40A8);
                    if(glyph->data && visible){
                        if(flags&2){style->color0=p->color0;style->color1=p->color1;}
                        func_00217058(glyph,matrix,style,flags);
                        drawCount++;
                        if(drawCount>=until)flags=0;
                    }
                    matrix->a[8]+=func_00216B80(p->font,ch)*p->fontScale*p->sx*0.8f;
                }
            }
        }
    }
    Align(&D_002DEA28,0);
    *(ulong *)D_002DEA28.cur=0x60000000UL;D_002DEA28.cur+=16;
    {
        struct E *e=func_001CD998();
        e->kind=0x15;e->priority=style->priority+(p->priority<<18);e->address=(uint)tag&0x0FFFFFFF;e->tag=0;
    }
}

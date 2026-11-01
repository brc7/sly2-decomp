typedef int qword __attribute__((mode(TI)));
typedef struct { float a[6]; } __attribute__((aligned(16))) T;
typedef struct { float a[16]; } __attribute__((aligned(16))) O;
struct RT {char pad[8];void *font;char padC[0x14];qword color0,color1;char pad40[0x20];int changed;float cameraX,cameraY,offsetX,offsetY,width,height,sx,sy,lineHeight,fontScale;int vAlign,hAlign,priority;};
struct DATA {unsigned int color;int mode;float priority;unsigned int limit;qword color0,color1;};

void func_002162D8(struct RT *p, O *po, T *pt);
void func_00216750(struct RT *p, O *po, struct DATA *q);
static inline void StoreX(struct RT *p,float value){register float x asm("$f2")=value;asm("swc1 %1,%0":"=m"(p->sx):"f"(x));}
/** @sly2-annot addr=0x2166B0
 * Purpose: draw at a sprite transform (font scale, line height)
 * Info: proposed name CRichText_DrawXf (T2); class CRichText; TU hud
 * Notes: names_v2 C proposed PostTotalsLoad__FP6TOTALS (0.438)
 * Confidence: low (draft) [T2 0.40] - evidence: static naming pass; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
void func_002166B0(struct RT *p, T *pt, struct DATA *q)
{
    T t = *pt;
    O o;
    float x=t.a[0],y=t.a[3],g=p->fontScale,shift=p->lineHeight;
    p->sy=y; StoreX(p,x);
    t.a[0] = x*g;
    t.a[3] = y*g;
    t.a[5] += shift*y;
    func_002162D8(p, &o, &t);
    func_00216750(p, &o, q);
}

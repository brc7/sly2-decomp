typedef int qword __attribute__((mode(TI)));
typedef unsigned char byte;
typedef unsigned long ulong;
typedef union { qword q; float f[4]; int n[4]; } U;
struct M4 { qword r[4]; };
struct Ring { float t,g,length; int pad; struct M4 m; };
struct Sample { qword pos; float fraction; char pad14[7]; byte alpha; int pad1c; };
struct RGBA { byte c[4]; };
struct Batch { int count; char pad4[8]; U *positions; byte *uv; struct RGBA *colors; int pad18; struct Sample *samples; };
struct N { void *record; int count; struct Batch *batches; struct N *next; };
struct S { char pad[0xDC]; struct N *nodes; char padE0[0x4C]; float g; char pad130[0x20]; float fade; int pad154; int count; struct Ring *array; int index; char pad164[0x1C]; void *follow; };
extern struct CLOCK { float t,dt; } D_002DDED0;
void RenderSoSelf(struct S *);
static inline qword Broadcast(float f) { qword r; __asm__("qmtc2.ni %1,%0" : "=j"(r) : "r"(f)); return r; }
static inline qword Transform(const struct M4 *m,qword p) {
    qword r;
    __asm__("vmulax.xyz ACC,%1,%2x\n\tvmadday.xyz ACC,%3,%2y\n\tvmaddaz.xyz ACC,%4,%2z\n\tvmaddw.xyz %0,%5,$vf0w" : "=j"(r) : "j"(m->r[0]),"j"(p),"j"(m->r[1]),"j"(m->r[2]),"j"(m->r[3])); return r;
}
static inline qword Blend(qword a,qword b,qword beta,qword alpha) { register qword r __asm__("vf1"); __asm__("vmulax.xyzw ACC,%1,%2x\n\tvmaddx.xyzw %0,%3,%4x" : "=&j"(r) : "j"(a),"j"(beta),"j"(b),"j"(alpha)); return r; }
static inline float GetY(qword q) { float r; __asm__("prot3w %0,%1" : "=r"(r) : "r"(q)); return r; }
static inline float GetZ(qword q) { float r; __asm__("pextuw %0,$0,%1" : "=r"(r) : "r"(q)); return r; }
static inline void StoreXYZ(U *dst,qword v) {
    qword q;
    __asm__("qmfc2.ni %0,%1" : "=r"(q) : "j"(v));
    __asm__("sw %1,%0" : "=m"(dst->n[0]) : "r"(q));
    dst->f[1]=GetY(q);
    dst->f[2]=GetZ(q);
}
/** @sly2-annot addr=0x21C340
 * Purpose: deforms each mesh vertex by lerping between the two ring matrices bracketing its axis fraction,
 *   writes positions and per-vertex alpha, then base render 0x1F3C70
 * Info: proposed name RenderTrailSelf (T2); class CID105 slot19; TU trail
 * Notes: agent qualifier: cid105 slot19
 * Confidence: low (draft) [T2 0.50] - evidence: static analysis; sources: r8_21BB88
 * See: docs/reference/functions/particles-effects.md#trail-0x21bb88-0x21c820
 * @end-sly2-annot */
void func_0021C340(struct S *p) {
    struct N *node;
    U scalar;
    float saved;
    for(node=p->nodes;node;node=node->next) {
        int i;
        for(i=0;i<node->count;i++) {
            struct Batch *batch=&node->batches[i];
            struct Sample *s=batch->samples;
            U *out=batch->positions;
            struct RGBA *color=batch->colors;
            byte *uv=batch->uv;
            int j;
            for(j=0;j<batch->count;j++,s++) {
                float t=s->fraction*(p->count-2);
                int k=(int)t;
                float alpha=t-k;
                int ia=(p->index+p->count-k-1)%p->count;
                int ib=(ia+p->count-1)%p->count;
                struct Ring *a=&p->array[ia],*b=&p->array[ib];
                qword va=s->pos,wa=Broadcast(alpha),wb=Broadcast(1.0f-alpha),pa,pb,pos;
                scalar.q=wb;
                pa=Transform(&a->m,va);
                scalar.q=wa;
                pb=Transform(&b->m,va);
                pos=Blend(pa,pb,wb,wa);
                StoreXYZ(out,pos);
                out++;
                if(color) {
                    float fade=1.0f-(D_002DDED0.t-a->t)/p->fade;
                    if(fade<0.0f) fade=0.0f;
                    else if(1.0f<fade) fade=1.0f;
                    color->c[3]=(int)(a->g*fade*s->alpha);
                    color++;
                }
                if(uv && p->follow) {
                    ulong t=(int)(((1.0f-alpha)*a->length+alpha*b->length)*4096.0f);
                    ulong high=t>>8;
                    __asm__("sb %1,%0" : "=m"(uv[0]) : "r"(t));
                    __asm__("sb %1,%0" : "=m"(uv[1]) : "r"(high));
                    uv+=6;
                }
            }
        }
    }
    saved=p->g;
    p->g=1.0f;
    RenderSoSelf(p);
    p->g=saved;
}

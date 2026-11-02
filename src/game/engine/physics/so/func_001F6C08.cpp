// func_001F6C08 (0x1F6C08): build the scratch mesh of P once (p->mesh at +0x10).
// Allocates the MESH, per-point velocity/acceleration arrays and per-triangle vertex triples inside a
// temporary allocator FRAME, collects each triangle's three vertices from the edge list (edge face ids
// -> unique edge vertices, at most 3 per triangle), then orients every triple so that
// dot(cross(v1-v0, v2-v0), plane normal) >= 0 (swaps vertex 1/2 otherwise).
// MATCH (T1582): ONE function-scope triangle index (`itri`, used by the edge loop and as the second loop's
// counter) and ONE function-scope `ptri` shared by both loops; per-loop locals gave register rotation only.
typedef int qword __attribute__((mode(TI)));
union U { qword q; float f[4]; };
struct TRI {int vertex[3];};
struct MESH {qword *velocity,*acceleration;TRI *triangles;};
struct EDGE {unsigned short vertex[2],face[2];int extra[2];};
struct PLANE {U normal;U extra;};
struct P {char pad0[0x10];MESH *mesh;char pad14[0x68];U *positions;int planeCount;PLANE *planes;char pad88[8];int pointCount;U *source;int triCount;void *triSource;int edgeCount;EDGE *edges;};
struct FRAME;
extern "C" FRAME *STACKSCOPE_ctor(FRAME *);
extern "C" void STACKSCOPE_ctor_1AE4A0(FRAME *,int);
struct FRAME {FRAME(){}~FRAME(){}};
struct MSG {int bytes,alignment;};
extern "C" void *func_001AE1D0(MSG *);
extern "C" void *func_001AE2A0(MSG *);
extern "C" void *func_001AE378(MSG *);
static inline qword Sub(qword a,qword b) {qword r;__asm__("vsub.xyzw %0,%1,%2":"=j"(r):"0"(a),"j"(b));return r;}
static inline qword Cross(qword a,qword b){qword r;__asm__("vopmula.xyz $ACC,%1,%2\n\tvopmsub.xyz %0,%2,%1":"=j"(r):"0"(a),"j"(b));return r;}
static inline float Dot(qword a,qword b){qword r,one;float f;
__asm__("vmul.xyz %0,%2,%3\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"=j"(r),"=&j"(one):"0"(a),"j"(b));
__asm__("qmfc2.ni %0,%1":"=r"(f):"j"(r));return f;}
/** @sly2-annot addr=0x1F6C08
 * Purpose: Build a soft-mesh scratch record (velocities, accelerations, oriented triangles) for a part once
 * Info: proposed name EnsureSoPartMesh (T2); TU so
 * Notes: names_v2 v1 proposed DisplaceSo__FP2SOi (0.034)
 * Confidence: low (draft) [T2 0.45] - evidence: static naming pass; sources: r6_1D5150
 * See: docs/engine/physics-collision.md#1-so-the-solid-object-socpp-0x1f2108-0x1f8dd8-174-functions,
 *   docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
extern "C" void func_001F6C08(P *p)
{
    if(p->mesh)return;
    U first,second,cross;
    FRAME storage;
    MSG request;
    int triBytes=sizeof(TRI),alignment=16;
    STACKSCOPE_ctor(&storage);
    MSG *allocation=&request;
    request.bytes=triBytes;allocation->alignment=alignment;
    p->mesh=(MESH *)func_001AE1D0(allocation);
    request.bytes=p->pointCount*16;allocation->alignment=alignment;
    p->mesh->velocity=(qword *)func_001AE2A0(allocation);
    request.bytes=p->pointCount*16;allocation->alignment=alignment;
    p->mesh->acceleration=(qword *)func_001AE2A0(allocation);
    request.bytes=p->triCount*triBytes;allocation->alignment=alignment;
    p->mesh->triangles=(TRI *)func_001AE2A0(allocation);
    request.bytes=p->triCount*4;allocation->alignment=alignment;
    int *counts=(int *)func_001AE378(allocation);
    int itri;
    TRI *ptri;
    for(int i=0;i<p->edgeCount;i++) {
        EDGE *edge=&p->edges[i];
        for(int side=0;side<2;side++) {
            itri=edge->face[side];
            ptri=&p->mesh->triangles[itri];
            for(int endpoint=0;endpoint<2;endpoint++) {
                int vertex=edge->vertex[endpoint];
                int j;
                for(j=0;j<counts[itri];j++)if(vertex==ptri->vertex[j])break;
                if(j<counts[itri])continue;
                if(j<3){ptri->vertex[counts[itri]]=vertex;counts[itri]++;}
            }
        }
    }
    U *positions=p->positions;
    for(itri=0;itri<p->triCount;itri++) {
        ptri=&p->mesh->triangles[itri];
        PLANE *plane=&p->planes[itri];
        first.q=Sub(positions[ptri->vertex[1]].q,positions[ptri->vertex[0]].q);
        second.q=Sub(positions[ptri->vertex[2]].q,positions[ptri->vertex[0]].q);
        cross.q=Cross(first.q,second.q);
        if(Dot(cross.q,plane->normal.q)<0.0f){int t=ptri->vertex[1];ptri->vertex[1]=ptri->vertex[2];ptri->vertex[2]=t;}
    }
    STACKSCOPE_ctor_1AE4A0(&storage,2);
}

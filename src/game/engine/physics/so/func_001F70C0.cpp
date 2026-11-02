// Insert zeroed vertices, rebuild transformed geometry, and rebase its tree.
typedef int qword __attribute__((mode(TI)));
union U {qword q;float f[4];};
struct M4 {U rows[4];M4(){}M4(const M4&m){rows[0]=m.rows[0];rows[1]=m.rows[1];rows[2]=m.rows[2];rows[3]=m.rows[3];}M4&operator=(const M4&m){rows[0]=m.rows[0];rows[1]=m.rows[1];rows[2]=m.rows[2];rows[3]=m.rows[3];return *this;}};
struct LO {char pad40[0x40];M4 matrix;};
struct GEO {int count;qword *points;int planeCount;void *planes;int x10,x14;};
struct TREE {int count,flag,unused;void *nodes;};
struct P {LO *owner;int flags;float radius;int material;char pad10[0xC];int pinnedCount;char pad20[0x48];TREE tree;GEO local,world;};
struct FRAME {int pad[4];};
extern "C" FRAME *STACKSCOPE_ctor(FRAME *);
extern "C" void STACKSCOPE_ctor_1AE4A0(FRAME *,int);
struct MSG {int bytes,alignment;};
extern "C" void *func_001AE378(MSG *);
extern "C" void func_001AE5A0(void *,const void *,unsigned int);
extern "C" M4 *InvertMatrix4Rigid(M4 *,const M4 *);
extern "C" M4 *__ml__FRC7MATRIX4T0(M4 *,const M4 *,const M4 *);
extern "C" void CloneGeom__FP4GEOMP7MATRIX4T0(GEO *,const M4 *,GEO *);
extern "C" void func_001191E8(TREE *,GEO *,TREE *,GEO *);

static inline float SquaredLength(qword a){qword one;float f;__asm__("vmul.xyz %0,%0,%0\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"+j"(a),"=&j"(one));__asm__("mtc1 %1,%0":"=f"(f):"r"(a));return f;}
/* volatile: the game's sqrt.s is a barrier for the scheduler (the radius store must precede the next constant). */
static inline float Sqrt(float f){float r;__asm__ volatile("sqrt.s %0,%1":"=f"(r):"f"(f));return r;}
/** @sly2-annot addr=0x1F70C0
 * Purpose: Copy a part's geometry into another space inserting zeroed vertices, rebuild world geom and BSP
 * Info: proposed name CloneSoPartGeomInserting (T2); TU so
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
extern "C" void func_001F70C0(P *source,LO *space,int inserted,P *dest)
{
    M4 transform;
    GEO temporary;
    FRAME frame;
    STACKSCOPE_ctor(&frame); /* explicit begin/end calls (POD frame), not a ctor/dtor class: that reorders the argument copies. */
    MSG request;MSG *allocation=&request;
    float maximum=0.0f;
    dest->owner=space;dest->flags=source->flags;dest->material=source->material;dest->pinnedCount=source->pinnedCount;
    {for(int i=0;i<source->local.count;i++){float length=SquaredLength(source->local.points[i]);if(maximum<length)maximum=length;}}
    dest->radius=Sqrt(maximum);
    temporary=source->local;
    request.bytes=(temporary.count+inserted)*16;allocation->alignment=16;
    qword *points=(qword *)func_001AE378(allocation);
    int pinned=source->pinnedCount;
    func_001AE5A0(points,temporary.points,pinned*16);
    func_001AE5A0(points+(pinned+inserted),temporary.points+pinned,(temporary.count-pinned)*16);
    temporary.count+=inserted;temporary.points=points;
    {M4 product,inverse;__ml__FRC7MATRIX4T0(&product,(InvertMatrix4Rigid(&inverse,&space->matrix),&inverse),&source->owner->matrix);transform=product;}
    CloneGeom__FP4GEOMP7MATRIX4T0(&temporary,&transform,&dest->local);
    CloneGeom__FP4GEOMP7MATRIX4T0(&dest->local,&space->matrix,&dest->world);
    func_001191E8(&source->tree,&source->world,&dest->tree,&dest->world);
    STACKSCOPE_ctor_1AE4A0(&frame,2);
}

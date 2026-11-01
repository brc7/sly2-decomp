// Transform points and inverse-transpose plane normals.
typedef int qword __attribute__((mode(TI)));
struct M3{
 qword r[3];M3(){}
 M3(const M3&other){r[0]=other.r[0];r[1]=other.r[1];r[2]=other.r[2];}
 M3&operator=(const M3&other){r[0]=other.r[0];r[1]=other.r[1];r[2]=other.r[2];return *this;}
 M3(M3&source,qword a,qword b,qword c){
  source.r[0]=a;r[0]=source.r[0];
  source.r[1]=b;r[1]=source.r[1];
  source.r[2]=c;r[2]=source.r[2];
 }
};
struct M4{qword r[4];};
struct PLANE{qword normal;int unknown10,unknown14,unknown18,vertex;};
struct GEOMETRY{int pointCount;qword*points;int planeCount;PLANE*planes;};
extern "C" void func_00166428(const M4*,unsigned int,const qword*,qword*);
extern "C" M3 func_0011B340(const M3*);
static __inline__ qword ReadCPU(const qword*source){qword value;__asm__("lq %0,%1":"=r"(value):"m"(*source));return value;}
static __inline__ M3 Transpose(const M3&input,GEOMETRY*source,int&planeCount){
 M3 out;
 register qword row0 __asm__("$3"),row1 __asm__("$6"),row2 __asm__("$7");
 register qword first __asm__("$8"),third __asm__("$2"),zw __asm__("$5"),cx __asm__("$4");
 register int count __asm__("$9");
 __asm__("lq %3,%8\n\tlq %2,16+%8\n\tlq %4,32+%8\n\tpextlw %1,%2,%3\n\tpextuw %5,%2,%3\n\tpextlw %6,$zero,%4\n\tpextuw %0,$zero,%4\n\tlw %7,%9\n\tpcpyld %4,%6,%1\n\tpcpyud %3,%1,%6\n\tpcpyld %2,%0,%5\n\tpor %0,$zero,%4\n\tpor %1,$zero,%3"
  :"=&r"(row0),"=&r"(row1),"=&r"(row2),"=&r"(first),"=&r"(third),"=&r"(zw),"=&r"(cx),"=&r"(count)
  :"m"(input),"m"(source->planeCount));
 planeCount=count;return M3(out,row0,row1,row2);
}
static inline const qword *PrefetchAddress(const PLANE *input,const GEOMETRY *destination,qword &normal){
 register const qword *address asm("$2");
 register qword nextNormal asm("$19");
 asm("lw %1,%2\n\tlw $3,%3\n\tsll %1,%1,4\n\tlq %0,%4\n\taddu %1,%1,$3"
     :"=&r"(nextNormal),"=&r"(address)
     :"m"(input->vertex),"m"(destination->points),"m"(input->normal):"$3");
 normal=nextNormal;return address;
}
static inline PLANE *FinalPlane(PLANE *base,int count){
 register PLANE *afterAddress asm("$2");
 register PLANE *last asm("$4");
 asm("addu %0,%1,%2":"=r"(afterAddress):"r"(base),"r"(count*sizeof(PLANE)));
 asm("addiu %0,%1,-32":"=r"(last):"r"(afterAddress));
 return last;
}

/** @sly2-annot addr=0x1664A0
 * Purpose: Transforms a local GEOM into world space: points via VU0 (0x166428), plane normals by the
 *   inverse-transpose rotation, recomputing plane distances
 * Info: proposed name UpdateGeomWorld (T1); TU geom
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r3_1539A8
 * See: docs/reference/functions/physics-collision.md#geom-0x166270-0x166920,
 *   docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
extern "C" void func_001664A0(const M4*matrix,GEOMETRY*source,GEOMETRY*dest){
 register const M4 *inverseSource asm("$16")=matrix;
 if(source->points!=dest->points){
  func_00166428(matrix,source->pointCount,source->points,dest->points);
  int planeCount;
  M3 normal;normal=Transpose(func_0011B340((const M3*)inverseSource),source,planeCount);
  if(planeCount>0){
   PLANE*input=source->planes;PLANE*output=dest->planes;PLANE*last=FinalPlane(output,planeCount);
   register qword nextNormal asm("$19"),nextAnchor asm("$18");
   register qword v1 __asm__("vf1")=normal.r[0],v2 __asm__("vf2")=normal.r[1],v3 __asm__("vf3")=normal.r[2];
   register qword v4 __asm__("vf4")=dest->points[input->vertex],v5 __asm__("vf5")=input->normal;
   input++;
   for(;;){
    __asm__ __volatile__("vcallms 0x150" : : "j"(v1),"j"(v2),"j"(v3),"j"(v4),"j"(v5));
    if(output<last){
     const qword *anchor=PrefetchAddress(input,dest,nextNormal);
     asm("addiu %1,%1,32\n\tlq %0,%2":"=r"(nextAnchor),"+r"(input):"m"(*anchor));
    }
    {
     int sync;register qword result __asm__("vf5");
     __asm__ __volatile__("cfc2.i %0,$vi0":"=r"(sync),"=j"(result) : : "vf6","vf7","vf8");
     output->normal=result;
    }
    if(output==last)break;
    asm("qmtc2 %3,%0\n\taddiu %2,%2,32\n\tqmtc2 %4,%1"
        :"=j"(v4),"=j"(v5),"+r"(output):"r"(nextAnchor),"r"(nextNormal));
   }
  }
 }
}

typedef char layout_qword[(sizeof(qword)==16)?1:-1];
typedef char layout_pointer[(sizeof(void*)==4)?1:-1];
typedef char layout_M3[(sizeof(M3)==0x30)?1:-1];
typedef char layout_M3_alignment[(__alignof__(M3)==16)?1:-1];
typedef char layout_M4[(sizeof(M4)==0x40)?1:-1];
typedef char layout_PLANE[(sizeof(PLANE)==0x20)?1:-1];
typedef char layout_GEOMETRY[(sizeof(GEOMETRY)==0x10)?1:-1];
typedef char layout_GEOMETRY_pointCount[((unsigned long)&((GEOMETRY*)0)->pointCount==0x0)?1:-1];
typedef char layout_GEOMETRY_points[((unsigned long)&((GEOMETRY*)0)->points==0x4)?1:-1];
typedef char layout_GEOMETRY_planeCount[((unsigned long)&((GEOMETRY*)0)->planeCount==0x8)?1:-1];
typedef char layout_GEOMETRY_planes[((unsigned long)&((GEOMETRY*)0)->planes==0xc)?1:-1];
typedef char layout_PLANE_normal[((unsigned long)&((PLANE*)0)->normal==0x0)?1:-1];
typedef char layout_PLANE_vertex[((unsigned long)&((PLANE*)0)->vertex==0x1c)?1:-1];
typedef char layout_M3_row0[((unsigned long)&((M3*)0)->r[0]==0x0)?1:-1];
typedef char layout_M3_row1[((unsigned long)&((M3*)0)->r[1]==0x10)?1:-1];
typedef char layout_M3_row2[((unsigned long)&((M3*)0)->r[2]==0x20)?1:-1];

typedef char layout_int[sizeof(int)==4?1:-1];

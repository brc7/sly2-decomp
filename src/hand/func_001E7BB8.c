// World bounds from transformed path points and segment controls.
typedef int qword __attribute__((mode(TI)));
union U {qword q;float f[4];};
struct M4 {qword r[4];};
struct S {char pad[0xA0];struct M4 mat;int mode;char padE4[4];int n;float *times,*dist;qword *points;char padF8[0x48];qword *incoming,*outgoing;};
extern void func_00231368(struct S*);
static inline qword Point(qword old,qword r0,qword r1,qword r2,qword r3,qword p){__asm__("vmulax.xyz ACC,%1,%5x\n\tvmadday.xyz ACC,%2,%5y\n\tvmaddaz.xyz ACC,%3,%5z\n\tvmaddw.xyz %0,%4,$vf0w":"+j"(old):"j"(r0),"j"(r1),"j"(r2),"j"(r3),"j"(p));return old;}
/** @sly2-annot addr=0x1E7BB8
 * Purpose: World AABB of the transformed points and curve controls
 * Info: proposed name GetShapeBounds (T2); TU shape
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/physics-collision.md#shape-0x1e5d80-0x1ea568
 * @end-sly2-annot */
void func_001E7BB8(struct S*p,qword*lo,qword*hi){
 register qword r0 __asm__("vf2"),r1 __asm__("vf3"),r2 __asm__("vf4"),r3 __asm__("vf11");
 register qword maximum __asm__("vf5"),minimum __asm__("vf6");
 register qword *first __asm__("$5");
 func_00231368(p);
 {register struct M4*m __asm__("$2");
 __asm__("lw %0,%2\n\taddiu %1,%3,%4":"=&r"(first),"=r"(m):"m"(p->points),"r"(p),"I"((unsigned)&((struct S*)0)->mat));
 __asm__("lqc2 %0,0x30(%8)\n\tlqc2 %1,0x10(%8)\n\tlqc2 %2,0x20(%8)\n\tlqc2 %3,%7":"=j"(r3),"=j"(r1),"=j"(r2),"=j"(r0):"m"(m->r[3]),"m"(m->r[1]),"m"(m->r[2]),"m"(p->mat.r[0]),"r"(m));}
 {register qword value __asm__("vf1")=*first;register int mode __asm__("$2");
 __asm__ volatile("vmulax.xyz ACC,%2,%6x\n\tvmadday.xyz ACC,%3,%6y\n\tvmaddaz.xyz ACC,%4,%6z\n\tvmaddw.xyz %0,%5,$vf0w\n\tlw %1,%7":"=&j"(maximum),"=r"(mode):"j"(r0),"j"(r1),"j"(r2),"j"(r3),"j"(value),"m"(p->mode));
 minimum=maximum;
 if(mode==0){
  register int n __asm__("$4")=p->n;
  register int greater __asm__("$2");
  __asm__("addiu %0,$zero,%2\n\tslt %0,%0,%1":"=&r"(greater):"r"(n),"I"(1));
  if(greater){
   register qword a __asm__("vf8"),b __asm__("vf7"),c __asm__("vf3"),d __asm__("vf2");
   register qword *point __asm__("$3");
   register int remaining __asm__("$2");
   __asm__("vmove.xyzw %0,%3\n\taddiu %1,%4,%6\n\tvmove.xyzw %2,%5":"=j"(a),"=r"(point),"=j"(b):"j"(r0),"r"(first),"j"(r1),"I"(sizeof(qword)));
   remaining=n-1;
   __asm__("vmove.xyzw %0,%2\n\tvmove.xyzw %1,%3":"=j"(c),"=j"(d):"j"(r2),"j"(r3));
   do {register qword value __asm__("vf1")=*point;
    --remaining;value=Point(value,a,b,c,d,value);++point;
    __asm__("vmax.xyz %0,%0,%2\n\tvmini.xyz %1,%1,%2":"+&j"(maximum),"+j"(minimum):"j"(value));
   }while(remaining);
  }
 }else{
  register int bound __asm__("$7");
  register qword previous __asm__("vf7");
  __asm__("lw $2,%1\n\taddiu %0,$2,%2":"=r"(bound):"m"(p->n),"I"(-1):"$2");
  previous=maximum;
  if(bound>0){
   register qword a __asm__("vf10"),b __asm__("vf9"),c __asm__("vf8");
   register qword *incoming __asm__("$6"),*outgoing __asm__("$4"),*point __asm__("$5");
   register float *time __asm__("$3");
   register float positive __asm__("$f3"),negative __asm__("$f2");
   register int remaining __asm__("$2");
   __asm__("lw $2,%4\n\tvmove.xyzw %0,%6\n\tlw %1,%5\n\tvmove.xyzw %2,%7\n\taddiu %3,$2,%8":"=j"(a),"=r"(time),"=j"(b),"=r"(incoming):"m"(p->incoming),"m"(p->times),"j"(r0),"j"(r1),"I"(sizeof(qword)):"$2");
   positive=0.333333343f;
   __asm__("vmove.xyzw %0,%2\n\tli.s %1,%3":"=j"(c),"=f"(negative):"j"(r2),"F"(-0.333333343f):"$1");
   {register float lower __asm__("$f0");
   point=first;
   __asm__("lw %0,%4\n\taddiu %1,%1,%7\n\tdaddu %2,%5,$zero\n\tlwc1 %3,%6":"=&r"(outgoing),"+&r"(point),"=&r"(remaining),"=f"(lower):"m"(p->outgoing),"r"(bound),"m"(time[0]),"I"(sizeof(qword)));
   goto curve_body;
   do {
    lower=time[0];
curve_body: {union U scalar;
    register qword old __asm__("vf4"),value __asm__("vf1"),plus __asm__("vf3"),minus __asm__("vf2");
    register float interval __asm__("$f1");
    register int bits __asm__("$7");
    __asm__("vmove.xyzw %0,%3\n\tlwc1 %1,%4\n\taddiu %2,%2,%5":"=j"(old),"=f"(interval),"+r"(remaining):"j"(previous),"m"(time[1]),"I"(-1));
    value=*point;
    __asm__("addiu %0,%0,%3\n\tsub.s %1,%1,%2":"+r"(time),"+f"(interval):"f"(lower),"I"(sizeof(float)));
    __asm__("vmulax.xyz ACC,%1,%5x\n\tvmadday.xyz ACC,%2,%5y\n\tvmaddaz.xyz ACC,%3,%5z\n\tvmaddw.xyz %0,%4,$vf0w":"+j"(previous):"j"(a),"j"(b),"j"(c),"j"(r3),"j"(value));
    __asm__("addiu %0,%0,%5\n\tmul.s %1,%2,%3\n\tmul.s %2,%2,%4":"+r"(point),"=&f"(lower),"+f"(interval):"f"(positive),"f"(negative),"I"(sizeof(qword)));
    __asm__("mfc1 %0,%1":"=r"(bits):"f"(lower));
    __asm__("qmtc2.ni %2,%0\n\tsqc2 %0,%1":"=j"(plus),"=m"(scalar.q):"r"(bits));
    __asm__("mfc1 %0,%1":"=r"(bits):"f"(interval));
    __asm__("qmtc2.ni %3,%0\n\tlqc2 %1,%4\n\tsqc2 %0,%2":"=j"(minus),"=j"(value),"=m"(scalar.q):"r"(bits),"m"(*outgoing));
    __asm__("vmulax.xyz ACC,%1,%0x\n\tvmadday.xyz ACC,%2,%0y\n\tvmaddz.xyz %0,%3,%0z":"+j"(value):"j"(a),"j"(b),"j"(c));
    __asm__("vadda.xyzw ACC,%0,$vf0\n\tvmaddx.xyzw %0,%2,%3x\n\taddiu %1,%1,%4":"+j"(old),"+r"(outgoing):"j"(value),"j"(plus),"I"(sizeof(qword)));
    __asm__("vmax.xyz %0,%0,%3\n\tvmini.xyz %1,%1,%3\n\tlqc2 %2,%4":"+&j"(maximum),"+&j"(minimum),"=j"(value):"j"(old),"m"(*incoming));
    __asm__("vmulax.xyz ACC,%1,%0x\n\tvmadday.xyz ACC,%2,%0y\n\tvmaddz.xyz %0,%3,%0z":"+j"(value):"j"(a),"j"(b),"j"(c));++incoming;
    __asm__("vadda.xyzw ACC,%1,$vf0\n\tvmaddx.xyzw %0,%2,%0x":"+j"(minus):"j"(previous),"j"(value));
    __asm__("vmax.xyz %0,%0,%2\n\tvmini.xyz %1,%1,%2\n\tvmini.xyz %1,%1,%3\n\tvmax.xyz %0,%0,%3":"+&j"(maximum),"+&j"(minimum):"j"(minus),"j"(previous));
   }
   }while(remaining);
   }
  }
 }
 }
 *lo=minimum;*hi=maximum;
}

#define OFF(T,f) ((unsigned)&((T*)0)->f)
typedef char bound_layout_0[(sizeof(qword)==16)?1:-1];
typedef char bound_layout_1[(__alignof__(qword)==16)?1:-1];
typedef char bound_layout_2[(sizeof(union U)==16)?1:-1];
typedef char bound_layout_3[(__alignof__(union U)==16)?1:-1];
typedef char bound_layout_4[(sizeof(struct M4)==64)?1:-1];
typedef char bound_layout_5[(__alignof__(struct M4)==16)?1:-1];
typedef char bound_layout_6[(sizeof(((struct M4*)0)->r)==64)?1:-1];
typedef char bound_layout_7[(sizeof(((struct M4*)0)->r[0])==16)?1:-1];
typedef char bound_layout_8[(OFF(struct M4,r)==0)?1:-1];
typedef char bound_layout_9[(OFF(struct S,mat)==0xA0)?1:-1];
typedef char bound_layout_10[(OFF(struct S,mode)==0xE0)?1:-1];
typedef char bound_layout_11[(OFF(struct S,n)==0xE8)?1:-1];
typedef char bound_layout_12[(OFF(struct S,times)==0xEC)?1:-1];
typedef char bound_layout_13[(OFF(struct S,dist)==0xF0)?1:-1];
typedef char bound_layout_14[(OFF(struct S,points)==0xF4)?1:-1];
typedef char bound_layout_15[(OFF(struct S,incoming)==0x140)?1:-1];
typedef char bound_layout_16[(OFF(struct S,outgoing)==0x144)?1:-1];
typedef char bound_layout_17[(sizeof(void*)==4)?1:-1];
typedef char bound_layout_18[(sizeof(int)==4)?1:-1];
typedef char bound_layout_19[(sizeof(float)==4)?1:-1];
typedef char bound_layout_20[(sizeof(((struct S*)0)->mode)==4)?1:-1];
typedef char bound_layout_21[(sizeof(((struct S*)0)->n)==4)?1:-1];
typedef char bound_layout_22[(sizeof(*((struct S*)0)->points)==16)?1:-1];
typedef char bound_layout_23[(sizeof(*((struct S*)0)->times)==4)?1:-1];

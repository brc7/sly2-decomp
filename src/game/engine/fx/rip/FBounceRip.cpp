typedef int qw __attribute__((mode(TI)));
union U {qw q;float f[4];int i[4];};
struct POLY {float a,b,c;};
struct RIP {int kind;char p4[0x10];float start,duration;char p1C[4];POLY scale;char p2C[0x44];U position;char p80[0x10];U velocity;U acceleration;char pB0[0x20];U angular;char pE0[0x10];float radius,restitution,friction;};
struct SHAPE {void*frame;void*owner;};
struct CLOCK {float time,dt;};
struct PROX {float fraction,distance;};
struct WORK {U temporary,scalar,tangent,afterTangent;PROX proximity;qw contact;U firstTorque,frictionTorque,frictionAxis,inertia,normal,offset;};
extern "C" {
extern CLOCK D_002DDED0;
extern float D_002C1F60,D_002C1F64,D_002C1F68,D_002C1F6C;
extern qw D_002A2DA0,D_002A2DD0;
int FIsBasicDerivedFrom__FP5BASIC3CID(void*,int);
void CalculateWaterCurrent(void*,qw,qw*,qw*);
qw GetXfVelocity(void*,void*,qw);
void func_001CEAA8(RIP*,float);
void FindClosestPointOnLineSegment(qw,qw,qw,qw*,float*,float*);
int FBounceRip(RIP*,SHAPE*,qw,qw);
}
static inline float Max(float a,float b){return a<b?b:a;}
static inline float Evaluate(POLY*p,float t){return p->a+t*(p->b+t*p->c);}
union SPECIAL_WORD {float value;int bits;};
struct SPECIAL_PAGE {char before[0x1F60];SPECIAL_WORD duration,blend;};
static inline void*OwnerAndNormal(SHAPE*shape,WORK&w,qw normal){register void*owner __asm__("$4");__asm__("lw %0,%2\n\tsqc2 %3,%1":"=&r"(owner),"=m"(w.normal.q):"m"(shape->owner),"j"(normal));return owner;}
static inline void*SampleOwner(SHAPE*shape,WORK&w,qw*&output,qw&point){register void*owner __asm__("$4");register qw*target __asm__("$6");register qw sample __asm__("$5");__asm__("lw %0,%3\n\tdaddu %1,%4,$zero\n\tlq %2,%5":"=&r"(owner),"=&r"(target),"=r"(sample):"m"(shape->owner),"r"(&w.temporary.q),"m"(w.contact));output=target;point=sample;return owner;}
static inline float NormalLength(qw value){register float length __asm__("$f0");__asm__("qmfc2.ni $3,%1\n\tmtc1 $3,%0":"=f"(length):"j"(value):"$3");return length;}
static inline qw DistanceValue(float value){register float bits __asm__("$3")=value;register qw scalar __asm__("vf1");__asm__("qmtc2.ni %1,%0":"=j"(scalar):"r"(bits));return scalar;}
static inline qw ImpulseValue(float value){register float bits __asm__("$2")=value;register qw scalar __asm__("vf3");__asm__("qmtc2.ni %1,%0":"=j"(scalar):"r"(bits));return scalar;}
static inline unsigned FallbackPage(){register unsigned page __asm__("$2");__asm__("lui %0,%%hi(D_002A2DA0)":"=r"(page));return page;}
static inline qw*FallbackAddress(unsigned page){register qw*address __asm__("$2");__asm__("addiu %0,%1,%%lo(D_002A2DA0)":"=r"(address):"r"(page));return address;}
static inline float SecondProjection(qw value){register float bits __asm__("$3");__asm__("qmfc2.ni %0,%1":"=r"(bits):"j"(value));return bits;}
static inline qw ImpulseScalar(U&home,float value){register float bits __asm__("$2")=value;register qw scalar __asm__("vf3");__asm__("qmtc2.ni %1,%0":"=j"(scalar):"r"(bits));home.q=scalar;return home.q;}
static inline qw RelativeVelocity(qw point,qw other){register qw v __asm__("vf9");__asm__("vsub.xyzw %0,%1,%2":"=j"(v):"j"(point),"j"(other));return v;}
static inline qw PrepareOffsetTorque(qw normal,float amount,WORK&w){register qw torque __asm__("vf1");
 __asm__("mfc1 $3,%3\n\tqmtc2.ni $3,%0\n\tvmulx.xyzw $vf10,%4,%0x\n\tsqc2 %0,%1\n\tvopmula.xyz $ACC,$vf10,%4\n\tvopmsub.xyz %0,%4,$vf10\n\tsqc2 $vf10,%2":"=&j"(torque),"=m"(w.temporary.q),"=m"(w.offset.q):"f"(amount),"j"(normal):"vf10","$3");return torque;
}
static inline SHAPE*CaptureShape(SHAPE*incoming){register SHAPE*saved __asm__("$17");__asm__("daddu %0,%1,$zero":"=r"(saved):"r"(incoming));return saved;}
/** @sly2-annot addr=0x1CEB08
 * Purpose: RIP default FBounce (vt entry 8): reflect velocity off an SO contact, friction/elasticity,
 *   ForceRipFade
 * Info: proposed name FBounceRip (T1); TU rip
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r5_1AE130+verdicts_A2
 * See: docs/engine/particles-effects.md#6-key-functions,
 *   docs/guides/port-plan.md#11-first-20-engineering-tasks-ranked,
 *   docs/reference/functions/particles-effects.md#rip-0x1cdcd0-0x1d1370
 * @end-sly2-annot */
int FBounceRip(RIP*p,SHAPE*incomingShape,qw point,qw normal){
 SHAPE*shape=CaptureShape(incomingShape);
 WORK w;float radius,elastic,friction;void*owner;
 {register qw incomingNormal __asm__("vf8");__asm__("qmtc2.ni %1,%0":"=j"(incomingNormal):"r"(normal));
 if(p->kind==8){w.contact=point;radius=p->radius;elastic=p->restitution;friction=p->friction;}else{w.contact=point;radius=100.0f;elastic=0.7f;friction=0.8f;}
 radius*=Evaluate(&p->scale,(D_002DDED0.time-p->start)/p->duration);
 owner=OwnerAndNormal(shape,w,incomingNormal);}
 int special=FIsBasicDerivedFrom__FP5BASIC3CID(owner,0x59);register qw sampledNormal __asm__("vf8")=w.normal.q;
 if(special){
  qw*output;qw samplePoint;void*sampleOwner=SampleOwner(shape,w,output,samplePoint);
  CalculateWaterCurrent(sampleOwner,samplePoint,output,0);
  register unsigned blendPage __asm__("$2");register qw original __asm__("vf3");
  __asm__("lui %0,0x2C\n\tlqc2 %1,%2":"=r"(blendPage),"=j"(original):"m"(p->velocity.q));
  register int bits __asm__("$2")=((SPECIAL_PAGE*)blendPage)->blend.bits;
  register RIP*fadeArg __asm__("$4");register qw scale __asm__("vf4"),sample __asm__("vf1");register unsigned durationPage __asm__("$2");
  __asm__("daddu %0,%4,$zero\n\tqmtc2.ni %5,%1\n\tlqc2 %2,%6\n\tlui %3,%%hi(D_002C1F60)":"=&r"(fadeArg),"=j"(scale),"=j"(sample),"=r"(durationPage):"r"(p),"r"(bits),"m"(w.temporary.q));
  register qw difference __asm__("vf2");register float duration __asm__("$f12");
  __asm__("vsub.xyzw %0,%2,%3\n\tlwc1 %1,%%lo(D_002C1F60)(%4)":"=j"(difference),"=f"(duration):"j"(sample),"j"(original),"r"(durationPage),"m"(((SPECIAL_PAGE*)durationPage)->duration.value));
  __asm__("vadda.xyzw $ACC,%0,$vf0\n\tvmaddx.xyzw %0,%3,%4x\n\tvsub.xyzw %1,%1,%0\n\tsqc2 %4,%2":"+j"(original),"+j"(sample),"=m"(w.scalar.q):"j"(difference),"j"(scale));
  __asm__("mfc1 $2,%2\n\tqmtc2.ni $2,%0\n\tsqc2 %3,%1":"=j"(difference),"=m"(p->velocity.q):"f"(duration),"j"(original):"$2");
  __asm__("vdiv $Q,$vf0w,%1x\n\tvwaitq\n\tvmulq.xyzw %0,%0,$Q":"+j"(sample):"j"(difference));
  p->acceleration.q=sample;w.scalar.q=difference;
  func_001CEAA8(fadeArg,duration);
  return 1;
 }
 register void*frameArg __asm__("$4");register void*nullArg __asm__("$5");register qw pointArg __asm__("$6");register float negativeRadius __asm__("$f0");
 __asm__("neg.s %0,%5\n\tlw %1,%6\n\tlq %3,%7\n\tdaddu %2,$zero,$zero\n\tsqc2 %8,%4":"=f"(negativeRadius),"=&r"(frameArg),"=r"(nullArg),"=r"(pointArg),"=m"(w.normal.q):"f"(radius),"m"(shape->frame),"m"(w.contact),"j"(sampledNormal));
 w.firstTorque.q=PrepareOffsetTorque(sampledNormal,negativeRadius,w);
 qw packedOther=GetXfVelocity(frameArg,nullArg,pointArg);
 float impulse,fricImpulse;
 register float unit __asm__("$f21");
 register qw lineStart __asm__("$5"),queryPoint __asm__("$4");
 register qw*queryPosition __asm__("$7");
 register float*queryAlong __asm__("$8"),*queryDistance __asm__("$9");
 register qw tangent __asm__("vf7");
 {
 register qw working __asm__("vf2"),other __asm__("vf5");
 register float ten __asm__("$f0")=10.0f;
 __asm__("qmtc2.ni %2,%0\n\tlqc2 %1,%3":"=j"(other),"=j"(working):"r"(packedOther),"m"(p->angular.q));
 register qw offset __asm__("vf10");
 __asm__("max.s %0,%2,%0\n\tlqc2 %1,%3":"+f"(radius),"=j"(offset):"f"(ten),"m"(w.offset.q));
 register float squared __asm__("$f3"),invInertia __asm__("$f0");
 register qw scratch __asm__("vf1");
 __asm__("mul.s %0,%3,%3\n\t.set noat\n\tlui $at,0x4020\n\tmtc1 $at,%1\n\t.set at\n\tlqc2 %2,%4":"=f"(squared),"=f"(invInertia),"=j"(scratch):"f"(radius),"m"(p->velocity.q):"$1");
 register qw normalValue __asm__("vf8");
 __asm__("vopmula.xyz $ACC,%0,%4\n\tvopmsub.xyz %0,%4,%0\n\tvadd.xyzw %1,%1,%0\n\tlqc2 %2,%5\n\tdiv.s %3,%3,%6"
  :"+j"(working),"+j"(scratch),"=j"(normalValue),"+f"(invInertia):"j"(offset),"m"(w.normal.q),"f"(squared));
 qw relative=RelativeVelocity(scratch,other);
 register float elasticityFactor __asm__("$f1");
 __asm__("vmul.xyz %0,%3,%4\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z\n\t.set noat\n\tlui $at,0xBF80\n\tmtc1 $at,%2\n\t.set at"
  :"=&j"(scratch),"+j"(working),"=f"(elasticityFactor):"j"(relative),"j"(normalValue):"$1");
 register float projectionBits __asm__("$2"),epsilon __asm__("$f3");
 __asm__("qmfc2.ni %0,%3\n\t.set noat\n\tlui $at,0x38D1\n\tori $at,$at,0xB717\n\tmtc1 $at,%1\n\t.set at\n\tsub.s %2,%2,%4"
  :"=r"(projectionBits),"=f"(epsilon),"+f"(elasticityFactor):"j"(scratch),"f"(elastic):"$1");
 register float numerator __asm__("$f2");
 __asm__("lqc2 $vf1,%2\n\tvopmula.xyz $ACC,$vf1,%3\n\tvopmsub.xyz %0,%3,$vf1\n\tmtc1 %4,%1":"+j"(working),"=f"(numerator):"m"(w.firstTorque.q),"j"(offset),"r"(projectionBits):"vf1");
 register qw normalBase __asm__("vf11"),firstInertia __asm__("vf4");
 __asm__("vmove.xyzw %1,%3\n\tmfc1 $3,%4\n\tqmtc2.ni $3,%2\n\tmul.s %0,%0,%5"
  :"+f"(numerator),"=j"(normalBase),"=j"(firstInertia):"j"(normalValue),"f"(invInertia),"f"(elasticityFactor):"$3");
 register float negativeFriction __asm__("$f1");
 __asm__("vadda.xyzw $ACC,%2,$vf0\n\tvmaddx.xyzw %0,%0,%3x\n\tneg.s %1,%4":"+j"(working),"=f"(negativeFriction):"j"(normalValue),"j"(firstInertia),"f"(friction));
 __asm__("vmul.xyz %0,%2,%3\n\tvaddw.x $vf3,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,$vf3,%0z\n\tsqc2 %4,%1":"=&j"(scratch),"=m"(w.scalar.q):"j"(working),"j"(normalValue),"j"(firstInertia):"vf3");
 register qw scalar __asm__("vf6");
 register float firstImpulse __asm__("$f22");
 __asm__("qmfc2.ni $2,%2\n\tmtc1 $2,$f0\n\tmfc1 $3,%4\n\tdiv.s %0,%3,$f0\n\tqmtc2.ni $3,%1"
  :"=f"(firstImpulse),"=j"(scalar):"j"(scratch),"f"(numerator),"f"(negativeFriction):"$2","$3","$f0");
 impulse=firstImpulse;
 qw impulseScale=ImpulseValue(impulse);
 __asm__("vadda.xyzw $ACC,%1,$vf0\n\tvmaddx.xyzw %0,%0,%2x":"+j"(working):"j"(relative),"j"(impulseScale));
 unsigned fallbackPage=FallbackPage();
 __asm__("vmul.xyz %0,%1,%2\n\tvaddw.x $vf4,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,$vf4,%0z":"=&j"(scratch):"j"(working),"j"(normalValue):"vf4");
 qw*fallbackAddress=FallbackAddress(fallbackPage);
 register qw fallback __asm__("$2");
 __asm__("vmulx.xyzw %0,%2,%0x\n\tlq %1,%3":"+j"(scratch),"=r"(fallback):"j"(normalValue),"m"(*fallbackAddress));
 __asm__("vsub.xyzw %0,%2,%3\n\tsqc2 %4,%1":"=j"(tangent),"=m"(w.scalar.q):"j"(working),"j"(scratch),"j"(impulseScale));
 __asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"=&j"(scratch),"+j"(working):"j"(tangent));
 __asm__("sqc2 %3,%1\n\tvsqrt $Q,%0x\n\tvwaitq\n\tvaddq.x %0,$vf0,$Q\n\tsqc2 %4,%2":"+j"(scratch),"=m"(w.temporary.q),"=m"(w.scalar.q):"j"(other),"j"(scalar));
 register float length __asm__("$f0")=NormalLength(scratch);
 if(length<epsilon)scratch=fallback;
 else{__asm__("vdiv $Q,$vf0w,%0x\n\tvwaitq\n\tvmulq.xyzw %0,%1,$Q":"+j"(scratch):"j"(tangent));}
 register float secondSquared __asm__("$f2"),secondInverse __asm__("$f0"),secondFactor __asm__("$f1");register qw axis __asm__("vf5");
 __asm__("mul.s %0,%4,%4\n\t.set noat\n\tlui $at,0x4020\n\tmtc1 $at,%1\n\t.set at\n\tvadda.xyzw $ACC,%5,$vf0\n\tvmaddx.xyzw %2,%6,%7x\n\t.set noat\n\tlui $at,0xBF80\n\tmtc1 $at,%3\n\t.set at":"=f"(secondSquared),"=f"(secondInverse),"=j"(axis),"=f"(secondFactor):"f"(radius),"j"(normalBase),"j"(scratch),"j"(scalar):"$1");
 __asm__("vopmula.xyz $ACC,%4,%5\n\tvopmsub.xyz %0,%5,%4\n\tqmfc2.ni %1,%6\n\tdiv.s %2,%2,%7\n\tsqc2 %0,%3"
  :"+j"(scratch),"=r"(lineStart),"+f"(secondInverse),"=m"(w.frictionTorque.q):"j"(offset),"j"(axis),"j"(tangent),"f"(secondSquared));
 __asm__("vmul.xyz %0,%2,%3\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"=&j"(scratch),"+j"(working):"j"(relative),"j"(normalValue));
 register float secondBits __asm__("$3"),secondNumerator __asm__("$f20");
 __asm__("sub.s %0,%0,%4\n\tqmfc2.ni %1,%3\n\tdaddu %2,$zero,$zero":"+f"(secondFactor),"=r"(secondBits),"=r"(queryPosition):"j"(scratch),"f"(elastic));
 __asm__("lqc2 %0,%3\n\tmtc1 %4,%2\n\tvopmula.xyz $ACC,%0,%5\n\tvopmsub.xyz %1,%5,%0"
  :"=j"(scratch),"+j"(working),"=f"(secondNumerator):"m"(w.frictionTorque.q),"r"(secondBits),"j"(offset));
 __asm__("mul.s %0,%0,%4\n\taddiu %1,%5,0x40\n\taddiu %2,%5,0x44\n\tmfc1 $2,%6\n\tqmtc2.ni $2,%3":"+f"(secondNumerator),"=&r"(queryAlong),"=r"(queryDistance),"=j"(scalar):"f"(secondFactor),"r"(&w),"f"(secondInverse):"$2");
 register unsigned queryPage __asm__("$2");
 __asm__(".set noat\n\tlui $at,0x3F80\n\tmtc1 $at,%0\n\t.set at\n\tvadda.xyzw $ACC,%3,$vf0\n\tvmaddx.xyzw %1,%1,%4x\n\tlui %2,%%hi(D_002A2DD0)":"=f"(unit),"+j"(working),"=r"(queryPage):"j"(axis),"j"(scalar):"$1");
 register qw*queryAddress __asm__("$2");
 __asm__("vmul.xyz %0,%2,%3\n\tvaddw.x $vf3,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,$vf3,%0z\n\taddiu %1,%4,%%lo(D_002A2DD0)":"=&j"(scratch),"=r"(queryAddress):"j"(working),"j"(normalValue),"r"(queryPage):"vf3");
 register float denominatorBits __asm__("$3"),secondDenominator __asm__("$f0");
 __asm__("qmfc2.ni %0,%2\n\tlq %1,%3":"=&r"(denominatorBits),"=r"(queryPoint):"j"(scratch),"m"(*queryAddress));
 __asm__("mtc1 %2,%0\n\tsqc2 %3,%1":"=f"(secondDenominator),"=m"(w.scalar.q):"r"(denominatorBits),"j"(scalar));
 __asm__("div.s %0,%0,%1\n\tsqc2 %2,0x20(%6)\n\tsqc2 %3,0x80(%6)\n\tsqc2 %4,0x90(%6)\n\tsqc2 %5,0xA0(%6)":"+f"(secondNumerator):"f"(secondDenominator),"j"(tangent),"j"(axis),"j"(scalar),"j"(normalValue),"r"(&w):"memory");
 fricImpulse=secondNumerator;
 qw fricScale=ImpulseScalar(w.scalar,fricImpulse);
 __asm__("vadda.xyzw $ACC,%1,$vf0\n\tvmaddx.xyzw %0,%0,%2x":"+j"(working):"j"(relative),"j"(fricScale));
 __asm__("vmul.xyz %0,%1,%2\n\tvaddw.x $vf4,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,$vf4,%0z":"=&j"(scratch):"j"(working),"j"(normalValue):"vf4");
 __asm__("vmulx.xyzw %0,%1,%0x":"+j"(scratch):"j"(normalValue));
 __asm__("vsub.xyzw %0,%0,%1":"+j"(working):"j"(scratch));
 w.afterTangent.q=working;
 }
 FindClosestPointOnLineSegment(queryPoint,lineStart,w.afterTangent.q,queryPosition,queryAlong,queryDistance);
 register qw tailNormal __asm__("vf8");
 {
 register float secondWeight __asm__("$f0")=w.proximity.fraction;
 register float minimum __asm__("$f2"),firstWeight __asm__("$f1");
 register qw axis __asm__("vf5"),linear __asm__("vf3"),angular __asm__("vf4");
 __asm__("lwc1 %0,%4\n\tsub.s %1,%5,%3\n\tlqc2 %2,%6\n\tmul.s %3,%3,%7":"=f"(minimum),"=f"(firstWeight),"=j"(axis),"+f"(secondWeight):"m"(D_002C1F68),"f"(unit),"m"(w.frictionAxis.q),"f"(fricImpulse));
 __asm__("lqc2 %0,%4\n\tlqc2 %1,%5\n\tmul.s %2,%2,%6\n\tlqc2 %3,%7":"=j"(tailNormal),"=j"(linear),"+f"(firstWeight),"=j"(angular):"m"(w.normal.q),"m"(p->velocity.q),"f"(impulse),"m"(p->angular.q));
 register qw second __asm__("vf2"),frictionTorque __asm__("vf7"),first __asm__("vf1");
 __asm__("mfc1 $3,%3\n\tqmtc2.ni $3,%0\n\tlqc2 %1,%4\n\tmfc1 $2,%5\n\tqmtc2.ni $2,%2":"=j"(second),"=j"(frictionTorque),"=j"(first):"f"(secondWeight),"m"(w.frictionTorque.q),"f"(firstWeight):"$3","$2");
 __asm__("vmulax.xyzw $ACC,%4,%5x\n\tvmaddx.xyzw %0,%0,%6x\n\tsqc2 %5,%2\n\tvadd.xyzw %1,%1,%0\n\tlqc2 %0,%7\n\tsqc2 %1,%3"
  :"+j"(axis),"+j"(linear),"=m"(w.scalar.q),"=m"(p->velocity.q):"j"(tailNormal),"j"(first),"j"(second),"m"(w.firstTorque.q));
 __asm__("vmulax.xyzw $ACC,%2,%0x\n\tvmaddx.xyzw %0,%3,%4x\n\tsqc2 %4,%1":"+j"(first),"=m"(w.scalar.q):"j"(axis),"j"(frictionTorque),"j"(second));
 register qw inverse __asm__("vf6");register float vertical __asm__("$f0");
 __asm__("lqc2 %0,%4\n\tlwc1 %1,%5\n\tvadda.xyzw $ACC,%2,$vf0\n\tvmaddx.xyzw %2,%6,%0x\n\tsqc2 %2,%3"
  :"=j"(inverse),"=f"(vertical),"+j"(angular),"=m"(p->angular.q):"m"(w.inertia.q),"m"(p->velocity.f[2]),"j"(first));
 w.scalar.q=inverse;
 if(vertical<minimum){
  register unsigned fadePage __asm__("$2");
  __asm__("lui %0,%%hi(D_002C1F6C)\n\tswc1 %2,%1":"=&r"(fadePage),"=m"(p->velocity.f[2]):"f"(minimum));
  register float duration __asm__("$f12");register RIP*receiver __asm__("$4");
  __asm__("lwc1 %0,%%lo(D_002C1F6C)(%2)\n\tdaddu %1,%3,$zero":"=f"(duration),"=r"(receiver):"r"(fadePage),"r"(p),"m"(*(float*)(fadePage+0x1F6C)));
  w.normal.q=tailNormal;func_001CEAA8(receiver,duration);
  __asm__("lqc2 %0,%1":"=j"(tailNormal):"m"(w.normal.q));
 }
 }
 register int returnValue __asm__("$2");
 {
 register qw position __asm__("vf2"),contact __asm__("vf3");
 __asm__("lqc2 %0,%3\n\taddiu %1,$zero,1\n\tlqc2 %2,%4"
  :"=j"(position),"=&r"(returnValue),"=j"(contact):"m"(p->position.q),"m"(w.contact));
 register qw difference __asm__("vf1");register float zero __asm__("$f1");
 __asm__("vsub.xyzw %0,%2,%3\n\tmtc1 $zero,%1":"=j"(difference),"=f"(zero):"j"(contact),"j"(position));
 __asm__("vmul.xyz %0,%0,%1\n\tvaddw.x $vf3,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,$vf3,%0z":"+j"(difference):"j"(tailNormal):"vf3");
 register float distance __asm__("$f0")=Max(SecondProjection(difference),zero)+unit;
 qw scale=DistanceValue(distance);
 __asm__("vadda.xyzw $ACC,%0,$vf0\n\tvmaddx.xyzw %0,%3,%4x\n\tsqc2 %4,%1\n\tsqc2 %0,%2":"+j"(position),"=m"(w.scalar.q),"=m"(p->position.q):"j"(tailNormal),"j"(scale));
 }
 return returnValue;
}

typedef char layout_ceb08_0[(sizeof(qw)==16)?1:-1];
typedef char layout_ceb08_1[(sizeof(U)==16)?1:-1];
typedef char layout_ceb08_2[(sizeof(void*)==4)?1:-1];
typedef char layout_ceb08_3[(sizeof(POLY)==12)?1:-1];
typedef char layout_ceb08_4[(sizeof(SHAPE)==8)?1:-1];
typedef char layout_ceb08_5[(sizeof(PROX)==8)?1:-1];
typedef char layout_ceb08_6[(sizeof(WORK)==0xC0)?1:-1];
typedef char layout_ceb08_7[((unsigned)&((RIP*)0)->kind==0x0)?1:-1];
typedef char layout_ceb08_8[((unsigned)&((RIP*)0)->start==0x14)?1:-1];
typedef char layout_ceb08_9[((unsigned)&((RIP*)0)->duration==0x18)?1:-1];
typedef char layout_ceb08_10[((unsigned)&((RIP*)0)->scale==0x20)?1:-1];
typedef char layout_ceb08_11[((unsigned)&((RIP*)0)->position==0x70)?1:-1];
typedef char layout_ceb08_12[((unsigned)&((RIP*)0)->velocity==0x90)?1:-1];
typedef char layout_ceb08_13[((unsigned)&((RIP*)0)->acceleration==0xA0)?1:-1];
typedef char layout_ceb08_14[((unsigned)&((RIP*)0)->angular==0xD0)?1:-1];
typedef char layout_ceb08_15[((unsigned)&((RIP*)0)->radius==0xF0)?1:-1];
typedef char layout_ceb08_16[((unsigned)&((RIP*)0)->restitution==0xF4)?1:-1];
typedef char layout_ceb08_17[((unsigned)&((RIP*)0)->friction==0xF8)?1:-1];
typedef char layout_ceb08_18[((unsigned)&((SHAPE*)0)->frame==0x0)?1:-1];
typedef char layout_ceb08_19[((unsigned)&((SHAPE*)0)->owner==0x4)?1:-1];
typedef char layout_ceb08_20[((unsigned)&((CLOCK*)0)->time==0x0)?1:-1];
typedef char layout_ceb08_21[((unsigned)&((CLOCK*)0)->dt==0x4)?1:-1];
typedef char layout_ceb08_22[((unsigned)&((PROX*)0)->fraction==0x0)?1:-1];
typedef char layout_ceb08_23[((unsigned)&((PROX*)0)->distance==0x4)?1:-1];
typedef char layout_ceb08_24[((unsigned)&((WORK*)0)->temporary==0x0)?1:-1];
typedef char layout_ceb08_25[((unsigned)&((WORK*)0)->scalar==0x10)?1:-1];
typedef char layout_ceb08_26[((unsigned)&((WORK*)0)->tangent==0x20)?1:-1];
typedef char layout_ceb08_27[((unsigned)&((WORK*)0)->afterTangent==0x30)?1:-1];
typedef char layout_ceb08_28[((unsigned)&((WORK*)0)->proximity==0x40)?1:-1];
typedef char layout_ceb08_29[((unsigned)&((WORK*)0)->contact==0x50)?1:-1];
typedef char layout_ceb08_30[((unsigned)&((WORK*)0)->firstTorque==0x60)?1:-1];
typedef char layout_ceb08_31[((unsigned)&((WORK*)0)->frictionTorque==0x70)?1:-1];
typedef char layout_ceb08_32[((unsigned)&((WORK*)0)->frictionAxis==0x80)?1:-1];
typedef char layout_ceb08_33[((unsigned)&((WORK*)0)->inertia==0x90)?1:-1];
typedef char layout_ceb08_34[((unsigned)&((WORK*)0)->normal==0xA0)?1:-1];
typedef char layout_ceb08_35[((unsigned)&((WORK*)0)->offset==0xB0)?1:-1];
typedef char layout_ceb08_36[(sizeof(float)==4)?1:-1];
typedef char layout_ceb08_37[(sizeof(int)==4)?1:-1];
typedef char layout_ceb08_38[(sizeof(unsigned)==4)?1:-1];
typedef char layout_ceb08_39[(sizeof(SPECIAL_WORD)==4)?1:-1];
typedef char layout_ceb08_40[(__alignof__(qw)==16)?1:-1];
typedef char layout_ceb08_41[(__alignof__(U)==16)?1:-1];
typedef char layout_ceb08_42[(__alignof__(WORK)==16)?1:-1];
typedef char layout_ceb08_43[(__alignof__(RIP)==16)?1:-1];
typedef char layout_ceb08_44[((unsigned)&((U*)0)->q==0)?1:-1];
typedef char layout_ceb08_45[((unsigned)&((U*)0)->f==0)?1:-1];
typedef char layout_ceb08_46[((unsigned)&((U*)0)->i==0)?1:-1];
typedef char layout_ceb08_47[((unsigned)&((U*)0)->f[2]==8)?1:-1];
typedef char layout_ceb08_48[((unsigned)&((U*)0)->f[3]==12)?1:-1];
typedef char layout_ceb08_49[((unsigned)&((POLY*)0)->a==0)?1:-1];
typedef char layout_ceb08_50[((unsigned)&((POLY*)0)->b==4)?1:-1];
typedef char layout_ceb08_51[((unsigned)&((POLY*)0)->c==8)?1:-1];
typedef char layout_ceb08_52[((unsigned)&((SPECIAL_WORD*)0)->value==0)?1:-1];
typedef char layout_ceb08_53[((unsigned)&((SPECIAL_WORD*)0)->bits==0)?1:-1];
typedef char layout_ceb08_54[((unsigned)&((SPECIAL_PAGE*)0)->duration==0x1F60)?1:-1];
typedef char layout_ceb08_55[((unsigned)&((SPECIAL_PAGE*)0)->blend==0x1F64)?1:-1];
typedef char layout_ceb08_56[((unsigned)&((WORK*)0)->proximity.fraction==0x40)?1:-1];
typedef char layout_ceb08_57[((unsigned)&((WORK*)0)->proximity.distance==0x44)?1:-1];
typedef char layout_ceb08_58[((unsigned)&((RIP*)0)->velocity.f[2]==0x98)?1:-1];
typedef char layout_ceb08_59[(sizeof(CLOCK)==8)?1:-1];
typedef char layout_ceb08_60[(sizeof(((WORK*)0)->contact)==16)?1:-1];
typedef char layout_ceb08_61[(sizeof(((WORK*)0)->normal)==16)?1:-1];
typedef char layout_ceb08_62[(sizeof(((WORK*)0)->scalar)==16)?1:-1];
typedef char layout_ceb08_63[(sizeof(((RIP*)0)->position)==16)?1:-1];
typedef char layout_ceb08_64[(sizeof(((RIP*)0)->velocity)==16)?1:-1];
typedef char layout_ceb08_65[(sizeof(((RIP*)0)->angular)==16)?1:-1];
typedef char layout_ceb08_66[(sizeof(((RIP*)0)->acceleration)==16)?1:-1];
typedef char layout_ceb08_67[(sizeof(((SHAPE*)0)->frame)==4)?1:-1];
typedef char layout_ceb08_68[(sizeof(((SHAPE*)0)->owner)==4)?1:-1];
typedef char layout_ceb08_69[(sizeof(((PROX*)0)->fraction)==4)?1:-1];
typedef char layout_ceb08_70[(sizeof(((PROX*)0)->distance)==4)?1:-1];

// (Claude agent) fdlibm sf_atan.c. MATCH with SLY2_DIVNOP; the empty asm barriers after the divisions emulate Sony's
// libm div.s being an unschedulable 'nop; nop; div.s' unit (gcc may not move id= across it or put it in a delay slot).
// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
// SLY2_DIVNOP
typedef int __int32_t;
typedef unsigned int __uint32_t;
typedef union { float value; __uint32_t word; } ieee_float_shape_type;
#define GET_FLOAT_WORD(i,d) do { ieee_float_shape_type gf_u; gf_u.value = (d); (i) = gf_u.word; } while (0)
float func_00244EF8(float x);
#define fabsf func_00244EF8

static const float atanhi[] = {
  4.6364760399e-01, 7.8539812565e-01, 9.8279368877e-01, 1.5707962513e+00 };
static const float atanlo[] = {
  5.0121582440e-09, 3.7748947079e-08, 3.4473217170e-08, 7.5497894159e-08 };
static const float aT[] = {
  3.3333334327e-01, -2.0000000298e-01, 1.4285714924e-01, -1.1111110449e-01,
  9.0908870101e-02, -7.6918758452e-02, 6.6610731184e-02, -5.8335702866e-02,
  4.9768779427e-02, -3.6531571299e-02, 1.6285819933e-02 };
static const float one = 1.0, huge = 1.0e30;

float atanf(float x)
{
	float w,s1,s2,z;
	__int32_t ix,hx,id;

	GET_FLOAT_WORD(hx,x);
	ix = hx&0x7fffffff;
	if(ix>=0x50800000) {
	    if(ix>0x7f800000)
		return x+x;
	    if(hx>0) return  atanhi[3]+atanlo[3];
	    else     return -atanhi[3]-atanlo[3];
	} if (ix < 0x3ee00000) {
	    if (ix < 0x31000000) {
		if(huge+x>one) return x;
	    }
	    id = -1;
	} else {
	x = fabsf(x);
	if (ix < 0x3f980000) {
	    if (ix < 0x3f300000) {
		id = 0; x = ((float)2.0*x-one)/((float)2.0+x); __asm__ __volatile__("");
	    } else {
		x  = (x-one)/(x+one); __asm__ __volatile__(""); id = 1;
	    }
	} else {
	    if (ix < 0x401c0000) {
		x  = (x-(float)1.5)/(one+(float)1.5*x); __asm__ __volatile__(""); id = 2;
	    } else {
		id = 3; x  = -(float)1.0/x;
	    }
	}}
	z = x*x;
	w = z*z;
	s1 = z*(aT[0]+w*(aT[2]+w*(aT[4]+w*(aT[6]+w*(aT[8]+w*aT[10])))));
	s2 = w*(aT[1]+w*(aT[3]+w*(aT[5]+w*(aT[7]+w*aT[9]))));
	if (id<0) return x - x*(s1+s2);
	else {
	    z = atanhi[id] - ((x*(s1+s2) - atanlo[id]) - x);
	    return (hx<0)? -z:z;
	}
}

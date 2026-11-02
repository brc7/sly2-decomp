// (Claude agent) fdlibm ef_atan2.c. TOOLCHAIN-ONLY near miss: MATCHES exactly if the div.s is emitted as a noreorder
// 'nop; nop; div.s' unit (verified with an inline-asm DIV wrapper, not kept). Sony's libm compiler pads every div.s
// that way (also atanf, __ieee754_asinf, __ieee754_powf); the SDK asm modes do not.
// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
// SLY2_DIVNOP
typedef int __int32_t;
typedef unsigned int __uint32_t;
typedef union { float value; __uint32_t word; } ieee_float_shape_type;
#define GET_FLOAT_WORD(i,d) do { ieee_float_shape_type gf_u; gf_u.value = (d); (i) = gf_u.word; } while (0)
#define SET_FLOAT_WORD(d,i) do { ieee_float_shape_type sf_u; sf_u.word = (i); (d) = sf_u.value; } while (0)
#define FLT_UWORD_IS_NAN(x) ((x)>0x7f800000L)
#define FLT_UWORD_IS_ZERO(x) ((x)==0)
#define FLT_UWORD_IS_INFINITE(x) ((x)==0x7f800000L)
float atanf(float x);
float func_00244EF8(float x);
#define fabsf func_00244EF8

static const float
tiny  = 1.0e-30,
zero  = 0.0,
pi_o_4  = 7.8539818525e-01,
pi_o_2  = 1.5707963705e+00,
pi      = 3.1415925026e+00,
pi_lo   = 1.5099578832e-07;

float __ieee754_atan2f(float y, float x)
{
	float z;
	__int32_t k,m,hx,hy,ix,iy;

	GET_FLOAT_WORD(hx,x);
	ix = hx&0x7fffffff;
	GET_FLOAT_WORD(hy,y);
	iy = hy&0x7fffffff;
	if(FLT_UWORD_IS_NAN(ix)||
	   FLT_UWORD_IS_NAN(iy))
	   return x+y;
	if(hx==0x3f800000) return atanf(y);
	m = ((hy>>31)&1)|((hx>>30)&2);

	if(FLT_UWORD_IS_ZERO(iy)) {
	    switch(m) {
		case 0:
		case 1: return y;
		case 2: return  pi+tiny;
		case 3: return -pi-tiny;
	    }
	}
	if(FLT_UWORD_IS_ZERO(ix)) return (hy<0)?  -pi_o_2-tiny: pi_o_2+tiny;
	if(FLT_UWORD_IS_INFINITE(ix)) {
	    if(FLT_UWORD_IS_INFINITE(iy)) {
		switch(m) {
		    case 0: return  pi_o_4+tiny;
		    case 1: return -pi_o_4-tiny;
		    case 2: return  (float)3.0*pi_o_4+tiny;
		    case 3: return (float)-3.0*pi_o_4-tiny;
		}
	    } else {
		switch(m) {
		    case 0: return  zero  ;
		    case 1: return -zero  ;
		    case 2: return  pi+tiny  ;
		    case 3: return -pi-tiny  ;
		}
	    }
	}
	if(FLT_UWORD_IS_INFINITE(iy)) return (hy<0)? -pi_o_2-tiny: pi_o_2+tiny;
	k = (iy-ix)>>23;
	if(k > 60) z=pi_o_2+(float)0.5*pi_lo;
	else if(hx<0&&k<-60) z=0.0;
	else z=atanf(fabsf(y/x));
	switch (m) {
	    case 0: return       z  ;
	    case 1: {
	    	      __uint32_t zh;
		      GET_FLOAT_WORD(zh,z);
		      SET_FLOAT_WORD(z,zh ^ 0x80000000);
		    }
		    return       z  ;
	    case 2: return  pi-(z-pi_lo);
	    default:
	    	    return  (z-pi_lo)-pi;
	}
}

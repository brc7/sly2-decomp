// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0 -fno-strength-reduce
// SLY2_ASM: gnu-reorder-db
// (Claude agent) fdlibm ef_rem_pio2f; two8 copied to a local before the tx loop fixes the hoist order.
typedef int __int32_t;
typedef unsigned int __uint32_t;
typedef union { float value; __uint32_t word; } ieee_float_shape_type;
#define GET_FLOAT_WORD(i,d) do { ieee_float_shape_type gf_u; gf_u.value = (d); (i) = gf_u.word; } while (0)
#define SET_FLOAT_WORD(d,i) do { ieee_float_shape_type sf_u; sf_u.word = (i); (d) = sf_u.value; } while (0)
#define FLT_UWORD_IS_FINITE(x) ((x)<0x7f800000L)
float func_00244EF8(float x);
#define fabsf func_00244EF8
int func_00248F38(float *x, float *y, int e0, int nx, int prec, const __int32_t *ipio2);
#define __kernel_rem_pio2f func_00248F38
extern const __int32_t two_over_pi[];
extern const __int32_t npio2_hw[];

static const float
zero =  0.0000000000e+00,
half =  5.0000000000e-01,
two8 =  2.5600000000e+02,
invpio2 =  6.3661980629e-01,
pio2_1  =  1.5707855225e+00,
pio2_1t =  1.0804334124e-05,
pio2_2  =  1.0804273188e-05,
pio2_2t =  6.0770999344e-11,
pio2_3  =  6.0770943833e-11,
pio2_3t =  6.1232342629e-17;

__int32_t __ieee754_rem_pio2f(float x, float *y)
{
	float z,w,t,r,fn;
	float tx[3];
	__int32_t i,j,n,ix,hx;
	int e0,nx;

	GET_FLOAT_WORD(hx,x);
	ix = hx&0x7fffffff;
	if(ix<=0x3f490fd8)
	    {y[0] = x; y[1] = 0; return 0;}
	if(ix<0x4016cbe4) {
	    if(hx>0) { 
		z = x - pio2_1;
		if((ix&0xfffffff0)!=0x3fc90fd0) {
		    y[0] = z - pio2_1t;
		    y[1] = (z-y[0])-pio2_1t;
		} else {
		    z -= pio2_2;
		    y[0] = z - pio2_2t;
		    y[1] = (z-y[0])-pio2_2t;
		}
		return 1;
	    } else {
		z = x + pio2_1;
		if((ix&0xfffffff0)!=0x3fc90fd0) {
		    y[0] = z + pio2_1t;
		    y[1] = (z-y[0])+pio2_1t;
		} else {
		    z += pio2_2;
		    y[0] = z + pio2_2t;
		    y[1] = (z-y[0])+pio2_2t;
		}
		return -1;
	    }
	}
	if(ix<=0x43490f80) {
	    t  = fabsf(x);
	    n  = (__int32_t) (t*invpio2+half);
	    fn = (float)n;
	    r  = t-fn*pio2_1;
	    w  = fn*pio2_1t;
	    if(n<32&&(ix&0xffffff00)!=npio2_hw[n-1]) {	
		y[0] = r-w;
	    } else {
	        __uint32_t high;
	        j  = ix>>23;
	        y[0] = r-w; 
		GET_FLOAT_WORD(high,y[0]);
	        i = j-((high>>23)&0xff);
	        if(i>8) {
		    t  = r;
		    w  = fn*pio2_2;	
		    r  = t-w;
		    w  = fn*pio2_2t-((t-r)-w);	
		    y[0] = r-w;
		    GET_FLOAT_WORD(high,y[0]);
		    i = j-((high>>23)&0xff);
		    if(i>25)  {
		    	t  = r;
		    	w  = fn*pio2_3;	
		    	r  = t-w;
		    	w  = fn*pio2_3t-((t-r)-w);	
		    	y[0] = r-w;
		    }
		}
	    }
	    y[1] = (r-y[0])-w;
	    if(hx<0) 	{y[0] = -y[0]; y[1] = -y[1]; return -n;}
	    goto ret;
	}
	if(!FLT_UWORD_IS_FINITE(ix)) {
	    y[0]=y[1]=x-x; return 0;
	}
	e0 	= (int)((ix>>23)-134);
	SET_FLOAT_WORD(z, ix - ((__int32_t)(e0<<23)));
	{ float *p; float t8 = two8;
	for(i=1, p = tx;i>=0;i--) {
		*p = (float)((__int32_t)(z));
		z     = (z-*p)*t8;
		p++;
	} }
	tx[2] = z;
	nx = 3;
	while(tx[nx-1]==zero) nx--;
	n  =  __kernel_rem_pio2f(tx,y,e0,nx,2,two_over_pi);
	if(hx<0) {y[0] = -y[0]; y[1] = -y[1]; return -n;}
ret:
	return n;
}

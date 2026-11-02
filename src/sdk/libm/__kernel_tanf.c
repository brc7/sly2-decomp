// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111-01
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
/* newlib kf_tan.c
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this software is freely
 * granted, provided that this notice is preserved.
 */
typedef int __int32_t;
typedef union { float value; unsigned int word; } ieee_float_shape_type;
#define GET_FLOAT_WORD(i,d) do { ieee_float_shape_type gf_u; gf_u.value = (d); (i) = gf_u.word; } while (0)
#define SET_FLOAT_WORD(d,i) do { ieee_float_shape_type sf_u; sf_u.word = (i); (d) = sf_u.value; } while (0)
float func_00244EF8(float);
#define fabsf func_00244EF8
static const float
one   =  1.0000000000e+00,
pio4  =  7.8539812565e-01,
pio4lo=  3.7748947079e-08,
T[] =	{
  3.3333334327e-01,
  1.3333334029e-01,
  5.3968254477e-02,
  2.1869488060e-02,
  8.8632395491e-03,
  3.5920790397e-03,
  1.4562094584e-03,
  5.8804126456e-04,
  2.4646313977e-04,
  7.8179444245e-05,
  7.1407252108e-05,
 -1.8558637748e-05,
  2.5907305826e-05,
};
float __kernel_tanf(float x, float y, int iy)
{
	float z,r,v,w,s;
	__int32_t ix,hx;
	GET_FLOAT_WORD(hx,x);
	ix = hx&0x7fffffff;
	if(ix<0x31800000)
	    {if((int)x==0) {
		if((ix|(iy+1))==0) return one/fabsf(x);
		else if (iy!=1) return -one/x; else return x;
	    }
	    }
	if(ix>=0x3f2ca140) {
	    if(hx<0) {x = -x; y = -y;}
	    z = pio4-x;
	    w = pio4lo-y;
	    x = z+w; y = 0.0;
	}
	z	=  x*x;
	w 	=  z*z;
	r = T[1]+w*(T[3]+w*(T[5]+w*(T[7]+w*(T[9]+w*T[11]))));
	v = z*(T[2]+w*(T[4]+w*(T[6]+w*(T[8]+w*(T[10]+w*T[12])))));
	s = z*x;
	r = y + z*(s*(r+v)+y);
	r += T[0]*s;
	w = x+r;
	if(ix>=0x3f2ca140) {
	    v = (float)iy;
	    return (float)(1-((hx>>30)&2))*(v-(float)2.0*(x-(w*w/(w+v)-r)));
	}
	if(iy==1) return w;
	else {
	    float a,t;
	    __int32_t i;
	    z  = w;
	    GET_FLOAT_WORD(i,z);
	    SET_FLOAT_WORD(z,i&0xfffff000);
	    v  = r-(z - x);
	    t = a  = -(float)1.0/w;
	    GET_FLOAT_WORD(i,t);
	    SET_FLOAT_WORD(t,i&0xfffff000);
	    s  = (float)1.0+t*z;
	    return t+a*(s+t*v);
	}
}

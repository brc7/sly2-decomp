// (Claude agent) 5/65: exact except the mfc1->bnel hazard nop (needs gnu_reorder.py rule, see notes/245000.md); MATCH with that rule in scratch.
// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
typedef union { float value; unsigned int word; } ieee_float_shape_type;
#define GET_FLOAT_WORD(i,d) do { ieee_float_shape_type gf_u; gf_u.value = (d); (i) = gf_u.word; } while (0)
static const float
half =  5.0000000000e-01,
S1  = -1.6666667163e-01,
S2  =  8.3333337680e-03,
S3  = -1.9841270114e-04,
S4  =  2.7557314297e-06,
S5  = -2.5050759689e-08,
S6  =  1.5896910177e-10;
float __kernel_sinf(float x, float y, int iy)
{
	float z,r,v;
	int ix;
	GET_FLOAT_WORD(ix,x);
	ix &= 0x7fffffff;
	if(ix<0x32000000)
	   {if((int)x==0) return x;}
	z	=  x*x;
	v	=  z*x;
	r	=  S2+z*(S3+z*(S4+z*(S5+z*S6)));
	if(iy==0) return x+v*(S1+z*r);
	else      return x-((z*(half*y-v*r)-y)-v*S1);
}

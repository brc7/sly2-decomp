// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0 -fexceptions
// SLY2_ASM: gnu-reorder-db
#define FIRST_PSEUDO_REGISTER 79
#define REG_SAVED_OFFSET 1
#define REG_SAVED_REG 2
typedef int word_type __attribute__ ((mode (DI)));
typedef struct frame_state
{
  void *cfa;
  void *eh_ptr;
  long cfa_offset;
  long args_size;
  long reg_or_offset[FIRST_PSEUDO_REGISTER+1];
  unsigned short cfa_reg;
  unsigned short retaddr_column;
  char saved[FIRST_PSEUDO_REGISTER+1];
} frame_state;
extern void abort (void);

static inline int
in_reg_window (int reg, frame_state *udata)
{
  return 0;
}

word_type *
get_reg_addr (unsigned reg, frame_state *udata, frame_state *sub_udata)
{
  while (udata->saved[reg] == REG_SAVED_REG)
    {
      reg = udata->reg_or_offset[reg];
      if (in_reg_window (reg, udata))
	{
          udata = sub_udata;
	  sub_udata = 0;
	}
    }
  if (udata->saved[reg] == REG_SAVED_OFFSET)
    return (word_type *)(udata->cfa + udata->reg_or_offset[reg]);
  else
    abort ();
}

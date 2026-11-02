// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
typedef int ptr_type __attribute__ ((mode (pointer)));
typedef int word_type __attribute__ ((mode (DI)));
typedef struct frame_state {
  void *cfa; void *eh_ptr; long cfa_offset; long args_size;
  long reg_or_offset[80];
  unsigned short cfa_reg; unsigned short retaddr_column;
  char saved[80];
} frame_state;
#define REG_SAVED_OFFSET 1
#define REG_SAVED_REG 2
void abort(void);
word_type *get_reg_addr(unsigned reg, frame_state *udata, frame_state *sub_udata);
frame_state *__frame_state_for(void *, frame_state *);
static inline void *get_reg(unsigned reg, frame_state *udata, frame_state *sub_udata)
{ return (void *)(ptr_type)*get_reg_addr(reg, udata, sub_udata); }
frame_state *next_stack_level(void *pc, frame_state *udata, frame_state *caller_udata)
{
  caller_udata = __frame_state_for(pc, caller_udata);
  if (!caller_udata)
    return 0;
  if (udata->saved[caller_udata->cfa_reg])
    caller_udata->cfa = get_reg(caller_udata->cfa_reg, udata, 0);
  else
    caller_udata->cfa = udata->cfa;
  caller_udata->cfa += caller_udata->cfa_offset;
  return caller_udata;
}

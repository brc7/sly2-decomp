// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0 -fexceptions
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
void *func_0023320C(void *, const void *, unsigned long);
static inline int dwarf_reg_size(int reg) { return reg < 31 ? 16 : reg < 64 ? 4 : 8; }
void copy_reg(unsigned reg, frame_state *udata, frame_state *target_udata)
{
  word_type *preg = get_reg_addr(reg, udata, 0);
  word_type *ptreg = get_reg_addr(reg, target_udata, 0);
  func_0023320C(ptreg, preg, dwarf_reg_size(reg));
}


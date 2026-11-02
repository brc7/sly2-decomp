// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
typedef unsigned int size_t;
typedef unsigned int uword;
typedef int sword;
typedef unsigned char ubyte;
#define FIRST_PSEUDO_REGISTER 79
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
struct frame_state_internal
{
  struct frame_state s;
  struct frame_state_internal *saved_state;
};
struct cie_info {
  char *augmentation;
  void *eh_ptr;
  int code_align;
  int data_align;
  unsigned ra_regno;
};
typedef struct dwarf_fde { uword length; sword CIE_delta; void *pc_begin; uword pc_range; } fde;
fde *find_fde (void *pc);
void *extract_cie_info (fde *f, struct cie_info *c);
void *func_002333B4 (void *, int, size_t);
void *execute_cfa_insn (void *p, struct frame_state_internal *state, struct cie_info *info, void **pc);
void *decode_uleb128 (unsigned char *buf, unsigned *r);
static inline void *get_cie (fde *f) { return ((void *)&f->CIE_delta) - f->CIE_delta; }
static inline fde *next_fde (fde *f) { return (fde *)(((char *)f) + f->length + sizeof (f->length)); }

struct frame_state *
__frame_state_for (void *pc_target, struct frame_state *state_in)
{
  fde *f;
  void *insn, *end, *pc;
  struct cie_info info;
  struct frame_state_internal state;

  f = find_fde (pc_target);
  if (f == 0)
    return 0;

  insn = extract_cie_info (f, &info);
  if (insn == 0)
    return 0;

  func_002333B4 (&state, 0, sizeof (state));
  state.s.retaddr_column = info.ra_regno;
  state.s.eh_ptr = info.eh_ptr;

  end = next_fde ((fde*) get_cie (f));
  while (insn < end)
    insn = execute_cfa_insn (insn, &state, &info, 0);

  insn = ((fde *)f) + 1;

  if (info.augmentation[0] == 'z')
    {
      int i;
      insn = decode_uleb128 (insn, &i);
      insn += i;
    }

  end = next_fde (f);
  pc = f->pc_begin;
  while (insn < end && pc <= pc_target)
    insn = execute_cfa_insn (insn, &state, &info, &pc);

  __builtin_memcpy (state_in, &state.s, sizeof (state.s));
  return state_in;
}

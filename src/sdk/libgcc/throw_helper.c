// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0 -fexceptions
// SLY2_ASM: gnu-reorder-db
#define FIRST_PSEUDO_REGISTER 79
#define DWARF_FRAME_REGISTERS FIRST_PSEUDO_REGISTER
#define REG_SAVED_REG 2
#define NEW_EH_RUNTIME ((void *) -2)
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
struct eh_context
{
  void *handler_label;
  void **dynamic_handler_chain;
  void *info;
  void *table_index;
};
typedef struct exception_descriptor
{
  void *runtime_id_field;
} exception_descriptor;
typedef struct __eh_info __eh_info;
extern void __terminate (void);
extern void __unwinding_cleanup (void);
extern void *find_exception_handler (void *pc, exception_descriptor *table, __eh_info *eh_info, int rethrow, int *cleanup);
extern void *old_find_exception_handler (void *pc, void *table);
extern word_type *get_reg_addr (unsigned reg, frame_state *udata, frame_state *sub_udata);
extern void copy_reg (unsigned reg, frame_state *udata, frame_state *target_udata);
extern frame_state *next_stack_level (void *pc, frame_state *udata, frame_state *caller_udata);

static inline int
in_reg_window (int reg, frame_state *udata)
{
  return 0;
}

static inline word_type
get_reg (unsigned reg, frame_state *udata, frame_state *sub_udata)
{
  return *get_reg_addr (reg, udata, sub_udata);
}

static inline void *
get_return_addr (frame_state *udata, frame_state *sub_udata)
{
  return (void *) (int) get_reg (udata->retaddr_column, udata, sub_udata);
}

void *
throw_helper (struct eh_context *eh, void *pc, frame_state *my_udata, frame_state **udata_p)
{
  frame_state *udata = *udata_p;
  frame_state sub_ustruct;
  frame_state *sub_udata = &sub_ustruct;
  void *saved_pc = pc;
  void *handler;
  void *handler_p;
  void *pc_p;
  frame_state saved_ustruct;
  int new_eh_model;
  int cleanup = 0;
  int only_cleanup = 0;
  int rethrow = 0;
  int saved_state = 0;
  __eh_info *eh_info = (__eh_info *)eh->info;

  if (eh->table_index != (void *) 0)
    rethrow = 1;

  handler = (void *) 0;
  for (;;)
    {
      frame_state *p = udata;
      udata = next_stack_level (pc, udata, sub_udata);
      sub_udata = p;

      if (! udata)
	break;

      if (udata->eh_ptr == 0)
        new_eh_model = 0;
      else
        new_eh_model = (((exception_descriptor *)(udata->eh_ptr))->
                                          runtime_id_field == NEW_EH_RUNTIME);

      if (rethrow)
        {
          rethrow = 0;
          handler = find_exception_handler (eh->table_index, udata->eh_ptr,
                                          eh_info, 1, &cleanup);
          eh->table_index = (void *)0;
        }
      else
        if (new_eh_model)
          handler = find_exception_handler (pc, udata->eh_ptr, eh_info,
                                            0, &cleanup);
        else
          handler = old_find_exception_handler (pc, udata->eh_ptr);

      if (handler)
	{
	  if (cleanup)
	    {
	      if (!saved_state)
		{
		  saved_ustruct = *udata;
		  handler_p = handler;
		  pc_p = pc;
		  saved_state = 1;
		  only_cleanup = 1;
		}
	    }
	  else
	    {
	      only_cleanup = 0;
	      break;
	    }
	}

      pc = get_return_addr (udata, sub_udata) - 1;
    }

  if (saved_state)
    {
      udata = &saved_ustruct;
      handler = handler_p;
      pc = pc_p;
      if (only_cleanup)
        __unwinding_cleanup ();
    }

  if (! handler)
    __terminate();

  eh->handler_label = handler;

  if (pc == saved_pc)
    udata = my_udata;
  else
    {
      int i;
      void *handler_pc = pc;

      pc = saved_pc;
      __builtin_memcpy (udata, my_udata, sizeof (*udata));

      while (pc != handler_pc)
	{
	  frame_state *p = udata;
	  udata = next_stack_level (pc, udata, sub_udata);
	  sub_udata = p;

	  for (i = 0; i < DWARF_FRAME_REGISTERS; ++i)
	    if (i != udata->retaddr_column && udata->saved[i])
	      {
		if (in_reg_window (i, udata)
		    && udata->saved[udata->retaddr_column] == REG_SAVED_REG
		    && udata->reg_or_offset[udata->retaddr_column] == i)
		  continue;
		copy_reg (i, udata, my_udata);
	      }

	  pc = get_return_addr (udata, sub_udata) - 1;
	}

      if (udata->saved[udata->retaddr_column] == REG_SAVED_REG)
	{
	  i = udata->reg_or_offset[udata->retaddr_column];
	  if (in_reg_window (i, udata))
	    copy_reg (i, udata, my_udata);
	}
    }
  *udata_p = udata;
  return handler;
}

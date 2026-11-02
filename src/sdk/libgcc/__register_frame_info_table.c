// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
struct object { void *pc_begin; void *pc_end; void *fde_begin; void **fde_array; unsigned int count; struct object *next; };
extern struct object *objects;
void __register_frame_info_table(void *begin, struct object *ob)
{
  ob->fde_array = begin;
  ob->next = objects;
  objects = ob;
  ob->fde_begin = begin;
  ob->pc_begin = 0;
  ob->count = 0;
  ob->pc_end = 0;
}

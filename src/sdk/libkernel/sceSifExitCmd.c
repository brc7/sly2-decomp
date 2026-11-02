// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
int DisableIntc(int);
int RemoveIntcHandler(int, int);
extern int _sif_cmd_handler_id; extern int _sif_cmd_inited;
void sceSifExitCmd(void)
{
    DisableIntc(5);
    RemoveIntcHandler(5, _sif_cmd_handler_id);
    _sif_cmd_inited = 0;
}

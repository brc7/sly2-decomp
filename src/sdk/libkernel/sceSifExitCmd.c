// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
int DisableDmac(int);   /* the SIF command handler is a DMAC handler on channel 5 (SIF0); was spelled DisableIntc */
int RemoveDmacHandler(int, int);
extern int _sif_cmd_handler_id; extern int _sif_cmd_inited;
void sceSifExitCmd(void)
{
    DisableDmac(5);
    RemoveDmacHandler(5, _sif_cmd_handler_id);
    _sif_cmd_inited = 0;
}

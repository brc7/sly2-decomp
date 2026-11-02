// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-114
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
// SLY2_SAVES: sd
typedef struct { char d[0x28]; } sceGsDispEnv;
typedef struct { sceGsDispEnv disp[2]; char giftag0[0xF0]; char giftag1[0xF0]; } sceGsDBuff;
void sceGsPutDispEnv(sceGsDispEnv *);
void sceGsPutDrawEnv(void *);
void sceGsSwapDBuff(sceGsDBuff *db, int id)
{
    id &= 1;
    sceGsPutDispEnv(&db->disp[id]);
    if (id) sceGsPutDrawEnv(db->giftag1);
    else sceGsPutDrawEnv(db->giftag0);
}

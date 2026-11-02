// (Claude agent) one reused `temp` for the three swaps; check=out1, out2 if type==1.
// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0 -fstrict-aliasing
// SLY2_ASM: gnu-reorder-db
typedef struct IMG {
    char pad0[0x18]; long pts, dts; int ready;
    int structure, type, temporal, width, parameter;
    int height, a, b, c, d, e, f, h, i;
} IMG;
typedef struct DEC {
    char p0[0xB4]; int count0, count1;
    char p1[0xFC-0xBC]; int reset;
    char p2[0x14C-0x100]; int temporal;
    char p3[0x158-0x150]; int h,i,structure;
    char p4[0x184-0x164]; int type,parameter;
    char p5[0x194-0x18C]; int height,width,a,b,c,d,e,f,delayed,available;
    char p6[0x1C8-0x1BC];
    IMG *old0,*cur0,*out0,*spare0;
    IMG *old1,*cur1,*out1,*spare1;
    IMG *old2,*cur2,*out2,*spare2;
    char p7[0x838-0x1F8]; long pts,dts;
} DEC;
int _updateRefImage(DEC *p, int secondField)
{
    int threshold=4;
    int type=p->type;
    int structure=p->structure;
    IMG *selected=0;
    int ready=0;
    if(type==3)threshold=2;
    if(structure==3){
        p->out0=p->spare0;
        p->out1=p->spare1;
        p->out2=p->spare2;
        if(p->count0+p->count1>=threshold){p->reset=0;p->available=0;p->delayed=0;}
        if((p->reset || p->available) && !p->delayed){
            p->old0->ready=0;
            p->old1->ready=0;
            p->old2->ready=0;
            type=p->type;
        }
        p->reset=0;
        p->available=0;
        if(type==3){
            if(p->old0->ready==1 || p->delayed){
                if(p->cur0->ready==1)ready=1;
            }
        }else{
            if((p->old1->ready==1 && p->old2->ready==1) || p->delayed){
                if(p->cur1->ready==1 && p->cur2->ready==1)ready=1;
            }
        }
    }else{
        if(!secondField){
            IMG *temp;
            temp=p->old0;p->old0=p->cur0;p->cur0=temp;
            temp=p->old1;p->old1=p->cur1;p->cur1=temp;
            temp=p->old2;p->old2=p->cur2;p->cur2=temp;
        }
        p->out0=p->cur0;
        p->out1=p->cur1;
        p->out2=p->cur2;
        if(type==3){
            if(structure!=2 || p->old0->ready==1)ready=1;
        }else{
            IMG *check=p->out1;
            if(type==1)check=p->out2;
            if(structure!=2)ready=1;
            else if((secondField && check->ready==1) ||
                    (p->old1->ready==1 && p->old2->ready==1))ready=1;
        }
    }
    switch(type){case 1:selected=p->out1;break;case 2:selected=p->out2;break;case 3:selected=p->out0;break;}
    selected->ready=0;
    selected->pts=p->pts;
    selected->structure=p->structure;
    selected->dts=p->dts;
    selected->type=p->type;
    selected->temporal=p->temporal;
    selected->width=p->width;
    selected->parameter=p->parameter;
    selected->height=p->height;
    selected->a=p->a;
    selected->b=p->b;
    selected->c=p->c;
    selected->d=p->d;
    selected->e=p->e;
    selected->f=p->f;
    selected->h=p->h;
    selected->i=p->i;
    return ready;
}

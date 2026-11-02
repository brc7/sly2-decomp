// Build the screen-effect VIF and GIF packets.
#define FIELD_OFFSET(t,m) ((uint)&(((t*)0)->m))
#include "vifs.h"
#include "gifs.h"
struct COMMAND {short kind,index;};
struct GAME {char pad[0x3E38];int ready;};
extern GAME D_003DD490;
extern int D_002DEB70[2];
extern ulong D_003E1330;
extern QW*D_002DEA4C;
extern QW D_002DEA50[],D_002DF940[],D_002DFA80[];
extern "C" void AddFramePatch(int,GIFS*,COMMAND**);
extern "C" void func_0016D6C8(GIFS*,uint,COMMAND**);
typedef char GIFS_Size[sizeof(GIFS)==40?1:-1];
typedef char VIFS_Size[sizeof(VIFS)==36?1:-1];
typedef char DMAS_Size[sizeof(DMAS)==28?1:-1];
typedef char QW_Size[sizeof(QW)==16?1:-1];
typedef char COMMAND_Size[sizeof(COMMAND)==4?1:-1];
typedef char DMAS_m_ab_Offset[FIELD_OFFSET(DMAS,m_ab)==4?1:-1];
typedef char DMAS_m_pb_Offset[FIELD_OFFSET(DMAS,m_pb)==12?1:-1];
typedef char GIFS_m_ab_Offset[FIELD_OFFSET(GIFS,m_ab)==4?1:-1];
typedef char GIFS_m_pb_Offset[FIELD_OFFSET(GIFS,m_pb)==12?1:-1];
typedef char GAME_ready_Offset[FIELD_OFFSET(GAME,ready)==15928?1:-1];
typedef char COMMAND_kind_Offset[FIELD_OFFSET(COMMAND,kind)==0?1:-1];
typedef char COMMAND_index_Offset[FIELD_OFFSET(COMMAND,index)==2?1:-1];
/** @sly2-annot addr=0x16DD58
 * Purpose: Pass 6/7 end: apply pass reading the framebuffer as an indexed texture (TEX2 D_003E1330) with
 *   alpha blend over full-screen strips (MSCAL 0x1C48)
 * Info: proposed name EndRpProjVolume (T1); TU glob
 * Notes: no usable identifier; purpose only
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r3_1539A8+rendering
 * See: docs/engine/rendering.md#4-the-26-kinds-and-12-passes-cstatic-tables-dumped-from-the-elf-roles-cstatic-unless-marked,
 *   docs/engine/rendering.md#7-shadow-volumes-kinds-78-p6p7-prog4-z-fail-counting-in-the-green-channel-cstatic,
 *   docs/engine/rendering.md#13-key-functions
 * @end-sly2-annot */
extern "C" void EndRpProjVolume(VIFS*input,int first,int mode){
 register VIFS*vifs __asm__("$20")=input;
 register uint programHigh __asm__("$22");
 const uint entryAddress=0x1C48;
 __asm__("lui %0,%1":"=r"(programHigh):"i"(entryAddress>>16));
 vifs->AddDmaCnt();
 uint*pun;
 vifs->UnpackHelper(UPK_V2_16,8,0x1B,0,&pun);
 COMMAND*cursor=(COMMAND*)pun;
 GIFS gifs;
 QW data[0x200];
 gifs.AllocStatic(0x200,data);
 if(mode!=8){
  if(D_002DEB70[1]){
   gifs.AddPrimPack(0,1,0xE);
   int right=0x7200,left=0x7000;
   AddFramePatch(0,&gifs,&cursor);
   gifs.PackAD(0x4C,0xFFFFFEFF00000000L);
   gifs.PackAD(0x47,0x31001L);
   gifs.PackAD(0x42,0x2000000A9L);
   gifs.PackAD(1,0x100000000L);
   gifs.PackAD(0,0x46L);
   gifs.AddPrimList(2,0x55UL);
   int remaining=0x1E0;
   do{
    gifs.ListXYZ(left,0x7900,0);
    left+=0x200;
    gifs.ListXYZ(right,0x8700,0);
    remaining-=0x20;
    right+=0x200;
   }while(remaining>=0);
  }
  func_0016D6C8(&gifs,0x810095,&cursor);
  func_0016D6C8(&gifs,0x266,&cursor);
 }
 gifs.AddPrimPack(0,1,0xE);
 AddFramePatch(0,&gifs,&cursor);
 gifs.PackAD(0x4C,0);
 gifs.PackAD(0x46,1);
 if(mode!=8){
  AddFramePatch(2,&gifs,&cursor);
  gifs.PackAD(6,0x6A8000000L);
  gifs.PackAD(0x14,1);
  gifs.PackAD(0x16,D_003E1330);
  gifs.PackAD(0x3F,0);
  gifs.PackAD(0x47,0x31001);
  gifs.PackAD(1,0x180808080L);
  gifs.PackAD(0x42,0x44);
  gifs.PackAD(0,0x156);
  gifs.AddPrimList(4,0x5353);
  for(int x=0;x<0x200;x+=0x20){
   gifs.ListUV((x<<4)|8,8);
   gifs.ListXYZ((x+0x700)<<4,0x7900,0);
   gifs.ListUV(((x+0x20)<<4)|8,0xE08);
   gifs.ListXYZ((x+0x720)<<4,0x8700,0);
  }
 }
 gifs.EndPrim(1);
 AddFramePatch(-1,&gifs,&cursor);
 int count=(uint)(gifs.m_pb-gifs.m_ab)>>4;
 if(count>=0x100){
  vifs->AddVifUnpack(UPK_V4_32,0xFF,gifs.m_ab,0x23);
  vifs->AddVifUnpack(UPK_V4_32,((uint)(gifs.m_pb-gifs.m_ab)>>4)-0xFF,gifs.m_ab+0xFF0,0x122);
 }else vifs->AddVifUnpack(UPK_V4_32,count,gifs.m_ab,0x23);
 vifs->AddVifFlusha();
 {register char*program __asm__("$5");
 __asm__("addiu %0,%1,%2":"=r"(program):"r"(programHigh),"i"(entryAddress&0xFFFF));
 vifs->AddVifMscal(program);}
 vifs->AddVifFlusha();
 GAME*game=&D_003DD490;
 if(game->ready){
  if(D_002DEA4C==D_002DEA50)vifs->AddDmaCall(D_002DF940);
  else vifs->AddDmaCall(D_002DFA80);
  vifs->AddVifBaseOffset(0x32,0x1E7);
 }
 vifs->EndDmaCnt();
 if(mode!=8){D_002DEB70[0]=0;D_002DEB70[1]=0;}
}

typedef char UINT_Size[sizeof(uint)==4?1:-1];
typedef char ULONG_Size[sizeof(ulong)==8?1:-1];
typedef char GAME_Size[sizeof(GAME)==0x3E3C?1:-1];

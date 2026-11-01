// Initialize the fixed GIF register packets.
#include "gifs.h"
extern QW D_004421B0[7];
extern QW D_00442240[0x27];
extern QW D_00442210[3];
extern QW D_004424B0[2];
extern QW D_004424D0[2];
extern QW D_00442560[4];
extern QW D_004425A0[3];
extern QW D_004424F0[3];
extern QW D_00442520[4];
extern int D_002DEDB0;
extern ulong D_002DECE0,D_002DEBF0;
/** @sly2-annot addr=0x211528
 * Purpose: startup-table entry: builds the static GIF register packets used by sprite/HUD rendering
 * Info: proposed name StartupScreen (T2); TU hud
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r7c_hud
 * See: docs/engine/main-loop-startup.md#open-questions, docs/engine/overview.md#1-boot,
 *   docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
extern "C" void func_00211528(){
 GIFS setup;
 QW *data=D_004421B0;
 QW *header=(QW *)((int)data-0x10);
 setup.AllocStatic(7,data);
 ((QW *)((int)data-0x10))->aul[0]=1UL<<60;header->aul[1]=5;
 setup.AddPrimBulk(0x44,0,1,1,0);header[1].aul[0]|=1;
 setup.AddPrimBulk(0x84,0,1,1,0);header[2].aul[0]|=1;
 setup.AddPrimBulk(0x45,0,1,1,0);header[3].aul[0]|=1;
 setup.AddPrimBulk(0x85,0,1,1,0);header[4].aul[0]|=1;
 setup.AddPrimBulk(0x42,0,1,1,0);header[5].aul[0]|=1;
 setup.AddPrimBulk(0x82,0,1,1,0);header[6].aul[0]|=1;
 GIFS tiles;
 tiles.AllocStatic(0x27,D_00442240);
 tiles.AddPrimPack(0x46,1,0xE);
 tiles.PackAD(0x4A,1);tiles.PackAD(0x42,0x4A);tiles.PackAD(0x47,0x31001);
 for(int i=0;i<0x200;i+=0x20){tiles.PackAD(5,0x79000000L|(0x7000+i*0x10));tiles.PackAD(5,0x87000000L|(0x7200+i*0x10));}
 tiles.PackAD(0x4A,0);tiles.PackAD(0x42,0x44);tiles.PackAD(0x47,0x50000);
 tiles.AddPrimEnd();
 GIFS frame;
 ulong *active=(D_002DEDB0&1)?&D_002DECE0:&D_002DEBF0;
 ulong frameWord=*active;
 frame.AllocStatic(3,D_00442210);
 frame.AddPrimPack(0,1,0xE);frame.PackAD(0x3F,0);frame.PackAD(0x4E,0x3A000000L|(frameWord&0x1FF));
 frame.AddPrimEnd();
 GIFS unmask;
 unmask.AllocStatic(2,D_004424B0);
 unmask.AddPrimPack(0,1,0xE);unmask.PackAD(0x4C,0);unmask.AddPrimEnd();
 GIFS mask;
 mask.AllocStatic(2,D_004424D0);
 mask.AddPrimPack(0,1,0xE);mask.PackAD(0x4C,0xFF00000000000000UL);mask.AddPrimEnd();
 GIFS restore;
 restore.AllocStatic(4,D_00442560);
 restore.AddPrimPack(0,1,0xE);restore.PackAD(0x4A,0);restore.PackAD(0x42,0x44);restore.PackAD(0x47,0x50000);restore.AddPrimEnd();
 GIFS region;
 region.AllocStatic(3,D_004425A0);
 region.AddPrimPack(0,1,0xE);region.PackAD(0x42,0x44);region.PackAD(0x47,0x74000);region.AddPrimEnd();
 GIFS test;
 test.AllocStatic(3,D_004424F0);
 test.AddPrimPack(0,1,0xE);test.PackAD(0x42,0x4A);test.PackAD(0x47,0x30000);test.AddPrimEnd();
 GIFS enable;
 enable.AllocStatic(4,D_00442520);
 enable.AddPrimPack(0,1,0xE);enable.PackAD(0x4A,1);enable.PackAD(0x42,0x4A);enable.PackAD(0x47,0x31001);enable.AddPrimEnd();
}

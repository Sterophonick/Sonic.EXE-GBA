#include <string.h>
#include <agb_lib.h>
#include "../inc/defs.h"
#include "../inc/gbfs.h"
#include "../inc/lz77.h"

void longstatic()
{
	REG_SOUNDCNT1_H = 0;                                                       //REG_SOUNDCNT_H = 0000 1011 0000 0100, volume = 100, sound goes to the left, sound goes to the right, timer 0 is used, FIFO buffer reset
	REG_SOUNDCNT1_X = 0;                                                       //REG_SOUNDCNT_X = 0000 0000 1000 0000, enable the sound system, DMA 1
	REG_SD1SAD = 0;                                //REG_DM1SAD = NAME, address of DMA source is the digitized music sample
	REG_SD1DAD = 0;                                                   //REG_DM1DAD = REG_SGFIFOA, address of DMA destination is FIFO buffer for direct sound A
	REG_SD1CNT_H = 0;                                                       //REG_DM1CNT_H = 1011 0110 0100 0000, DMA destination is fixed, repeat transfer of 4 bytes when FIFO , buffer is empty, enable DMA 1 (number of DMA transfers is ignored), INTERRUPT
	REG_TM0SD = 0;                          //REG_TM0D = 65536-(16777216/frequency);, play sample every 16777216/frequency CPU cycles
	REG_TMSDCNT = 0;
	playSound(7);
	frames = 0;
	for (i = 0; i < 20; i++) {
		WaitForVblank();
		setbg2novb((u16*)bg_static_1Bitmap, (u16*)bg_static_1Palette);
		WaitForVblank();
		setbg2novb((u16*)bg_static_2Bitmap, (u16*)bg_static_2Palette);
		WaitForVblank();
		setbg2novb((u16*)bg_static_3Bitmap, (u16*)bg_static_3Palette);
	}
}

void playSound(int s) {
    REG_SOUNDCNT1_H = 0x0B04;                                                       //REG_SOUNDCNT_H = 0000 1011 0000 0100, volume = 100, sound goes to the left, sound goes to the right, timer 0 is used, FIFO buffer reset
    REG_SOUNDCNT1_X = 0x0080;                                                       //REG_SOUNDCNT_X = 0000 0000 1000 0000, enable the sound system, DMA 1
    REG_SD1SAD = (unsigned long)sound[s].song;                                //REG_DM1SAD = NAME, address of DMA source is the digitized music sample
    REG_SD1DAD = 0x040000A0;                                                   //REG_DM1DAD = REG_SGFIFOA, address of DMA destination is FIFO buffer for direct sound A
    REG_SD1CNT_H = 0xB640;                                                       //REG_DM1CNT_H = 1011 0110 0100 0000, DMA destination is fixed, repeat transfer of 4 bytes when FIFO , buffer is empty, enable DMA 1 (number of DMA transfers is ignored), INTERRUPT
    REG_TM0SD = 65536 - (16777216 / sound[s].frequency);                          //REG_TM0D = 65536-(16777216/frequency);, play sample every 16777216/frequency CPU cycles
    REG_TMSDCNT = 0x00C0;
}

void loadSmallSpritePal(u16* data) {
    int 	x;

    for (x = 0; x < 16; x++) {
        OBJPaletteMem[x] = ((unsigned short*)data)[x];
    }
}

void vid_vsync() {
    while (REG_VCOUNT >= 160);   // wait till VDraw
    while (REG_VCOUNT < 160);    // wait till VBlank
}

void vblfunc() {
    CopyOAM();
    g_newframe = 0;
}

BG bg3;
void UpdateBG(BG *bg) {
    switch(bg->n) {
    case 0:
        REG_BG0HOFS = bg->x;
        REG_BG0VOFS = bg->y;
        break;
    case 1:
        REG_BG1HOFS = bg->x;
        REG_BG1VOFS = bg->y;
        break;
    case 2:
        REG_BG2HOFS = bg->x;
        REG_BG2VOFS = bg->y;
        break;
    case 3:
        REG_BG3HOFS = bg->x;
        REG_BG3VOFS = bg->y;
        break;
    default:
        break;
    }
}

int main() {
    int s;
    initsound8(1, 22050, 45072, (void*)snd_laugh);
    initsound8(2, 22050, 93200, (void*)snd_rip);
    initsound8(3, 22050, 212944, (void*)bgm_datasel);
    initsound8(4, 22050, 5365456, (void*)bgm_run);
    initsound8(5, 22050, 2835056, (void*)bgm_hill);
	initsound8(6, 22050, 36992, (void*)snd_msg);
	initsound8(7, 22050, 44128, (void*)snd_static);
	initsound8(8, 22050, 235024, (void*)bgm_gg);
	initsound8(9, 22050, 1631744, (void*)bgm_hide);
	initsound8(10, 22050, 861024, (void*)bgm_prank);
	initsound8(11, 22050, 737872, (void*)bgm_jpn);
	initsound8(12, 22050, 1894976, (void*)bgm_3dot);
	initsound8(13, 22050, 194368, (void*)bgm_title);
	initsound8(14, 22050, 302016, (void*)bgm_gattagofest);
	bgx = 0;
	bg3.x = 0;
	bg3.y = 0;
	bg3.n = 2;
    SetMode(MODE_4 | BG2_ENABLE);
	playSound(13);
    for (i = 0; i < 25; i++) {
        setbg2((u16*)title_frame1Bitmap, (u16*)title_frame1Palette);
        Sleep(7);
        setbg2((u16*)title_frame2Bitmap, (u16*)title_frame2Palette);
        Sleep(7);
    }
    SetMode(MODE_3 | BG2_ENABLE);
    memcpy(BGPaletteMem, 0x02000000, 256);
    memcpy(videoBuffer, title_demonicBitmap, 76800);
    Sleep(10);
    SetMode(MODE_4 | BG2_ENABLE);
	REG_SOUNDCNT1_H = 0;                                                       //REG_SOUNDCNT_H = 0000 1011 0000 0100, volume = 100, sound goes to the left, sound goes to the right, timer 0 is used, FIFO buffer reset
	REG_SOUNDCNT1_X = 0;                                                       //REG_SOUNDCNT_X = 0000 0000 1000 0000, enable the sound system, DMA 1
	REG_SD1SAD = 0;                                //REG_DM1SAD = NAME, address of DMA source is the digitized music sample
	REG_SD1DAD = 0;                                                   //REG_DM1DAD = REG_SGFIFOA, address of DMA destination is FIFO buffer for direct sound A
	REG_SD1CNT_H = 0;                                                       //REG_DM1CNT_H = 1011 0110 0100 0000, DMA destination is fixed, repeat transfer of 4 bytes when FIFO , buffer is empty, enable DMA 1 (number of DMA transfers is ignored), INTERRUPT
	REG_TM0SD = 0;                          //REG_TM0D = 65536-(16777216/frequency);, play sample every 16777216/frequency CPU cycles
	REG_TMSDCNT = 0;
    Sleep(120);
    setbg2((u16*)bg_datasel_tBitmap, (u16*)bg_datasel_tPalette);
    SetMode(MODE_4 | BG2_ENABLE);
	playSound(3);
    while (!(keyDown(KEY_START))) {
        WaitForVblank();
        frames++;
        if (!(frames % 540)) {
            REG_SOUNDCNT1_H = 0;                                                       //REG_SOUNDCNT_H = 0000 1011 0000 0100, volume = 100, sound goes to the left, sound goes to the right, timer 0 is used, FIFO buffer reset
            REG_SOUNDCNT1_X = 0;                                                       //REG_SOUNDCNT_X = 0000 0000 1000 0000, enable the sound system, DMA 1
            REG_SD1SAD = 0;                                //REG_DM1SAD = NAME, address of DMA source is the digitized music sample
            REG_SD1DAD = 0;                                                   //REG_DM1DAD = REG_SGFIFOA, address of DMA destination is FIFO buffer for direct sound A
            REG_SD1CNT_H = 0;                                                       //REG_DM1CNT_H = 1011 0110 0100 0000, DMA destination is fixed, repeat transfer of 4 bytes when FIFO , buffer is empty, enable DMA 1 (number of DMA transfers is ignored), INTERRUPT
            REG_TM0SD = 0;                          //REG_TM0D = 65536-(16777216/frequency);, play sample every 16777216/frequency CPU cycles
            REG_TMSDCNT = 0;
            playSound(3);
        }
    }
    REG_SOUNDCNT1_H = 0;                                                       //REG_SOUNDCNT_H = 0000 1011 0000 0100, volume = 100, sound goes to the left, sound goes to the right, timer 0 is used, FIFO buffer reset
    REG_SOUNDCNT1_X = 0;                                                       //REG_SOUNDCNT_X = 0000 0000 1000 0000, enable the sound system, DMA 1
    REG_SD1SAD = 0;                                //REG_DM1SAD = NAME, address of DMA source is the digitized music sample
    REG_SD1DAD = 0;                                                   //REG_DM1DAD = REG_SGFIFOA, address of DMA destination is FIFO buffer for direct sound A
    REG_SD1CNT_H = 0;                                                       //REG_DM1CNT_H = 1011 0110 0100 0000, DMA destination is fixed, repeat transfer of 4 bytes when FIFO , buffer is empty, enable DMA 1 (number of DMA transfers is ignored), INTERRUPT
    REG_TM0SD = 0;                          //REG_TM0D = 65536-(16777216/frequency);, play sample every 16777216/frequency CPU cycles
    REG_TMSDCNT = 0;
    srand(frames);
    Sleep(240);
    playSound(1);
    Sleep(120);
    REG_SOUNDCNT1_H = 0;                                                       //REG_SOUNDCNT_H = 0000 1011 0000 0100, volume = 100, sound goes to the left, sound goes to the right, timer 0 is used, FIFO buffer reset
    REG_SOUNDCNT1_X = 0;                                                       //REG_SOUNDCNT_X = 0000 0000 1000 0000, enable the sound system, DMA 1
    REG_SD1SAD = 0;                                //REG_DM1SAD = NAME, address of DMA source is the digitized music sample
    REG_SD1DAD = 0;                                                   //REG_DM1DAD = REG_SGFIFOA, address of DMA destination is FIFO buffer for direct sound A
    REG_SD1CNT_H = 0;                                                       //REG_DM1CNT_H = 1011 0110 0100 0000, DMA destination is fixed, repeat transfer of 4 bytes when FIFO , buffer is empty, enable DMA 1 (number of DMA transfers is ignored), INTERRUPT
    REG_TM0SD = 0;                          //REG_TM0D = 65536-(16777216/frequency);, play sample every 16777216/frequency CPU cycles
    REG_TMSDCNT = 0;
    SetMode(0);
    sprites[0].attribute0 = COLOR_16 | SQUARE | 0;
    sprites[0].attribute1 = SIZE_64 | 0;
    sprites[0].attribute2 = 512 + 0 | PALETTE(0); // NOTE: mode4 doesn't support the first tiles, so offset of 512 is requirerd
    sprites[1].attribute0 = COLOR_16 | TALL | 0 ;
    sprites[1].attribute1 = SIZE_64 | 0;
    sprites[1].attribute2 = 576 | PALETTE(1); // NOTE: mode4 doesn't support the first tiles, so offset of 512 is requirerd
    sprites[2].attribute0 = COLOR_16 | WIDE | 144;
    sprites[2].attribute1 = SIZE_64 | 1;
    sprites[2].attribute2 = 640 | PALETTE(2); // NOTE: mode4 doesn't support the first tiles, so offset of 512 is requirerd2

    MoveSprite(&sprites[1], 240, 91);
    loadSpriteGraphics((void*)t_idle_1Tiles, 1024);
    loadSpriteGraphics((void*)s_sleep_1Tiles, 512);
    offsetOAM = 2048;
    loadSpriteGraphics((void*)lifecnt_tTiles, 512);
    loadSmallSpritePal((void*)t_idle_1Pal);
    for (i = 0; i < 16; i++) {
        OBJPaletteMem[0x10 + i] = s_sleep_1Pal[i];
    }
    for (i = 0; i < 16; i++) {
        OBJPaletteMem[0x20 + i] = lifecnt_tPal[i];
    }
    Sleep(60);
    FadeOut(0);
    SetMode(MODE_4 | BG2_ENABLE);
    setbg2((void*)bg_hill_startBitmap, (void*)bg_hill_startPalette);
    FadeIn(2);
    Sleep(60);
    FadeOut(2);
    MoveSprite(&sprites[0], 88, 98);
    CopyOAM();
    offsetOAM = 0;
    Sleep(1);
    for (x = 0; x < 256; x++) {
        BGPaletteMem[x] = bg_hillPal[x];
    }
    for (x = 0; x < 32800; x++) {
        BGTileMem[x] = bg_hillTiles[x];
    }
    for (x = 0; x < 2048; x++) {
        videoBuffer[x] = bg_hillMap[x];
    }
    REG_BG2CNT = CHAR_BASE(1) | BG_COLOR_256 | BG_MOSAIC_ENABLE;
    SetMode(MODE_0 | BG2_ENABLE | OBJ_MAP_1D | OBJ_ENABLE);
    frames = 0;
    g_newframe = 0;
    FadeIn(2);
    CopyOAM();
    playSound(5);
    hill_enablemusic = 1;
    while (1) {
        if (g_newframe == 0) {
            if (hill_enablemusic == 1) {
                if (!(frames%7680)) {
                    REG_SOUNDCNT1_H = 0;                                                       //REG_SOUNDCNT_H = 0000 1011 0000 0100, volume = 100, sound goes to the left, sound goes to the right, timer 0 is used, FIFO buffer reset
                    REG_SOUNDCNT1_X = 0;                                                       //REG_SOUNDCNT_X = 0000 0000 1000 0000, enable the sound system, DMA 1
                    REG_SD1SAD = 0;                                //REG_DM1SAD = NAME, address of DMA source is the digitized music sample
                    REG_SD1DAD = 0;                                                   //REG_DM1DAD = REG_SGFIFOA, address of DMA destination is FIFO buffer for direct sound A
                    REG_SD1CNT_H = 0;                                                       //REG_DM1CNT_H = 1011 0110 0100 0000, DMA destination is fixed, repeat transfer of 4 bytes when FIFO , buffer is empty, enable DMA 1 (number of DMA transfers is ignored), INTERRUPT
                    REG_TM0SD = 0;                          //REG_TM0D = 65536-(16777216/frequency);, play sample every 16777216/frequency CPU cycles
                    REG_TMSDCNT = 0;
                    playSound(5);
                }
            }
            rng = rand() % 5;
            UpdateBG(&bg3);
            frames++;
            if (keyDown(KEY_LEFT)) {
                bgx -= 3;
                bg3.x -= 3;

                if (hill_enablesonic == 1) {
                    hill_sonix -= 3;
                    MoveSprite(&sprites[1], 240 - hill_sonix, 89);
                }
            }
            if (keyDown(KEY_RIGHT)) {
                bgx += 3;
                bg3.x += 3;
                if (hill_enablesonic == 1) {
                    hill_sonix += 3;
                    MoveSprite(&sprites[1], 240 - hill_sonix, 89);
                }
            }
            if (keyDown(KEY_LEFT)) {
                sprites[0].attribute1 = SIZE_64 | HORIZONTAL_FLIP | 88;
                if (animframe == 1) {
                    loadSpriteGraphics((void*)t_run_1Tiles, 1024);
                    loadSmallSpritePal((void*)t_run_1Pal);
                }
                if (animframe == 2) {
                    loadSpriteGraphics((void*)t_run_2Tiles, 1024);
                    loadSmallSpritePal((void*)t_run_2Pal);
                }
                if (animframe == 3) {
                    loadSpriteGraphics((void*)t_run_3Tiles, 1024);
                    loadSmallSpritePal((void*)t_run_3Pal);
                }
                if (animframe == 4) {
                    loadSpriteGraphics((void*)t_run_4Tiles, 1024);
                    loadSmallSpritePal((void*)t_run_4Pal);
                }
                if (animframe == 5) {
                    loadSpriteGraphics((void*)t_run_5Tiles, 1024);
                    loadSmallSpritePal((void*)t_run_5Pal);
                }
                if (animframe == 6) {
                    loadSpriteGraphics((void*)t_run_6Tiles, 1024);
                    loadSmallSpritePal((void*)t_run_6Pal);
                }
                if (animframe == 7) {
                    loadSpriteGraphics((void*)t_run_7Tiles, 1024);
                    loadSmallSpritePal((void*)t_run_7Pal);
                }
                if (animframe == 8) {
                    loadSpriteGraphics((void*)t_run_8Tiles, 1024);
                    loadSmallSpritePal((void*)t_run_8Pal);
                }
                if (!(frames % 6)) {
                    animframe++;
                    if (animframe == 9) {
                        animframe = 1;
                    }
                }
                pressinglr = 1;
                resetOffset();
            }
            if (keyDown(KEY_RIGHT)) {
                sprites[0].attribute1 = SIZE_64 | 88;
                if (animframe == 1) {
                    loadSpriteGraphics((void*)t_run_1Tiles, 1024);
                    loadSmallSpritePal((void*)t_run_1Pal);
                }
                if (animframe == 2) {
                    loadSpriteGraphics((void*)t_run_2Tiles, 1024);
                    loadSmallSpritePal((void*)t_run_2Pal);
                }
                if (animframe == 3) {
                    loadSpriteGraphics((void*)t_run_3Tiles, 1024);
                    loadSmallSpritePal((void*)t_run_3Pal);
                }
                if (animframe == 4) {
                    loadSpriteGraphics((void*)t_run_4Tiles, 1024);
                    loadSmallSpritePal((void*)t_run_4Pal);
                }
                if (animframe == 5) {
                    loadSpriteGraphics((void*)t_run_5Tiles, 1024);
                    loadSmallSpritePal((void*)t_run_5Pal);
                }
                if (animframe == 6) {
                    loadSpriteGraphics((void*)t_run_6Tiles, 1024);
                    loadSmallSpritePal((void*)t_run_6Pal);
                }
                if (animframe == 7) {
                    loadSpriteGraphics((void*)t_run_7Tiles, 1024);
                    loadSmallSpritePal((void*)t_run_7Pal);
                }
                if (animframe == 8) {
                    loadSpriteGraphics((void*)t_run_8Tiles, 1024);
                    loadSmallSpritePal((void*)t_run_8Pal);
                }
                if (!(frames % 6)) {
                    animframe++;
                    if (animframe == 9) {
                        animframe = 1;
                    }
                }
                pressinglr = 1;
                resetOffset();
            }
            if (!((keyDown(KEY_RIGHT))OR(keyDown(KEY_LEFT)))) {
                animframe = 6;
                if (pressinglr == 1) {
                    loadSpriteGraphics((void*)t_idle_1Tiles, 1024);
                    loadSmallSpritePal((void*)t_idle_1Pal);
                    pressinglr = 0;
                }
            }
            resetOffset();
            if (bgx > 4096) {
                if (hill_enablesonic == 0) {
                    hill_enablesonic = 1;
                }
            }
            if (bgx > 3072) {
                hill_enablemusic = 0;
                REG_SOUNDCNT1_H = 0;                                                       //REG_SOUNDCNT_H = 0000 1011 0000 0100, volume = 100, sound goes to the left, sound goes to the right, timer 0 is used, FIFO buffer reset
                REG_SOUNDCNT1_X = 0;                                                       //REG_SOUNDCNT_X = 0000 0000 1000 0000, enable the sound system, DMA 1
                REG_SD1SAD = 0;                                //REG_DM1SAD = NAME, address of DMA source is the digitized music sample
                REG_SD1DAD = 0;                                                   //REG_DM1DAD = REG_SGFIFOA, address of DMA destination is FIFO buffer for direct sound A
                REG_SD1CNT_H = 0;                                                       //REG_DM1CNT_H = 1011 0110 0100 0000, DMA destination is fixed, repeat transfer of 4 bytes when FIFO , buffer is empty, enable DMA 1 (number of DMA transfers is ignored), INTERRUPT
                REG_TM0SD = 0;                          //REG_TM0D = 65536-(16777216/frequency);, play sample every 16777216/frequency CPU cycles
                REG_TMSDCNT = 0;
            }
            if (bgx > 4123) {
                sprites[0].attribute1 = SIZE_64 | 88;
                CopyOAM();
                offsetOAM = 0;
                loadSpriteGraphics((void*)t_pointTiles, 1024);
                loadSmallSpritePal((void*)t_pointPal);
                Sleep(60);
                offsetOAM = 0;
                loadSpriteGraphics((void*)t_surpTiles, 1024);
                loadSmallSpritePal((void*)t_surpPal);
                Sleep(180);
                animframe = 1;
                frames = 0;
                offsetOAM = 0;
                for(i=0; i<85; i++) {
                    rng = rand() % 5;
                    frames++;
                    Sleep(7);
                    if (animframe == 1) {
                        loadSpriteGraphics((void*)t_run_1Tiles, 1024);
                        loadSmallSpritePal((void*)t_run_1Pal);
                    }
                    if (animframe == 2) {
                        loadSpriteGraphics((void*)t_run_2Tiles, 1024);
                        loadSmallSpritePal((void*)t_run_2Pal);
                    }
                    if (animframe == 3) {
                        loadSpriteGraphics((void*)t_run_3Tiles, 1024);
                        loadSmallSpritePal((void*)t_run_3Pal);
                    }
                    if (animframe == 4) {
                        loadSpriteGraphics((void*)t_run_4Tiles, 1024);
                        loadSmallSpritePal((void*)t_run_4Pal);
                    }
                    if (animframe == 5) {
                        loadSpriteGraphics((void*)t_run_5Tiles, 1024);
                        loadSmallSpritePal((void*)t_run_5Pal);
                    }
                    if (animframe == 6) {
                        loadSpriteGraphics((void*)t_run_6Tiles, 1024);
                        loadSmallSpritePal((void*)t_run_6Pal);
                    }
                    if (animframe == 7) {
                        loadSpriteGraphics((void*)t_run_7Tiles, 1024);
                        loadSmallSpritePal((void*)t_run_7Pal);
                    }
                    if (animframe == 8) {
                        loadSpriteGraphics((void*)t_run_8Tiles, 1024);
                        loadSmallSpritePal((void*)t_run_8Pal);
                    }
                    if (!(frames % 8)) {
                        animframe++;
                        if (animframe == 9) {
                            animframe = 1;
                        }
                    }
                    CopyOAM();
                    offsetOAM = 0;
                    bgx += 1;
                    bg3.x += 1;
                    hill_sonix += 1;
                    MoveSprite(&sprites[1], 240 - hill_sonix, 89);
                    UpdateBG(&bg3);
                }
                loadSpriteGraphics((void*)t_heyTiles, 1024);
                loadSmallSpritePal((void*)t_heyPal);
                loadSpriteGraphics((void*)s_sleep_2Tiles, 512);
                for (i = 0; i < 16; i++) {
                    OBJPaletteMem[0x10 + i] = s_sleep_2Pal[i];
                }
                Sleep(15);
                loopbreak = 1;
                break;
            }
            if (bgx > 2048) {
                if (hill_statictrigger == 0) {
					REG_SOUNDCNT1_H = 0;                                                       //REG_SOUNDCNT_H = 0000 1011 0000 0100, volume = 100, sound goes to the left, sound goes to the right, timer 0 is used, FIFO buffer reset
					REG_SOUNDCNT1_X = 0;                                                       //REG_SOUNDCNT_X = 0000 0000 1000 0000, enable the sound system, DMA 1
					REG_SD1SAD = 0;                                //REG_DM1SAD = NAME, address of DMA source is the digitized music sample
					REG_SD1DAD = 0;                                                   //REG_DM1DAD = REG_SGFIFOA, address of DMA destination is FIFO buffer for direct sound A
					REG_SD1CNT_H = 0;                                                       //REG_DM1CNT_H = 1011 0110 0100 0000, DMA destination is fixed, repeat transfer of 4 bytes when FIFO , buffer is empty, enable DMA 1 (number of DMA transfers is ignored), INTERRUPT
					REG_TM0SD = 0;                          //REG_TM0D = 65536-(16777216/frequency);, play sample every 16777216/frequency CPU cycles
					REG_TMSDCNT = 0;
					playSound(7);
                    WaitForVblank();
                    SetMode(MODE_4 | BG2_ENABLE);
                    for (i = 0; i <20; i++) {
                        rng = rand() % 5;
						WaitForVblank();
						setbg2novb((u16*)bg_static_1Bitmap, (u16*)bg_static_1Palette);
						WaitForVblank();
						setbg2novb((u16*)bg_static_2Bitmap, (u16*)bg_static_2Palette);
						WaitForVblank();
						setbg2novb((u16*)bg_static_3Bitmap, (u16*)bg_static_3Palette);
                    }
                    WaitForVblank();
                    memcpy(BGPaletteMem, bg_hill_bluudyPal, 512);
                    memcpy(BGTileMem, bg_hill_bluudyTiles, 65600);
                    memcpy(videoBuffer, bg_hill_bluudyMap, 2048);
                    REG_BG2CNT = CHAR_BASE(1) | BG_COLOR_256 | BG_MOSAIC_ENABLE;
                    SetMode(MODE_0 | BG2_ENABLE | OBJ_MAP_1D | OBJ_ENABLE);
                    hill_statictrigger = 1;
					frames = 0;
                }
            }
            g_newframe = 1;
        }
        while (REG_VCOUNT < 160);
        CopyOAM();
        g_newframe = 0;
        if (loopbreak == 1) {
            break;
        }
    }
    bg3.x = 0;
    bgx = 0;
    loopbreak = 0;
    FadeOut(0);
    Sleep(60);
    SetMode(MODE_4 | BG2_ENABLE);
    setbg2((u16*)bg_playBitmap, (u16*)bg_playPalette);
	playSound(6);
    FadeIn(3);
	Sleep(45);
	REG_SOUNDCNT1_H = 0;                                                       //REG_SOUNDCNT_H = 0000 1011 0000 0100, volume = 100, sound goes to the left, sound goes to the right, timer 0 is used, FIFO buffer reset
	REG_SOUNDCNT1_X = 0;                                                       //REG_SOUNDCNT_X = 0000 0000 1000 0000, enable the sound system, DMA 1
	REG_SD1SAD = 0;                                //REG_DM1SAD = NAME, address of DMA source is the digitized music sample
	REG_SD1DAD = 0;                                                   //REG_DM1DAD = REG_SGFIFOA, address of DMA destination is FIFO buffer for direct sound A
	REG_SD1CNT_H = 0;                                                       //REG_DM1CNT_H = 1011 0110 0100 0000, DMA destination is fixed, repeat transfer of 4 bytes when FIFO , buffer is empty, enable DMA 1 (number of DMA transfers is ignored), INTERRUPT
	REG_TM0SD = 0;                          //REG_TM0D = 65536-(16777216/frequency);, play sample every 16777216/frequency CPU cycles
	REG_TMSDCNT = 0;
    Sleep(375);
    FadeOut(3);
    setbg2((u16*)bg_hide_startBitmap, (u16*)bg_hide_startPalette);
    FadeIn(2);
    Sleep(60);
    FadeOut(2);
    sprites[1].attribute0 = COLOR_16 | SQUARE | 240;
    sprites[1].attribute1 = SIZE_64 | 160;
    sprites[1].attribute2 = 576 | PALETTE(1); // NOTE: mode4 doesn't support the first tiles, so offset of 512 is requirerd
    offsetOAM = 0;
    for (x = 0; x < 256; x++) {
        BGPaletteMem[x] = bg_hidePal[x];
    }
    for (x = 0; x < 32800; x++) {
        BGTileMem[x] = bg_hideTiles[x];
    }
    for (x = 0; x < 2048; x++) {
        videoBuffer[x] = bg_hideMap[x];
    }
	offsetOAM = 0;
    loadSpriteGraphics((void*)t_spooked_1Tiles, 1024);
    loadSpriteGraphics((void*)s_crouchTiles, 1024);
    loadSpriteGraphics((void*)lifecnt_tTiles, 512);
    for (i = 0; i < 16; i++) {
        OBJPaletteMem[0x20 + i] = lifecnt_tPal[i];
    }
    loadSmallSpritePal((void*)t_spooked_1Pal);
    for (i = 0; i < 16; i++) {
        OBJPaletteMem[0x10 + i] = s_sleep_1Pal[i];
    }
    MoveSprite(&sprites[1], 240, 160);
    MoveSprite(&sprites[0], 88, 109);
    REG_BG2CNT = CHAR_BASE(1) | BG_COLOR_256 | BG_MOSAIC_ENABLE;
    SetMode(MODE_0 | BG2_ENABLE | OBJ_MAP_1D | OBJ_ENABLE);
    frames = 0;
    g_newframe = 0;
    CopyOAM();
    UpdateBG(&bg3);
    FadeIn(2);
    sprites[2].attribute2 = 640 | PALETTE(2); // NOTE: mode4 doesn't support the first tiles, so offset of 512 is requirerd
    bg3.x = 0;
    bgx = 0;
	frames = 0;
	playSound(9);
    while (1) {
		WaitForVblank();
			if (!(frames%2220)) {
				REG_SOUNDCNT1_H = 0;                                                       //REG_SOUNDCNT_H = 0000 1011 0000 0100, volume = 100, sound goes to the left, sound goes to the right, timer 0 is used, FIFO buffer reset
				REG_SOUNDCNT1_X = 0;                                                       //REG_SOUNDCNT_X = 0000 0000 1000 0000, enable the sound system, DMA 1
				REG_SD1SAD = 0;                                //REG_DM1SAD = NAME, address of DMA source is the digitized music sample
				REG_SD1DAD = 0;                                                   //REG_DM1DAD = REG_SGFIFOA, address of DMA destination is FIFO buffer for direct sound A
				REG_SD1CNT_H = 0;                                                       //REG_DM1CNT_H = 1011 0110 0100 0000, DMA destination is fixed, repeat transfer of 4 bytes when FIFO , buffer is empty, enable DMA 1 (number of DMA transfers is ignored), INTERRUPT
				REG_TM0SD = 0;                          //REG_TM0D = 65536-(16777216/frequency);, play sample every 16777216/frequency CPU cycles
				REG_TMSDCNT = 0;
				playSound(9);
			}
            rng = rand() % 5;
            UpdateBG(&bg3);
            frames++;
            if (keyDown(KEY_LEFT)) {
                bgx -= 3;
                bg3.x -= 3;
            }
            if (keyDown(KEY_RIGHT)) {
                bgx += 3;
                bg3.x += 3;
            }
            if (keyDown(KEY_LEFT)) {
                sprites[0].attribute1 = SIZE_64 | HORIZONTAL_FLIP | 88;
                if (animframe == 1) {
                    loadSpriteGraphics((void*)t_run_1Tiles, 1024);
                    loadSmallSpritePal((void*)t_run_1Pal);
                }
                if (animframe == 2) {
                    loadSpriteGraphics((void*)t_run_2Tiles, 1024);
                    loadSmallSpritePal((void*)t_run_2Pal);
                }
                if (animframe == 3) {
                    loadSpriteGraphics((void*)t_run_3Tiles, 1024);
                    loadSmallSpritePal((void*)t_run_3Pal);
                }
                if (animframe == 4) {
                    loadSpriteGraphics((void*)t_run_4Tiles, 1024);
                    loadSmallSpritePal((void*)t_run_4Pal);
                }
                if (animframe == 5) {
                    loadSpriteGraphics((void*)t_run_5Tiles, 1024);
                    loadSmallSpritePal((void*)t_run_5Pal);
                }
                if (animframe == 6) {
                    loadSpriteGraphics((void*)t_run_6Tiles, 1024);
                    loadSmallSpritePal((void*)t_run_6Pal);
                }
                if (animframe == 7) {
                    loadSpriteGraphics((void*)t_run_7Tiles, 1024);
                    loadSmallSpritePal((void*)t_run_7Pal);
                }
                if (animframe == 8) {
                    loadSpriteGraphics((void*)t_run_8Tiles, 1024);
                    loadSmallSpritePal((void*)t_run_8Pal);
                }
                if (!(frames % 6)) {
                    animframe++;
                    if (animframe == 9) {
                        animframe = 1;
                    }
                }
                pressinglr = 1;
                resetOffset();
            }
            if (keyDown(KEY_RIGHT)) {
                sprites[0].attribute1 = SIZE_64 | 88;
                if (animframe == 1) {
                    loadSpriteGraphics((void*)t_run_1Tiles, 1024);
                    loadSmallSpritePal((void*)t_run_1Pal);
                }
                if (animframe == 2) {
                    loadSpriteGraphics((void*)t_run_2Tiles, 1024);
                    loadSmallSpritePal((void*)t_run_2Pal);
                }
                if (animframe == 3) {
                    loadSpriteGraphics((void*)t_run_3Tiles, 1024);
                    loadSmallSpritePal((void*)t_run_3Pal);
                }
                if (animframe == 4) {
                    loadSpriteGraphics((void*)t_run_4Tiles, 1024);
                    loadSmallSpritePal((void*)t_run_4Pal);
                }
                if (animframe == 5) {
                    loadSpriteGraphics((void*)t_run_5Tiles, 1024);
                    loadSmallSpritePal((void*)t_run_5Pal);
                }
                if (animframe == 6) {
                    loadSpriteGraphics((void*)t_run_6Tiles, 1024);
                    loadSmallSpritePal((void*)t_run_6Pal);
                }
                if (animframe == 7) {
                    loadSpriteGraphics((void*)t_run_7Tiles, 1024);
                    loadSmallSpritePal((void*)t_run_7Pal);
                }
                if (animframe == 8) {
                    loadSpriteGraphics((void*)t_run_8Tiles, 1024);
                    loadSmallSpritePal((void*)t_run_8Pal);
                }
                if (!(frames % 6)) {
                    animframe++;
                    if (animframe == 9) {
                        animframe = 1;
                    }
                }
                pressinglr = 1;
                resetOffset();
            }
            if (!((keyDown(KEY_RIGHT))OR(keyDown(KEY_LEFT)))) {
                sprites[0].attribute1 = SIZE_64 | 88;
                animframe = 6;
                if (pressinglr == 1) {
                    pressinglr = 0;
                    loadSpriteGraphics((void*)t_spooked_1Tiles, 1024);
                    loadSmallSpritePal((void*)t_spooked_1Pal);
                }
            }
            resetOffset();
            g_newframe = 1;
        while (REG_VCOUNT < 160);
        CopyOAM();
        g_newframe = 0;
        if (loopbreak == 1) {
            break;
        }
        hide_spookpos = 0;
        if (bgx > 1024) {
            if (!(hide_2spooky4metrig == 1)) {
                frames = 0;
                offsetOAM = 0;
                sprites[1].attribute0 = COLOR_16 | SQUARE | SIZE_DOUBLE | ROTATION_FLAG | 0x200 | 0;
                sprites[1].attribute1 = SIZE_64 | ROTDATA(1) | 0;
                sprites[1].attribute2 = PALETTE(1) | 576; // NOTE: mode4 doesn't support the first tiles, so offset of 512 is requirerd
                RotateSprite(1, 0, 255, 255);
				REG_SOUNDCNT1_H = 0;                                                       //REG_SOUNDCNT_H = 0000 1011 0000 0100, volume = 100, sound goes to the left, sound goes to the right, timer 0 is used, FIFO buffer reset
				REG_SOUNDCNT1_X = 0;                                                       //REG_SOUNDCNT_X = 0000 0000 1000 0000, enable the sound system, DMA 1
				REG_SD1SAD = 0;                                //REG_DM1SAD = NAME, address of DMA source is the digitized music sample
				REG_SD1DAD = 0;                                                   //REG_DM1DAD = REG_SGFIFOA, address of DMA destination is FIFO buffer for direct sound A
				REG_SD1CNT_H = 0;                                                       //REG_DM1CNT_H = 1011 0110 0100 0000, DMA destination is fixed, repeat transfer of 4 bytes when FIFO , buffer is empty, enable DMA 1 (number of DMA transfers is ignored), INTERRUPT
				REG_TM0SD = 0;                          //REG_TM0D = 65536-(16777216/frequency);, play sample every 16777216/frequency CPU cycles
				REG_TMSDCNT = 0;
                playSound(1);
                for (frames = 0; frames < 120;) {
                    WaitForVblank();
                    frames++;
                    if (!(frames % 5)) {
                        hide_sonicspookcnt++;
                        if (hide_sonicspookcnt == 0) {
                            MoveSprite(&sprites[1], 64-32, 64-32);
                            offsetOAM = 1024;
                            loadSpriteGraphics((void*)s_spook_1Tiles, 1024);
                            RotateSprite(1, 0, 128, 128);
                            for (i = 0; i < 16; i++) {
                                OBJPaletteMem[0x10 + i] = s_spook_1Pal[i];
                            }
                        }
                        if (hide_sonicspookcnt == 1) {
                            MoveSprite(&sprites[1], 203-32, 48-32);
                            offsetOAM = 1024;
                            loadSpriteGraphics((void*)s_spook_3Tiles, 1024);
                            RotateSprite(1, 0, 128, 128);
                            for (i = 0; i < 16; i++) {
                                OBJPaletteMem[0x10 + i] = s_spook_3Pal[i];
                            }
                        }
                        if (hide_sonicspookcnt == 2) {
                            MoveSprite(&sprites[1], 48-32, 48-32);
                            offsetOAM = 1024;
                            loadSpriteGraphics((void*)s_spook_2Tiles, 1024);
                            RotateSprite(1, 0, 128, 128);
                            for (i = 0; i < 16; i++) {
                                OBJPaletteMem[0x10 + i] = s_spook_2Pal[i];
                            }
                        }
                        if (hide_sonicspookcnt == 3) {
                            MoveSprite(&sprites[1], 88 - 32, 48 - 32);
                            offsetOAM = 1024;
                            loadSpriteGraphics((void*)s_spook_5Tiles, 1024);
                            RotateSprite(1, 0, 128, 128);
                            for (i = 0; i < 16; i++) {
                                OBJPaletteMem[0x10 + i] = s_spook_5Pal[i];
                            }
                        }
                        if (hide_sonicspookcnt == 4) {
                            MoveSprite(&sprites[1], 103-32, 61-32);
                            offsetOAM = 1024;
                            loadSpriteGraphics((void*)s_spook_4Tiles, 1024);
                            RotateSprite(1, 0, 0, 0);
                            for (i = 0; i < 16; i++) {
                                OBJPaletteMem[0x10 + i] = s_spook_4Pal[i];
                            }
                        }
                        if (hide_sonicspookcnt == 5) {
                            hide_sonicspookcnt = -1;
                        }
                        offsetOAM = 0;
                    }
                    UpdateBG(&bg3);
                    if (keyDown(KEY_LEFT)) {
                        bgx -= 3;
                        bg3.x -= 3;
                    }
                    if (keyDown(KEY_RIGHT)) {
                        bgx += 3;
                        bg3.x += 3;
                    }
                    if (keyDown(KEY_LEFT)) {
                        sprites[0].attribute1 = SIZE_64 | HORIZONTAL_FLIP | 88;
                        if (animframe == 1) {
                            loadSpriteGraphics((void*)t_run_1Tiles, 1024);
                            loadSmallSpritePal((void*)t_run_1Pal);
                        }
                        if (animframe == 2) {
                            loadSpriteGraphics((void*)t_run_2Tiles, 1024);
                            loadSmallSpritePal((void*)t_run_2Pal);
                        }
                        if (animframe == 3) {
                            loadSpriteGraphics((void*)t_run_3Tiles, 1024);
                            loadSmallSpritePal((void*)t_run_3Pal);
                        }
                        if (animframe == 4) {
                            loadSpriteGraphics((void*)t_run_4Tiles, 1024);
                            loadSmallSpritePal((void*)t_run_4Pal);
                        }
                        if (animframe == 5) {
                            loadSpriteGraphics((void*)t_run_5Tiles, 1024);
                            loadSmallSpritePal((void*)t_run_5Pal);
                        }
                        if (animframe == 6) {
                            loadSpriteGraphics((void*)t_run_6Tiles, 1024);
                            loadSmallSpritePal((void*)t_run_6Pal);
                        }
                        if (animframe == 7) {
                            loadSpriteGraphics((void*)t_run_7Tiles, 1024);
                            loadSmallSpritePal((void*)t_run_7Pal);
                        }
                        if (animframe == 8) {
                            loadSpriteGraphics((void*)t_run_8Tiles, 1024);
                            loadSmallSpritePal((void*)t_run_8Pal);
                        }
                        if (!(frames % 6)) {
                            animframe++;
                            if (animframe == 9) {
                                animframe = 1;
                            }
                        }
                        pressinglr = 1;
                        resetOffset();
                    }
                    if (keyDown(KEY_RIGHT)) {
                        sprites[0].attribute1 = SIZE_64 | 88;
                        if (animframe == 1) {
                            loadSpriteGraphics((void*)t_run_1Tiles, 1024);
                            loadSmallSpritePal((void*)t_run_1Pal);
                        }
                        if (animframe == 2) {
                            loadSpriteGraphics((void*)t_run_2Tiles, 1024);
                            loadSmallSpritePal((void*)t_run_2Pal);
                        }
                        if (animframe == 3) {
                            loadSpriteGraphics((void*)t_run_3Tiles, 1024);
                            loadSmallSpritePal((void*)t_run_3Pal);
                        }
                        if (animframe == 4) {
                            loadSpriteGraphics((void*)t_run_4Tiles, 1024);
                            loadSmallSpritePal((void*)t_run_4Pal);
                        }
                        if (animframe == 5) {
                            loadSpriteGraphics((void*)t_run_5Tiles, 1024);
                            loadSmallSpritePal((void*)t_run_5Pal);
                        }
                        if (animframe == 6) {
                            loadSpriteGraphics((void*)t_run_6Tiles, 1024);
                            loadSmallSpritePal((void*)t_run_6Pal);
                        }
                        if (animframe == 7) {
                            loadSpriteGraphics((void*)t_run_7Tiles, 1024);
                            loadSmallSpritePal((void*)t_run_7Pal);
                        }
                        if (animframe == 8) {
                            loadSpriteGraphics((void*)t_run_8Tiles, 1024);
                            loadSmallSpritePal((void*)t_run_8Pal);
                        }
                        if (!(frames % 6)) {
                            animframe++;
                            if (animframe == 9) {
                                animframe = 1;
                            }
                        }
                        pressinglr = 1;
                        resetOffset();
                    }
                    if (!((keyDown(KEY_RIGHT))OR(keyDown(KEY_LEFT)))) {
                        sprites[0].attribute1 = SIZE_64 | 88;
                        animframe = 6;
                        if (pressinglr == 1) {
                            pressinglr = 0;
                            loadSpriteGraphics((void*)t_spooked_1Tiles, 1024);
                            loadSmallSpritePal((void*)t_spooked_1Pal);
                        }
                    }
                    resetOffset();
                    CopyOAM();
                }
                REG_SOUNDCNT1_H = 0;                                                       //REG_SOUNDCNT_H = 0000 1011 0000 0100, volume = 100, sound goes to the left, sound goes to the right, timer 0 is used, FIFO buffer reset
                REG_SOUNDCNT1_X = 0;                                                       //REG_SOUNDCNT_X = 0000 0000 1000 0000, enable the sound system, DMA 1
                REG_SD1SAD = 0;                                //REG_DM1SAD = NAME, address of DMA source is the digitized music sample
                REG_SD1DAD = 0;                                                   //REG_DM1DAD = REG_SGFIFOA, address of DMA destination is FIFO buffer for direct sound A
                REG_SD1CNT_H = 0;                                                       //REG_DM1CNT_H = 1011 0110 0100 0000, DMA destination is fixed, repeat transfer of 4 bytes when FIFO , buffer is empty, enable DMA 1 (number of DMA transfers is ignored), INTERRUPT
                REG_TM0SD = 0;                          //REG_TM0D = 65536-(16777216/frequency);, play sample every 16777216/frequency CPU cycles
                REG_TMSDCNT = 0;
                sprites[1].attribute0 = COLOR_16 | SQUARE | 0;
                sprites[1].attribute1 = SIZE_64 | 0;
                sprites[1].attribute2 = 576 | PALETTE(1); // NOTE: mode4 doesn't support the first tiles, so offset of 512 is requirerd
                hide_2spooky4metrig = 1;
                hide_enablesonic = 1;
                offsetOAM = 1024;
                loadSpriteGraphics((void*)s_gattagofastTiles, 1024);
                for (i = 0; i < 16; i++) {
                    OBJPaletteMem[0x10 + i] = s_gattagofastPal[i];
                }
                MoveSprite(&sprites[1], 0, 106);
                CopyOAM();
				playSound(14);
                while (!(hide_sonix > 98)) {
                    hide_sonix += 2;
                    MoveSprite(&sprites[1], hide_sonix, 106);
                    CopyOAM();
                    WaitForVblank();
                }
				REG_SOUNDCNT1_H = 0;                                                       //REG_SOUNDCNT_H = 0000 1011 0000 0100, volume = 100, sound goes to the left, sound goes to the right, timer 0 is used, FIFO buffer reset
				REG_SOUNDCNT1_X = 0;                                                       //REG_SOUNDCNT_X = 0000 0000 1000 0000, enable the sound system, DMA 1
				REG_SD1SAD = 0;                                //REG_DM1SAD = NAME, address of DMA source is the digitized music sample
				REG_SD1DAD = 0;                                                   //REG_DM1DAD = REG_SGFIFOA, address of DMA destination is FIFO buffer for direct sound A
				REG_SD1CNT_H = 0;                                                       //REG_DM1CNT_H = 1011 0110 0100 0000, DMA destination is fixed, repeat transfer of 4 bytes when FIFO , buffer is empty, enable DMA 1 (number of DMA transfers is ignored), INTERRUPT
				REG_TM0SD = 0;                          //REG_TM0D = 65536-(16777216/frequency);, play sample every 16777216/frequency CPU cycles
				REG_TMSDCNT = 0;
                MoveSprite(&sprites[1], 240, 160);
                CopyOAM();
                offsetOAM = 0;
                WaitForVblank();
                loadSpriteGraphics((void*)t_crai_1Tiles, 1024);
                loadSpriteGraphics((void*)s_crouchTiles, 1024);
                loadSmallSpritePal((void*)t_crai_1Pal);
                for (i = 0; i < 16; i++) {
                    OBJPaletteMem[0x10 + i] = s_crouchPal[i];
                }
                offsetOAM = 0;
                for (i = 0; i < 100; i++) {
                    Sleep(2);
                    loadSpriteGraphics((void*)t_crai_1Tiles, 1024);
                    loadSmallSpritePal((void*)t_crai_1Pal);
                    offsetOAM = 0;
                    Sleep(2);
                    loadSpriteGraphics((void*)t_crai_2Tiles, 1024);
                    loadSmallSpritePal((void*)t_crai_2Pal);
                    offsetOAM = 0;
                }
                sprites[1].attribute1 = SIZE_64 | HORIZONTAL_FLIP | 75;
                loadSpriteGraphics((void*)t_spooked_2Tiles, 1024);
                loadSmallSpritePal((void*)t_spooked_2Pal);
                MoveSprite(&sprites[1], 96, 96);
                CopyOAM();
                Sleep(300);
                loadSpriteGraphics((void*)s_pwnTiles, 1024);
                for (i = 0; i < 16; i++) {
                    OBJPaletteMem[0x10 + i] = s_pwnPal[i];
                }
                offsetOAM = 0;
                MoveSprite(&sprites[0], 88, 100);
                CopyOAM();
                loadSpriteGraphics((void*)t_dedTiles, 1024);
                loadSmallSpritePal((void*)t_dedPal);
                playSound(2);
                Sleep(10);
                memcpy(BGPaletteMem, 0x02000000, 256);
                SetMode(MODE_4 | BG2_ENABLE);
                FadeOut(0);
                Sleep(240);
                REG_SOUNDCNT1_H = 0;                                                       //REG_SOUNDCNT_H = 0000 1011 0000 0100, volume = 100, sound goes to the left, sound goes to the right, timer 0 is used, FIFO buffer reset
                REG_SOUNDCNT1_X = 0;                                                       //REG_SOUNDCNT_X = 0000 0000 1000 0000, enable the sound system, DMA 1
                REG_SD1SAD = 0;                                //REG_DM1SAD = NAME, address of DMA source is the digitized music sample
                REG_SD1DAD = 0;                                                   //REG_DM1DAD = REG_SGFIFOA, address of DMA destination is FIFO buffer for direct sound A
                REG_SD1CNT_H = 0;                                                       //REG_DM1CNT_H = 1011 0110 0100 0000, DMA destination is fixed, repeat transfer of 4 bytes when FIFO , buffer is empty, enable DMA 1 (number of DMA transfers is ignored), INTERRUPT
                REG_TM0SD = 0;                          //REG_TM0D = 65536-(16777216/frequency);, play sample every 16777216/frequency CPU cycles
                REG_TMSDCNT = 0;
                Sleep(50);
                setbg2((u16*)bg_slowBitmap, (u16*)bg_slowPalette);
                playSound(1);
                FadeIn(2);
                Sleep(90);
                REG_SOUNDCNT1_H = 0;                                                       //REG_SOUNDCNT_H = 0000 1011 0000 0100, volume = 100, sound goes to the left, sound goes to the right, timer 0 is used, FIFO buffer reset
                REG_SOUNDCNT1_X = 0;                                                       //REG_SOUNDCNT_X = 0000 0000 1000 0000, enable the sound system, DMA 1
                REG_SD1SAD = 0;                                //REG_DM1SAD = NAME, address of DMA source is the digitized music sample
                REG_SD1DAD = 0;                                                   //REG_DM1DAD = REG_SGFIFOA, address of DMA destination is FIFO buffer for direct sound A
                REG_SD1CNT_H = 0;                                                       //REG_DM1CNT_H = 1011 0110 0100 0000, DMA destination is fixed, repeat transfer of 4 bytes when FIFO , buffer is empty, enable DMA 1 (number of DMA transfers is ignored), INTERRUPT
                REG_TM0SD = 0;                          //REG_TM0D = 65536-(16777216/frequency);, play sample every 16777216/frequency CPU cycles
                REG_TMSDCNT = 0;
                Sleep(330);
                FadeOut(2);
                setbg2((u16*)bg_datasel_kBitmap, (u16*)bg_datasel_kPalette);
                FadeIn(2);
				playSound(3);
				while (!(keyDown(KEY_START))) {
					WaitForVblank();
					frames++;
					if (!(frames % 540)) {
						REG_SOUNDCNT1_H = 0;                                                       //REG_SOUNDCNT_H = 0000 1011 0000 0100, volume = 100, sound goes to the left, sound goes to the right, timer 0 is used, FIFO buffer reset
						REG_SOUNDCNT1_X = 0;                                                       //REG_SOUNDCNT_X = 0000 0000 1000 0000, enable the sound system, DMA 1
						REG_SD1SAD = 0;                                //REG_DM1SAD = NAME, address of DMA source is the digitized music sample
						REG_SD1DAD = 0;                                                   //REG_DM1DAD = REG_SGFIFOA, address of DMA destination is FIFO buffer for direct sound A
						REG_SD1CNT_H = 0;                                                       //REG_DM1CNT_H = 1011 0110 0100 0000, DMA destination is fixed, repeat transfer of 4 bytes when FIFO , buffer is empty, enable DMA 1 (number of DMA transfers is ignored), INTERRUPT
						REG_TM0SD = 0;                          //REG_TM0D = 65536-(16777216/frequency);, play sample every 16777216/frequency CPU cycles
						REG_TMSDCNT = 0;
						playSound(3);
					}
				}
				REG_SOUNDCNT1_H = 0;                                                       //REG_SOUNDCNT_H = 0000 1011 0000 0100, volume = 100, sound goes to the left, sound goes to the right, timer 0 is used, FIFO buffer reset
				REG_SOUNDCNT1_X = 0;                                                       //REG_SOUNDCNT_X = 0000 0000 1000 0000, enable the sound system, DMA 1
				REG_SD1SAD = 0;                                //REG_DM1SAD = NAME, address of DMA source is the digitized music sample
				REG_SD1DAD = 0;                                                   //REG_DM1DAD = REG_SGFIFOA, address of DMA destination is FIFO buffer for direct sound A
				REG_SD1CNT_H = 0;                                                       //REG_DM1CNT_H = 1011 0110 0100 0000, DMA destination is fixed, repeat transfer of 4 bytes when FIFO , buffer is empty, enable DMA 1 (number of DMA transfers is ignored), INTERRUPT
				REG_TM0SD = 0;                          //REG_TM0D = 65536-(16777216/frequency);, play sample every 16777216/frequency CPU cycles
				REG_TMSDCNT = 0;
                Sleep(240);
                playSound(1);
                Sleep(120);
                REG_SOUNDCNT1_H = 0;                                                       //REG_SOUNDCNT_H = 0000 1011 0000 0100, volume = 100, sound goes to the left, sound goes to the right, timer 0 is used, FIFO buffer reset
                REG_SOUNDCNT1_X = 0;                                                       //REG_SOUNDCNT_X = 0000 0000 1000 0000, enable the sound system, DMA 1
                REG_SD1SAD = 0;                                //REG_DM1SAD = NAME, address of DMA source is the digitized music sample
                REG_SD1DAD = 0;                                                   //REG_DM1DAD = REG_SGFIFOA, address of DMA destination is FIFO buffer for direct sound A
                REG_SD1CNT_H = 0;                                                       //REG_DM1CNT_H = 1011 0110 0100 0000, DMA destination is fixed, repeat transfer of 4 bytes when FIFO , buffer is empty, enable DMA 1 (number of DMA transfers is ignored), INTERRUPT
                REG_TM0SD = 0;                          //REG_TM0D = 65536-(16777216/frequency);, play sample every 16777216/frequency CPU cycles
                REG_TMSDCNT = 0;
                SetMode(0);
                SetMode(0);
                sprites[0].attribute0 = COLOR_16 | SQUARE | 0;
                sprites[0].attribute1 = SIZE_64 | 0;
                sprites[0].attribute2 = 512 + 0 | PALETTE(0); // NOTE: mode4 doesn't support the first tiles, so offset of 512 is requirerd
                sprites[1].attribute0 = COLOR_16 | SQUARE | 0;
                sprites[1].attribute1 = SIZE_64 | HORIZONTAL_FLIP | 0;
                sprites[1].attribute2 = 576 | PALETTE(1); // NOTE: mode4 doesn't support the first tiles, so offset of 512 is requirerd
                MoveSprite(&sprites[1], 240, 91);
                Sleep(60);
                FadeOut(0);
                SetMode(MODE_4 | BG2_ENABLE);
                setbg2((void*)bg_run_startBitmap, (void*)bg_run_startPalette);
                FadeIn(2);
                Sleep(60);
                FadeOut(2);
                MoveSprite(&sprites[0], 88, 64);
                CopyOAM();
                Sleep(1);
                for (x = 0; x < 256; x++) {
                    BGPaletteMem[x] = bg_runPal[x];
                }
                for (x = 0; x < 32800; x++) {
                    BGTileMem[x] = bg_runTiles[x];
                }
                for (x = 0; x < 2048; x++) {
                    videoBuffer[x] = bg_runMap[x];
                }
                offsetOAM = 0;
                loadSpriteGraphics((void*)k_idleTiles, 1024);
                loadSpriteGraphics((void*)s_crouchTiles, 1024);
                loadSmallSpritePal((void*)k_idlePal);
                for (i = 0; i < 16; i++) {
                    OBJPaletteMem[0x10 + i] = s_crouchPal[i];
                }
                loadSpriteGraphics((void*)lifecnt_kTiles, 512);
                for (i = 0; i < 16; i++) {
                    OBJPaletteMem[0x20 + i] = lifecnt_kPal[i];
                }
                REG_BG2CNT = CHAR_BASE(1) | BG_COLOR_256 | BG_MOSAIC_ENABLE;
                SetMode(MODE_0 | BG2_ENABLE | OBJ_MAP_1D | OBJ_ENABLE);
                frames = 0;
                g_newframe = 0;
                FadeIn(2);
                bg3.x = 0;
                bgx = 0;
                UpdateBG(&bg3);
                CopyOAM();
                loopbreak = 1;
                break;
            }
        }
    }
    loopbreak = 0;
    playSound(4);
    frames = 0;
	run_found = 0;
    while (1) {
        WaitForVblank();
		if ((!(frames % 14540))AND(run_found == 0)) {
            REG_SOUNDCNT1_H = 0;                                                       //REG_SOUNDCNT_H = 0000 1011 0000 0100, volume = 100, sound goes to the left, sound goes to the right, timer 0 is used, FIFO buffer reset
            REG_SOUNDCNT1_X = 0;                                                       //REG_SOUNDCNT_X = 0000 0000 1000 0000, enable the sound system, DMA 1
            REG_SD1SAD = 0;                                //REG_DM1SAD = NAME, address of DMA source is the digitized music sample
            REG_SD1DAD = 0;                                                   //REG_DM1DAD = REG_SGFIFOA, address of DMA destination is FIFO buffer for direct sound A
            REG_SD1CNT_H = 0;                                                       //REG_DM1CNT_H = 1011 0110 0100 0000, DMA destination is fixed, repeat transfer of 4 bytes when FIFO , buffer is empty, enable DMA 1 (number of DMA transfers is ignored), INTERRUPT
            REG_TM0SD = 0;                          //REG_TM0D = 65536-(16777216/frequency);, play sample every 16777216/frequency CPU cycles
            REG_TMSDCNT = 0;
            playSound(4);
        }
        rng = rand() % 5;
        UpdateBG(&bg3);
        frames++;
        if (keyDown(KEY_LEFT)) {
            bgx -= 3;
            bg3.x -= 3;
            if(run_found == 1) {
                if(bg3.x < 2048) {
                    bgx = 0;
                    bg3.x = 2048;
                    run_sonicpos = 0;
                }
            }
        }
        if (keyDown(KEY_RIGHT)) {
            bgx += 3;
            bg3.x += 3;
            if(run_found == 1) {
                if(bg3.x > 2096) {
                    bgx = 96;
                    bg3.x = 2096;
                    run_sonicpos = 1;
                }
            }
        }
        if(run_found == 1) {
            run_sonictimer++;
			if (!(frames%1140)) {
				REG_SOUNDCNT1_H = 0;                                                       //REG_SOUNDCNT_H = 0000 1011 0000 0100, volume = 100, sound goes to the left, sound goes to the right, timer 0 is used, FIFO buffer reset
				REG_SOUNDCNT1_X = 0;                                                       //REG_SOUNDCNT_X = 0000 0000 1000 0000, enable the sound system, DMA 1
				REG_SD1SAD = 0;                                //REG_DM1SAD = NAME, address of DMA source is the digitized music sample
				REG_SD1DAD = 0;                                                   //REG_DM1DAD = REG_SGFIFOA, address of DMA destination is FIFO buffer for direct sound A
				REG_SD1CNT_H = 0;                                                       //REG_DM1CNT_H = 1011 0110 0100 0000, DMA destination is fixed, repeat transfer of 4 bytes when FIFO , buffer is empty, enable DMA 1 (number of DMA transfers is ignored), INTERRUPT
				REG_TM0SD = 0;                          //REG_TM0D = 65536-(16777216/frequency);, play sample every 16777216/frequency CPU cycles
				REG_TMSDCNT = 0;
				playSound(10);
			}
            if(run_sonicpos == 0) {
                sprites[1].attribute1 = SIZE_64 | HORIZONTAL_FLIP | 59;
                MoveSprite(&sprites[1], 164-bgx, 59);
				offsetOAM = 1024;
				loadSpriteGraphics((void*)s_tauntTiles, 1024);
				for (i = 0; i < 16; i++) {
					OBJPaletteMem[0x10 + i] = s_tauntPal[i];
				}
				offsetOAM = 0;
            }
            if(run_sonicpos == 1) {
                sprites[1].attribute1 = SIZE_64 | 59;
                MoveSprite(&sprites[1], 116-bgx, 59);
				offsetOAM = 1024;
				loadSpriteGraphics((void*)s_tauntTiles, 1024);
				for (i = 0; i < 16; i++) {
					OBJPaletteMem[0x10 + i] = s_tauntPal[i];
				}
				offsetOAM = 0;
            }
        }
        if (keyDown(KEY_LEFT)) {
            sprites[0].attribute1 = SIZE_64 | HORIZONTAL_FLIP | 88;
            if (animframe == 1) {
                loadSpriteGraphics((void*)k_run_1Tiles, 1024);
                loadSmallSpritePal((void*)k_run_1Pal);
            }
            if (animframe == 2) {
                loadSpriteGraphics((void*)k_run_2Tiles, 1024);
                loadSmallSpritePal((void*)k_run_2Pal);
            }
            if (animframe == 3) {
                loadSpriteGraphics((void*)k_run_3Tiles, 1024);
                loadSmallSpritePal((void*)k_run_3Pal);
            }
            if (animframe == 4) {
                loadSpriteGraphics((void*)k_run_4Tiles, 1024);
                loadSmallSpritePal((void*)k_run_4Pal);
            }
            if (animframe == 5) {
                loadSpriteGraphics((void*)k_run_5Tiles, 1024);
                loadSmallSpritePal((void*)k_run_5Pal);
            }
            if (animframe == 6) {
                loadSpriteGraphics((void*)k_run_6Tiles, 1024);
                loadSmallSpritePal((void*)k_run_6Pal);
            }
            if (animframe == 7) {
                loadSpriteGraphics((void*)k_run_7Tiles, 1024);
                loadSmallSpritePal((void*)k_run_7Pal);
            }
            if (animframe == 8) {
                loadSpriteGraphics((void*)k_run_8Tiles, 1024);
                loadSmallSpritePal((void*)k_run_8Pal);
            }
            if (!(frames % 6)) {
                animframe++;
                if (animframe == 9) {
                    animframe = 1;
                }
            }
            pressinglr = 1;
            resetOffset();
        }
        if (keyDown(KEY_RIGHT)) {
            sprites[0].attribute1 = SIZE_64 | 88;
            if (animframe == 1) {
                loadSpriteGraphics((void*)k_run_1Tiles, 1024);
                loadSmallSpritePal((void*)k_run_1Pal);
            }
            if (animframe == 2) {
                loadSpriteGraphics((void*)k_run_2Tiles, 1024);
                loadSmallSpritePal((void*)k_run_2Pal);
            }
            if (animframe == 3) {
                loadSpriteGraphics((void*)k_run_3Tiles, 1024);
                loadSmallSpritePal((void*)k_run_3Pal);
            }
            if (animframe == 4) {
                loadSpriteGraphics((void*)k_run_4Tiles, 1024);
                loadSmallSpritePal((void*)k_run_4Pal);
            }
            if (animframe == 5) {
                loadSpriteGraphics((void*)k_run_5Tiles, 1024);
                loadSmallSpritePal((void*)k_run_5Pal);
            }
            if (animframe == 6) {
                loadSpriteGraphics((void*)k_run_6Tiles, 1024);
                loadSmallSpritePal((void*)k_run_6Pal);
            }
            if (animframe == 7) {
                loadSpriteGraphics((void*)k_run_7Tiles, 1024);
                loadSmallSpritePal((void*)k_run_7Pal);
            }
            if (animframe == 8) {
                loadSpriteGraphics((void*)k_run_8Tiles, 1024);
                loadSmallSpritePal((void*)k_run_8Pal);
            }
            if (!(frames % 6)) {
                animframe++;
                if (animframe == 9) {
                    animframe = 1;
                }
            }
            pressinglr = 1;
            resetOffset();
        }
        if (!((keyDown(KEY_RIGHT))OR(keyDown(KEY_LEFT)))) {
            animframe = 6;
            if (pressinglr == 1) {
                loadSpriteGraphics((void*)k_idleTiles, 1024);
                loadSmallSpritePal((void*)k_idlePal);
                pressinglr = 0;
            }
        }
        if (bgx > 1536) {
            if (run_statictrigger == 0) {
				REG_SOUNDCNT1_H = 0;                                                       //REG_SOUNDCNT_H = 0000 1011 0000 0100, volume = 100, sound goes to the left, sound goes to the right, timer 0 is used, FIFO buffer reset
				REG_SOUNDCNT1_X = 0;                                                       //REG_SOUNDCNT_X = 0000 0000 1000 0000, enable the sound system, DMA 1
				REG_SD1SAD = 0;                                //REG_DM1SAD = NAME, address of DMA source is the digitized music sample
				REG_SD1DAD = 0;                                                   //REG_DM1DAD = REG_SGFIFOA, address of DMA destination is FIFO buffer for direct sound A
				REG_SD1CNT_H = 0;                                                       //REG_DM1CNT_H = 1011 0110 0100 0000, DMA destination is fixed, repeat transfer of 4 bytes when FIFO , buffer is empty, enable DMA 1 (number of DMA transfers is ignored), INTERRUPT
				REG_TM0SD = 0;                          //REG_TM0D = 65536-(16777216/frequency);, play sample every 16777216/frequency CPU cycles
				REG_TMSDCNT = 0;
				playSound(7);
				WaitForVblank();
				SetMode(MODE_4 | BG2_ENABLE);
				for (i = 0; i < 20; i++) {
					rng = rand() % 5;
					WaitForVblank();
					setbg2novb((u16*)bg_static_1Bitmap, (u16*)bg_static_1Palette);
					WaitForVblank();
					setbg2novb((u16*)bg_static_2Bitmap, (u16*)bg_static_2Palette);
					WaitForVblank();
					setbg2novb((u16*)bg_static_3Bitmap, (u16*)bg_static_3Palette);
				}
				REG_SOUNDCNT1_H = 0;                                                       //REG_SOUNDCNT_H = 0000 1011 0000 0100, volume = 100, sound goes to the left, sound goes to the right, timer 0 is used, FIFO buffer reset
				REG_SOUNDCNT1_X = 0;                                                       //REG_SOUNDCNT_X = 0000 0000 1000 0000, enable the sound system, DMA 1
				REG_SD1SAD = 0;                                //REG_DM1SAD = NAME, address of DMA source is the digitized music sample
				REG_SD1DAD = 0;                                                   //REG_DM1DAD = REG_SGFIFOA, address of DMA destination is FIFO buffer for direct sound A
				REG_SD1CNT_H = 0;                                                       //REG_DM1CNT_H = 1011 0110 0100 0000, DMA destination is fixed, repeat transfer of 4 bytes when FIFO , buffer is empty, enable DMA 1 (number of DMA transfers is ignored), INTERRUPT
				REG_TM0SD = 0;                          //REG_TM0D = 65536-(16777216/frequency);, play sample every 16777216/frequency CPU cycles
				REG_TMSDCNT = 0;
				WaitForVblank();
                memcpy(BGPaletteMem, bg_run_bluudPal, 512);
                memcpy(BGTileMem, bg_run_bluudTiles, 65600);
                memcpy(videoBuffer, bg_run_bluudMap, 2048);
                REG_BG2CNT = CHAR_BASE(1) | BG_COLOR_256 | BG_MOSAIC_ENABLE;
                SetMode(MODE_0 | BG2_ENABLE | OBJ_MAP_1D | OBJ_ENABLE);
                run_statictrigger = 1;
				playSound(4);
            }
        }
        if(bgx > 2048) {
            if(run_enablesonic == 0) {
                offsetOAM = 0;
                loadSpriteGraphics((void*)k_spookTiles, 1024);
                loadSmallSpritePal((void*)k_spookPal);
				loadSpriteGraphics((void*)s_crouchTiles, 1024);
				for (i = 0; i < 16; i++) {
					OBJPaletteMem[0x10 + i] = s_crouchPal[i];
				}
                sprites[0].attribute1 = SIZE_64 | HORIZONTAL_FLIP | 75;
                sprites[1].attribute1 = SIZE_64 | HORIZONTAL_FLIP | 75;
                MoveSprite(&sprites[1], 96, 59);
                CopyOAM();
                Sleep(10);
				WaitForVblank();
				playSound(7);
				SetMode(MODE_4 | BG2_ENABLE);
				for (i = 0; i < 20; i++) {
					rng = rand() % 5;
					WaitForVblank();
					setbg2novb((u16*)bg_static_1Bitmap, (u16*)bg_static_1Palette);
					WaitForVblank();
					setbg2novb((u16*)bg_static_2Bitmap, (u16*)bg_static_2Palette);
					WaitForVblank();
					setbg2novb((u16*)bg_static_3Bitmap, (u16*)bg_static_3Palette);
				}
				REG_SOUNDCNT1_H = 0;                                                       //REG_SOUNDCNT_H = 0000 1011 0000 0100, volume = 100, sound goes to the left, sound goes to the right, timer 0 is used, FIFO buffer reset
				REG_SOUNDCNT1_X = 0;                                                       //REG_SOUNDCNT_X = 0000 0000 1000 0000, enable the sound system, DMA 1
				REG_SD1SAD = 0;                                //REG_DM1SAD = NAME, address of DMA source is the digitized music sample
				REG_SD1DAD = 0;                                                   //REG_DM1DAD = REG_SGFIFOA, address of DMA destination is FIFO buffer for direct sound A
				REG_SD1CNT_H = 0;                                                       //REG_DM1CNT_H = 1011 0110 0100 0000, DMA destination is fixed, repeat transfer of 4 bytes when FIFO , buffer is empty, enable DMA 1 (number of DMA transfers is ignored), INTERRUPT
				REG_TM0SD = 0;                          //REG_TM0D = 65536-(16777216/frequency);, play sample every 16777216/frequency CPU cycles
				REG_TMSDCNT = 0;
				WaitForVblank();
                WaitForVblank();
                setbg2novb((u16*)bg_foundBitmap, (u16*)bg_foundPalette);
                Sleep(90);
				playSound(7);
                for (i = 0; i < 20; i++) {
                    rng = rand() % 5;
					WaitForVblank();
					setbg2novb((u16*)bg_static_1Bitmap, (u16*)bg_static_1Palette);
					WaitForVblank();
					setbg2novb((u16*)bg_static_2Bitmap, (u16*)bg_static_2Palette);
					WaitForVblank();
					setbg2novb((u16*)bg_static_3Bitmap, (u16*)bg_static_3Palette);
                }
				REG_SOUNDCNT1_H = 0;                                                       //REG_SOUNDCNT_H = 0000 1011 0000 0100, volume = 100, sound goes to the left, sound goes to the right, timer 0 is used, FIFO buffer reset
				REG_SOUNDCNT1_X = 0;                                                       //REG_SOUNDCNT_X = 0000 0000 1000 0000, enable the sound system, DMA 1
				REG_SD1SAD = 0;                                //REG_DM1SAD = NAME, address of DMA source is the digitized music sample
				REG_SD1DAD = 0;                                                   //REG_DM1DAD = REG_SGFIFOA, address of DMA destination is FIFO buffer for direct sound A
				REG_SD1CNT_H = 0;                                                       //REG_DM1CNT_H = 1011 0110 0100 0000, DMA destination is fixed, repeat transfer of 4 bytes when FIFO , buffer is empty, enable DMA 1 (number of DMA transfers is ignored), INTERRUPT
				REG_TM0SD = 0;                          //REG_TM0D = 65536-(16777216/frequency);, play sample every 16777216/frequency CPU cycles
				REG_TMSDCNT = 0;
				loadSpriteGraphics((void*)s_tauntTiles, 1024);
                for (i = 0; i < 16; i++) {
                    OBJPaletteMem[0x10 + i] = s_tauntPal[i];
                }
                for (x = 0; x < 256; x++) {
                    BGPaletteMem[x] = bg_run_bluudPal[x];
                }
                for (x = 0; x < 32800; x++) {
                    BGTileMem[x] = bg_run_bluudTiles[x];
                }
                for (x = 0; x < 2048; x++) {
                    videoBuffer[x] = bg_run_bluudMap[x];
                }
				offsetOAM = 2048;
				loadSpriteGraphics((void*)lifecnt_kTiles, 512);
				for (i = 0; i < 16; i++) {
					OBJPaletteMem[0x20 + i] = lifecnt_kPal[i];
				}
				offsetOAM = 0;
                bgx = 0;
                MoveSprite(&sprites[1], 96, 59);
                REG_BG2CNT = CHAR_BASE(1) | BG_COLOR_256 | BG_MOSAIC_ENABLE;
                SetMode(MODE_0 | BG2_ENABLE | OBJ_MAP_1D | OBJ_ENABLE);
                sprites[0].attribute1 = SIZE_64 | 64;
                run_enablesonic = 1;
                run_found = 1;
				playSound(10);
				frames = 0;
            }
            offsetOAM = 0;
            g_newframe = 1;
        }

        CopyOAM();
        g_newframe = 0;
        if (loopbreak == 1) {
            break;
        }
        if (run_sonictimer > 900) {
            loopbreak = 1;
        }
    }
    loopbreak = 0;
    sprites[1].attribute1 = SIZE_64 | 59;
    MoveSprite(&sprites[0], 98, 64);
    MoveSprite(&sprites[1], 84, 64);
    CopyOAM();
    frames = 0;
    offsetOAM = 0;
	REG_SOUNDCNT1_H = 0;                                                       //REG_SOUNDCNT_H = 0000 1011 0000 0100, volume = 100, sound goes to the left, sound goes to the right, timer 0 is used, FIFO buffer reset
	REG_SOUNDCNT1_X = 0;                                                       //REG_SOUNDCNT_X = 0000 0000 1000 0000, enable the sound system, DMA 1
	REG_SD1SAD = 0;                                //REG_DM1SAD = NAME, address of DMA source is the digitized music sample
	REG_SD1DAD = 0;                                                   //REG_DM1DAD = REG_SGFIFOA, address of DMA destination is FIFO buffer for direct sound A
	REG_SD1CNT_H = 0;                                                       //REG_DM1CNT_H = 1011 0110 0100 0000, DMA destination is fixed, repeat transfer of 4 bytes when FIFO , buffer is empty, enable DMA 1 (number of DMA transfers is ignored), INTERRUPT
	REG_TM0SD = 0;                          //REG_TM0D = 65536-(16777216/frequency);, play sample every 16777216/frequency CPU cycles
	REG_TMSDCNT = 0;
    for (i = 0; i < 20; i++) {
        Sleep(10);
        loadSpriteGraphics((void*)k_crai_1Tiles, 1024);
        loadSmallSpritePal((void*)k_crai_1Pal);
        offsetOAM = 0;
        Sleep(10);
        loadSpriteGraphics((void*)k_crai_2Tiles, 1024);
        loadSmallSpritePal((void*)k_crai_2Pal);
        offsetOAM = 0;
    }
    CopyOAM();
    offsetOAM = 1024;
    loadSpriteGraphics((void*)s_pwnTiles, 1024);
    for (i = 0; i < 16; i++) {
        OBJPaletteMem[0x10 + i] = s_pwnPal[i];
    }
    offsetOAM = 0;
    sprites[0].attribute1 = SIZE_64 | HORIZONTAL_FLIP | 98;
    CopyOAM();
    loadSpriteGraphics((void*)k_dedTiles, 1024);
    loadSmallSpritePal((void*)k_dedPal);
    Sleep(10);
    playSound(2);
    memcpy(BGPaletteMem, 0x02000000, 256);
    SetMode(MODE_4 | BG2_ENABLE);
    FadeOut(0);
    Sleep(240);
    REG_SOUNDCNT1_H = 0;                                                       //REG_SOUNDCNT_H = 0000 1011 0000 0100, volume = 100, sound goes to the left, sound goes to the right, timer 0 is used, FIFO buffer reset
    REG_SOUNDCNT1_X = 0;                                                       //REG_SOUNDCNT_X = 0000 0000 1000 0000, enable the sound system, DMA 1
    REG_SD1SAD = 0;                                //REG_DM1SAD = NAME, address of DMA source is the digitized music sample
    REG_SD1DAD = 0;                                                   //REG_DM1DAD = REG_SGFIFOA, address of DMA destination is FIFO buffer for direct sound A
    REG_SD1CNT_H = 0;                                                       //REG_DM1CNT_H = 1011 0110 0100 0000, DMA destination is fixed, repeat transfer of 4 bytes when FIFO , buffer is empty, enable DMA 1 (number of DMA transfers is ignored), INTERRUPT
    REG_TM0SD = 0;                          //REG_TM0D = 65536-(16777216/frequency);, play sample every 16777216/frequency CPU cycles
    REG_TMSDCNT = 0;
    FadeOut(0);
    setbg2((u16*)bg_soulsBitmap, (u16*)bg_soulsPalette);
    playSound(1);
    FadeIn(2);
    Sleep(85);
    REG_SOUNDCNT1_H = 0;                                                       //REG_SOUNDCNT_H = 0000 1011 0000 0100, volume = 100, sound goes to the left, sound goes to the right, timer 0 is used, FIFO buffer reset
    REG_SOUNDCNT1_X = 0;                                                       //REG_SOUNDCNT_X = 0000 0000 1000 0000, enable the sound system, DMA 1
    REG_SD1SAD = 0;                                //REG_DM1SAD = NAME, address of DMA source is the digitized music sample
    REG_SD1DAD = 0;                                                   //REG_DM1DAD = REG_SGFIFOA, address of DMA destination is FIFO buffer for direct sound A
    REG_SD1CNT_H = 0;                                                       //REG_DM1CNT_H = 1011 0110 0100 0000, DMA destination is fixed, repeat transfer of 4 bytes when FIFO , buffer is empty, enable DMA 1 (number of DMA transfers is ignored), INTERRUPT
    REG_TM0SD = 0;                          //REG_TM0D = 65536-(16777216/frequency);, play sample every 16777216/frequency CPU cycles
    REG_TMSDCNT = 0;
    Sleep(555);
    FadeOut(2);
    setbg2((u16*)bg_datasel_rBitmap, (u16*)bg_datasel_rPalette);
    FadeIn(2);
	playSound(3);
	while (!(keyDown(KEY_START))) {
		WaitForVblank();
		frames++;
		if (!(frames % 540)) {
			REG_SOUNDCNT1_H = 0;                                                       //REG_SOUNDCNT_H = 0000 1011 0000 0100, volume = 100, sound goes to the left, sound goes to the right, timer 0 is used, FIFO buffer reset
			REG_SOUNDCNT1_X = 0;                                                       //REG_SOUNDCNT_X = 0000 0000 1000 0000, enable the sound system, DMA 1
			REG_SD1SAD = 0;                                //REG_DM1SAD = NAME, address of DMA source is the digitized music sample
			REG_SD1DAD = 0;                                                   //REG_DM1DAD = REG_SGFIFOA, address of DMA destination is FIFO buffer for direct sound A
			REG_SD1CNT_H = 0;                                                       //REG_DM1CNT_H = 1011 0110 0100 0000, DMA destination is fixed, repeat transfer of 4 bytes when FIFO , buffer is empty, enable DMA 1 (number of DMA transfers is ignored), INTERRUPT
			REG_TM0SD = 0;                          //REG_TM0D = 65536-(16777216/frequency);, play sample every 16777216/frequency CPU cycles
			REG_TMSDCNT = 0;
			playSound(3);
		}
	}
	REG_SOUNDCNT1_H = 0;                                                       //REG_SOUNDCNT_H = 0000 1011 0000 0100, volume = 100, sound goes to the left, sound goes to the right, timer 0 is used, FIFO buffer reset
	REG_SOUNDCNT1_X = 0;                                                       //REG_SOUNDCNT_X = 0000 0000 1000 0000, enable the sound system, DMA 1
	REG_SD1SAD = 0;                                //REG_DM1SAD = NAME, address of DMA source is the digitized music sample
	REG_SD1DAD = 0;                                                   //REG_DM1DAD = REG_SGFIFOA, address of DMA destination is FIFO buffer for direct sound A
	REG_SD1CNT_H = 0;                                                       //REG_DM1CNT_H = 1011 0110 0100 0000, DMA destination is fixed, repeat transfer of 4 bytes when FIFO , buffer is empty, enable DMA 1 (number of DMA transfers is ignored), INTERRUPT
	REG_TM0SD = 0;                          //REG_TM0D = 65536-(16777216/frequency);, play sample every 16777216/frequency CPU cycles
	REG_TMSDCNT = 0;
    Sleep(240);
    playSound(1);
    Sleep(120);
    REG_SOUNDCNT1_H = 0;                                                       //REG_SOUNDCNT_H = 0000 1011 0000 0100, volume = 100, sound goes to the left, sound goes to the right, timer 0 is used, FIFO buffer reset
    REG_SOUNDCNT1_X = 0;                                                       //REG_SOUNDCNT_X = 0000 0000 1000 0000, enable the sound system, DMA 1
    REG_SD1SAD = 0;                                //REG_DM1SAD = NAME, address of DMA source is the digitized music sample
    REG_SD1DAD = 0;                                                   //REG_DM1DAD = REG_SGFIFOA, address of DMA destination is FIFO buffer for direct sound A
    REG_SD1CNT_H = 0;                                                       //REG_DM1CNT_H = 1011 0110 0100 0000, DMA destination is fixed, repeat transfer of 4 bytes when FIFO , buffer is empty, enable DMA 1 (number of DMA transfers is ignored), INTERRUPT
    REG_TM0SD = 0;                          //REG_TM0D = 65536-(16777216/frequency);, play sample every 16777216/frequency CPU cycles
    REG_TMSDCNT = 0;
    SetMode(0);
    sprites[0].attribute0 = COLOR_16 | SQUARE | 0;
    sprites[0].attribute1 = SIZE_64 | 0;
    sprites[0].attribute2 = 512 + 0 | PALETTE(0); // NOTE: mode4 doesn't support the first tiles, so offset of 512 is requirerd
    sprites[1].attribute0 = COLOR_16 | SQUARE | 0;
    sprites[1].attribute1 = SIZE_64 | 0;
    sprites[1].attribute2 = 576 | PALETTE(1); // NOTE: mode4 doesn't support the first tiles, so offset of 512 is requirerd
    MoveSprite(&sprites[1], 240, 95);
    offsetOAM = 0;
    Sleep(60);
    FadeOut(0);
    SetMode(MODE_4 | BG2_ENABLE);
    setbg2((void*)bg_3dot_startBitmap, (void*)bg_3dot_startPal);
    FadeIn(2);
    Sleep(60);
    FadeOut(2);
    MoveSprite(&sprites[0], 88, 57);
    CopyOAM();
    Sleep(1);
    for (x = 0; x < 256; x++) {
        BGPaletteMem[x] = bg_3dotPal[x];
    }
    for (x = 0; x < 32800; x++) {
        BGTileMem[x] = bg_3dotTiles[x];
    }
    for (x = 0; x < 2048; x++) {
        videoBuffer[x] = bg_3dotMap[x];
    }
    loadSpriteGraphics((void*)r_idleTiles, 1024);
    loadSpriteGraphics((void*)s_crouchTiles, 1024);
    loadSmallSpritePal((void*)r_idlePal);
    for (i = 0; i < 16; i++) {
        OBJPaletteMem[0x10 + i] = s_crouchPal[i];
    }
    loadSpriteGraphics((void*)lifecnt_rTiles, 512);
    for (i = 0; i < 16; i++) {
        OBJPaletteMem[0x20 + i] = lifecnt_rPal[i];
    }
    REG_BG2CNT = CHAR_BASE(1) | BG_COLOR_256 | BG_MOSAIC_ENABLE;
    SetMode(MODE_0 | BG2_ENABLE | OBJ_MAP_1D | OBJ_ENABLE);
    frames = 0;
    g_newframe = 0;
    FadeIn(2);
    CopyOAM();
    loopbreak = 0;
	playSound(12);
    while (1) {
        if (g_newframe == 0) {
			frames++;
			if (!(frames % 2580)) {
				REG_SOUNDCNT1_H = 0;                                                       //REG_SOUNDCNT_H = 0000 1011 0000 0100, volume = 100, sound goes to the left, sound goes to the right, timer 0 is used, FIFO buffer reset
				REG_SOUNDCNT1_X = 0;                                                       //REG_SOUNDCNT_X = 0000 0000 1000 0000, enable the sound system, DMA 1
				REG_SD1SAD = 0;                                //REG_DM1SAD = NAME, address of DMA source is the digitized music sample
				REG_SD1DAD = 0;                                                   //REG_DM1DAD = REG_SGFIFOA, address of DMA destination is FIFO buffer for direct sound A
				REG_SD1CNT_H = 0;                                                       //REG_DM1CNT_H = 1011 0110 0100 0000, DMA destination is fixed, repeat transfer of 4 bytes when FIFO , buffer is empty, enable DMA 1 (number of DMA transfers is ignored), INTERRUPT
				REG_TM0SD = 0;                          //REG_TM0D = 65536-(16777216/frequency);, play sample every 16777216/frequency CPU cycles
				REG_TMSDCNT = 0;
				playSound(12);
			}
            rng = rand() % 5;
            UpdateBG(&bg3);
            frames++;
            if (keyDown(KEY_LEFT)) {
                bgx -= 3;
                bg3.x -= 3;
            }
            if (keyDown(KEY_RIGHT)) {
                bgx += 3;
                bg3.x += 3;
            }
            if (keyDown(KEY_LEFT)) {
                sprites[0].attribute1 = SIZE_64 | HORIZONTAL_FLIP | 88;
                if (animframe == 1) {
                    loadSpriteGraphics((void*)r_run_1Tiles, 1024);
                    loadSmallSpritePal((void*)r_run_1Pal);
                }
                if (animframe == 2) {
                    loadSpriteGraphics((void*)r_run_2Tiles, 1024);
                    loadSmallSpritePal((void*)r_run_2Pal);
                }
                if (animframe == 3) {
                    loadSpriteGraphics((void*)r_run_3Tiles, 1024);
                    loadSmallSpritePal((void*)r_run_3Pal);
                }
                if (animframe == 4) {
                    loadSpriteGraphics((void*)r_run_4Tiles, 1024);
                    loadSmallSpritePal((void*)r_run_4Pal);
                }
                if (!(frames % 6)) {
                    animframe++;
                    if (animframe == 5) {
                        animframe = 1;
                    }
                }
                pressinglr = 1;
                resetOffset();
            }
            if (keyDown(KEY_RIGHT)) {
                sprites[0].attribute1 = SIZE_64 | 88;
                if (animframe == 1) {
                    loadSpriteGraphics((void*)r_run_1Tiles, 1024);
                    loadSmallSpritePal((void*)r_run_1Pal);
                }
                if (animframe == 2) {
                    loadSpriteGraphics((void*)r_run_2Tiles, 1024);
                    loadSmallSpritePal((void*)r_run_2Pal);
                }
                if (animframe == 3) {
                    loadSpriteGraphics((void*)r_run_3Tiles, 1024);
                    loadSmallSpritePal((void*)r_run_3Pal);
                }
                if (animframe == 4) {
                    loadSpriteGraphics((void*)r_run_4Tiles, 1024);
                    loadSmallSpritePal((void*)r_run_4Pal);
                }
                if (!(frames % 6)) {
                    animframe++;
                    if (animframe == 5) {
                        animframe = 1;
                    }
                }
                pressinglr = 1;
                resetOffset();
            }
            if (!((keyDown(KEY_RIGHT))OR(keyDown(KEY_LEFT)))) {
                animframe = 1;
                if (pressinglr == 1) {
                    loadSpriteGraphics((void*)r_idleTiles, 1024);
                    loadSmallSpritePal((void*)r_idlePal);
                    pressinglr = 0;
                }
            }
            if (bgx > 4096) {
                offsetOAM = 0;
                sprites[1].attribute1 = SIZE_64 | HORIZONTAL_FLIP | 0;
                MoveSprite(&sprites[1], 96, 59);
                CopyOAM();
                loadSpriteGraphics((void*)r_idleTiles, 1024);
                loadSmallSpritePal((void*)r_idlePal);
                Sleep(30);
                loopbreak = 1;
            }
            resetOffset();
            g_newframe = 1;
        }
        while (REG_VCOUNT < 160);
        CopyOAM();
        g_newframe = 0;
        if (loopbreak == 1) {
            break;
        }
    }
    SetMode(MODE_4 | BG2_ENABLE);
	longstatic();
	longstatic();
	longstatic();
	longstatic();
	longstatic();
	longstatic();
	longstatic();
	REG_SOUNDCNT1_H = 0;                                                       //REG_SOUNDCNT_H = 0000 1011 0000 0100, volume = 100, sound goes to the left, sound goes to the right, timer 0 is used, FIFO buffer reset
	REG_SOUNDCNT1_X = 0;                                                       //REG_SOUNDCNT_X = 0000 0000 1000 0000, enable the sound system, DMA 1
	REG_SD1SAD = 0;                                //REG_DM1SAD = NAME, address of DMA source is the digitized music sample
	REG_SD1DAD = 0;                                                   //REG_DM1DAD = REG_SGFIFOA, address of DMA destination is FIFO buffer for direct sound A
	REG_SD1CNT_H = 0;                                                       //REG_DM1CNT_H = 1011 0110 0100 0000, DMA destination is fixed, repeat transfer of 4 bytes when FIFO , buffer is empty, enable DMA 1 (number of DMA transfers is ignored), INTERRUPT
	REG_TM0SD = 0;                          //REG_TM0D = 65536-(16777216/frequency);, play sample every 16777216/frequency CPU cycles
	REG_TMSDCNT = 0;
    SetMode(MODE_3 | BG2_ENABLE);
    memcpy(videoBuffer, bg_megaspoopBitmap, 76800);
    Sleep(1800);
    SetMode(MODE_4 | BG2_ENABLE);
	longstatic();
	longstatic();
	longstatic();
	longstatic();
	REG_SOUNDCNT1_H = 0;                                                       //REG_SOUNDCNT_H = 0000 1011 0000 0100, volume = 100, sound goes to the left, sound goes to the right, timer 0 is used, FIFO buffer reset
	REG_SOUNDCNT1_X = 0;                                                       //REG_SOUNDCNT_X = 0000 0000 1000 0000, enable the sound system, DMA 1
	REG_SD1SAD = 0;                                //REG_DM1SAD = NAME, address of DMA source is the digitized music sample
	REG_SD1DAD = 0;                                                   //REG_DM1DAD = REG_SGFIFOA, address of DMA destination is FIFO buffer for direct sound A
	REG_SD1CNT_H = 0;                                                       //REG_DM1CNT_H = 1011 0110 0100 0000, DMA destination is fixed, repeat transfer of 4 bytes when FIFO , buffer is empty, enable DMA 1 (number of DMA transfers is ignored), INTERRUPT
	REG_TM0SD = 0;                          //REG_TM0D = 65536-(16777216/frequency);, play sample every 16777216/frequency CPU cycles
	REG_TMSDCNT = 0;
    SetMode(MODE_3 | BG2_ENABLE);
    memcpy(videoBuffer, bg_megaspoopBitmap, 76800);
    Sleep(420);
    SetMode(MODE_4 | BG2_ENABLE);
	longstatic();
	longstatic();
	longstatic();
	longstatic();
	longstatic();
    memcpy(BGPaletteMem, 0x02000000, 256);
	REG_SOUNDCNT1_H = 0;                                                       //REG_SOUNDCNT_H = 0000 1011 0000 0100, volume = 100, sound goes to the left, sound goes to the right, timer 0 is used, FIFO buffer reset
	REG_SOUNDCNT1_X = 0;                                                       //REG_SOUNDCNT_X = 0000 0000 1000 0000, enable the sound system, DMA 1
	REG_SD1SAD = 0;                                //REG_DM1SAD = NAME, address of DMA source is the digitized music sample
	REG_SD1DAD = 0;                                                   //REG_DM1DAD = REG_SGFIFOA, address of DMA destination is FIFO buffer for direct sound A
	REG_SD1CNT_H = 0;                                                       //REG_DM1CNT_H = 1011 0110 0100 0000, DMA destination is fixed, repeat transfer of 4 bytes when FIFO , buffer is empty, enable DMA 1 (number of DMA transfers is ignored), INTERRUPT
	REG_TM0SD = 0;                          //REG_TM0D = 65536-(16777216/frequency);, play sample every 16777216/frequency CPU cycles
	REG_TMSDCNT = 0;
    Sleep(240);
    setbg2((u16*)bg_roundiiBitmap, (u16*)bg_roundiiPal);
    Sleep(240);
    SetMode(MODE_3 | BG2_ENABLE);
    gg_timer = 0;
	frames = 0;
	playSound(8);
    while (!(gg_timer == 1536)) {
        WaitForVblank();
		if (!(frames % 280)) {
			REG_SOUNDCNT1_H = 0;                                                       //REG_SOUNDCNT_H = 0000 1011 0000 0100, volume = 100, sound goes to the left, sound goes to the right, timer 0 is used, FIFO buffer reset
			REG_SOUNDCNT1_X = 0;                                                       //REG_SOUNDCNT_X = 0000 0000 1000 0000, enable the sound system, DMA 1
			REG_SD1SAD = 0;                                //REG_DM1SAD = NAME, address of DMA source is the digitized music sample
			REG_SD1DAD = 0;                                                   //REG_DM1DAD = REG_SGFIFOA, address of DMA destination is FIFO buffer for direct sound A
			REG_SD1CNT_H = 0;                                                       //REG_DM1CNT_H = 1011 0110 0100 0000, DMA destination is fixed, repeat transfer of 4 bytes when FIFO , buffer is empty, enable DMA 1 (number of DMA transfers is ignored), INTERRUPT
			REG_TM0SD = 0;                          //REG_TM0D = 65536-(16777216/frequency);, play sample every 16777216/frequency CPU cycles
			REG_TMSDCNT = 0;
			playSound(8);
		}
        if (!(KEY_ANY_PRESSED)) {
            memcpy(videoBuffer, bg_ggBitmap, 76800);
        } else {
            memcpy(videoBuffer, bg_gg_spookBitmap, 76800);
        }
        gg_timer++;
		frames++;
    }
    SetMode(MODE_4 | BG2_ENABLE);
	longstatic();
	longstatic();
	longstatic();
	longstatic();
	longstatic();
	REG_SOUNDCNT1_X = 0;                                                       //REG_SOUNDCNT_X = 0000 0000 1000 0000, enable the sound system, DMA 1
	REG_SD1SAD = 0;                                //REG_DM1SAD = NAME, address of DMA source is the digitized music sample
	REG_SD1DAD = 0;                                                   //REG_DM1DAD = REG_SGFIFOA, address of DMA destination is FIFO buffer for direct sound A
	REG_SD1CNT_H = 0;                                                       //REG_DM1CNT_H = 1011 0110 0100 0000, DMA destination is fixed, repeat transfer of 4 bytes when FIFO , buffer is empty, enable DMA 1 (number of DMA transfers is ignored), INTERRUPT
	REG_TM0SD = 0;                          //REG_TM0D = 65536-(16777216/frequency);, play sample every 16777216/frequency CPU cycles
	REG_TMSDCNT = 0;
    setbg2((u16*)bg_jpnBitmap, (u16*)bg_jpnPal);
    Sleep(512);
	REG_SOUNDCNT1_H = 0;                                                       //REG_SOUNDCNT_H = 0000 1011 0000 0100, volume = 100, sound goes to the left, sound goes to the right, timer 0 is used, FIFO buffer reset
	REG_SOUNDCNT1_X = 0;                                                       //REG_SOUNDCNT_X = 0000 0000 1000 0000, enable the sound system, DMA 1
	REG_SD1SAD = 0;                                //REG_DM1SAD = NAME, address of DMA source is the digitized music sample
	REG_SD1DAD = 0;                                                   //REG_DM1DAD = REG_SGFIFOA, address of DMA destination is FIFO buffer for direct sound A
	REG_SD1CNT_H = 0;                                                       //REG_DM1CNT_H = 1011 0110 0100 0000, DMA destination is fixed, repeat transfer of 4 bytes when FIFO , buffer is empty, enable DMA 1 (number of DMA transfers is ignored), INTERRUPT
	REG_TM0SD = 0;                          //REG_TM0D = 65536-(16777216/frequency);, play sample every 16777216/frequency CPU cycles
	REG_TMSDCNT = 0;
	frames = 0;
	playSound(11);
    while (1) {
		frames++;
		WaitForVblank();
		if (!(frames % 960)) {
			REG_SOUNDCNT1_H = 0;                                                       //REG_SOUNDCNT_H = 0000 1011 0000 0100, volume = 100, sound goes to the left, sound goes to the right, timer 0 is used, FIFO buffer reset
			REG_SOUNDCNT1_X = 0;                                                       //REG_SOUNDCNT_X = 0000 0000 1000 0000, enable the sound system, DMA 1
			REG_SD1SAD = 0;                                //REG_DM1SAD = NAME, address of DMA source is the digitized music sample
			REG_SD1DAD = 0;                                                   //REG_DM1DAD = REG_SGFIFOA, address of DMA destination is FIFO buffer for direct sound A
			REG_SD1CNT_H = 0;                                                       //REG_DM1CNT_H = 1011 0110 0100 0000, DMA destination is fixed, repeat transfer of 4 bytes when FIFO , buffer is empty, enable DMA 1 (number of DMA transfers is ignored), INTERRUPT
			REG_TM0SD = 0;                          //REG_TM0D = 65536-(16777216/frequency);, play sample every 16777216/frequency CPU cycles
			REG_TMSDCNT = 0;
			playSound(11);
		}
        if (keyDown(KEY_START)) {
            asm volatile("swi 0x26"::);
        }
    }
    return 0;
}
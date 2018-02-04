#include <string.h>
#include <agb_lib.h>
#include "../inc/defs.h"
#include "../inc/gbfs.h"
#include "../inc/lz77.h"

void loadSmallSpritePal(u16* data)
{
	int 	x;

	for (x = 0; x < 16; x++)
		OBJPaletteMem[x] = ((unsigned short*)data)[x];
}

void vid_vsync()
{
	while (REG_VCOUNT >= 160);   // wait till VDraw
	while (REG_VCOUNT < 160);    // wait till VBlank
}

void vblfunc()
{
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
	u8 datasel_paltoggle = 0;
	bgx = 0;
    u8 t_idleframe = 0;
    int i;
    int x;
    bg3.x = 0;
    bg3.y = 0;
    bg3.n = 2;
    SetMode(MODE_4 | BG2_ENABLE);
    for (i = 0; i < 20; i++) {
        setbg2((u16*)title_frame1Bitmap, (u16*)title_frame1Palette);
        Sleep(7);
        setbg2((u16*)title_frame2Bitmap, (u16*)title_frame2Palette);
        Sleep(7);
    }
    SetMode(MODE_3 | BG2_ENABLE);
	memcpy(BGPaletteMem, 0x02000000, 256);
    memcpy(videoBuffer, title_demonicBitmap, 76800);
    Sleep(5);
	SetMode(MODE_4 | BG2_ENABLE);
    Sleep(120);
    setbg2((u16*)bg_datasel_tBitmap, (u16*)bg_datasel_tPalette);
    SetMode(MODE_4 | BG2_ENABLE);
	while (!(keyDown(KEY_START)))
	{
		WaitForVblank();
		frames++;
		if (!(frames % 5))
		{
			datasel_paltoggle++;
				if (datasel_paltoggle == 0)
				{
					SetBGPalPoint(3, (u16*)0x001F);
				}
				if (datasel_paltoggle == 1)
				{
					SetBGPalPoint(3, (u16*)0x03FF);
				}
				if (datasel_paltoggle == 2)
				{
					datasel_paltoggle = 0;
				}
		}
	}
	srand(frames);
    Sleep(300);
    SetMode(0);
	sprites[0].attribute0 = COLOR_16 | SQUARE | 0;
	sprites[0].attribute1 = SIZE_64 | 0;
	sprites[0].attribute2 = 512 + 0 | PALETTE(0); // NOTE: mode4 doesn't support the first tiles, so offset of 512 is requirerd
	sprites[1].attribute0 = COLOR_16 | TALL | 0 ;
	sprites[1].attribute1 = SIZE_64 | 0;
	sprites[1].attribute2 = 576 | PALETTE(1); // NOTE: mode4 doesn't support the first tiles, so offset of 512 is requirerd
	MoveSprite(&sprites[1], 240, 91);
    loadSpriteGraphics((void*)t_idle_1Tiles, 1024);
	loadSpriteGraphics((void*)s_sleep_1Tiles, 512);
    loadSmallSpritePal((void*)t_idle_1Pal);
	for (i = 0; i < 16; i++)
	{
		OBJPaletteMem[0x10 + i] = s_sleep_1Pal[i];
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
	while (1) {
		if (g_newframe == 0)
		{
			rng = rand() % 5;
			UpdateBG(&bg3);
			frames++;
			if (keyDown(KEY_LEFT)) {
				bgx -= 3;
				bg3.x -= 3;
				
				if (hill_enablesonic == 1)
				{
					hill_sonix -= 3;
					MoveSprite(&sprites[1], 240 - hill_sonix, 89);
				}
			}
			if (keyDown(KEY_RIGHT)) {
				bgx += 3;
				bg3.x += 3;
				if (hill_enablesonic == 1)
				{
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
			if (bgx > 4096)
			{
				if (hill_enablesonic == 0)
				{
					hill_enablesonic = 1;
				}
			}
			if (bgx > 4123)
			{
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
				for(i=0;i<85;i++)
				{
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
				for (i = 0; i < 16; i++)
				{
					OBJPaletteMem[0x10 + i] = s_sleep_2Pal[i];
				}
				Sleep(15);
				loopbreak = 1;
				break;
			}
			if (bgx > 2048)
			{
				if (hill_statictrigger == 0)
				{
					WaitForVblank();
					SetMode(MODE_4 | BG2_ENABLE);
					for (i = 0; i < 75; i++)
					{
						rng = rand() % 5;
						setbg2novb((u16*)bg_static_1Bitmap, (u16*)bg_static_1Palette);
						setbg2novb((u16*)bg_static_2Bitmap, (u16*)bg_static_2Palette);
						setbg2novb((u16*)bg_static_3Bitmap, (u16*)bg_static_3Palette);
					}
					WaitForVblank();
					memcpy(BGPaletteMem, bg_hill_bluudyPal, 512);
					memcpy(BGTileMem, bg_hill_bluudyTiles, 65600);
					memcpy(videoBuffer, bg_hill_bluudyMap, 2048);
					REG_BG2CNT = CHAR_BASE(1) | BG_COLOR_256 | BG_MOSAIC_ENABLE;
					SetMode(MODE_0 | BG2_ENABLE | OBJ_MAP_1D | OBJ_ENABLE);
					hill_statictrigger = 1;
				}
			}
			g_newframe = 1;
		}
		while (REG_VCOUNT < 160);
		CopyOAM();
		g_newframe = 0;
		if (loopbreak == 1)
		{
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
	FadeIn(3);
	Sleep(420);
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
	loadSpriteGraphics((void*)t_spooked_1Tiles, 1024);
	loadSpriteGraphics((void*)s_spookTiles, 1024);
	loadSmallSpritePal((void*)t_spooked_1Pal);
	for (i = 0; i < 16; i++)
	{
		OBJPaletteMem[0x10 + i] = s_spookPal[i];
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
	while (1)
	{
		if (g_newframe == 0)
		{
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
		}
		while (REG_VCOUNT < 160);
		CopyOAM();
		g_newframe = 0;
		if (loopbreak == 1)
		{
			break;
		}
		hide_spookpos = 0;
		if (bgx > 1024)
		{
			if (!(hide_2spooky4metrig == 1))
			{
				frames = 0;
				offsetOAM = 0;
				loadSpriteGraphics((void*)t_spooked_1Tiles, 1024);
				loadSmallSpritePal((void*)t_spooked_1Pal);
				for (i = 0; i < 100; i++)
				{
					Sleep(2);
					frames++;
					if (!(frames % 5))
					{
						rng = (rand() % 5) - 1;
						if (rng == 0)
						{
							MoveSprite(&sprites[1], 2, 2);
						}
						if (rng == 1)
						{
							MoveSprite(&sprites[1], 203, 2);
						}
						if (rng == 2)
						{
							MoveSprite(&sprites[1], 2, 119);
						}
						if (rng == 3)
						{
							MoveSprite(&sprites[1], 203, 119);
						}
						if (rng == 4)
						{
							MoveSprite(&sprites[1], 103, 61);
						}
						CopyOAM();
					}
				}
				hide_2spooky4metrig = 1;
				hide_enablesonic = 1;
				offsetOAM = 1024;
				loadSpriteGraphics((void*)s_gattagofastTiles, 1024);
				for (i = 0; i < 16; i++)
				{
					OBJPaletteMem[0x10 + i] = s_gattagofastPal[i];
				}
				MoveSprite(&sprites[1], 0, 106);
				CopyOAM();
				while (!(hide_sonix > 103))
				{
					hide_sonix += 2;
					MoveSprite(&sprites[1], hide_sonix, 106);
					CopyOAM();
					WaitForVblank();
				}
				MoveSprite(&sprites[1], 240, 160);
				CopyOAM();
				offsetOAM = 0;
				WaitForVblank();
				loadSpriteGraphics((void*)t_crai_1Tiles, 1024);
				loadSpriteGraphics((void*)s_crouchTiles, 1024);
				loadSmallSpritePal((void*)t_crai_1Pal);
				for (i = 0; i < 16; i++)
				{
					OBJPaletteMem[0x10 + i] = s_crouchPal[i];
				}
				offsetOAM = 0;
				for (i = 0; i < 100; i++)
				{
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
				for (i = 0; i < 16; i++)
				{
					OBJPaletteMem[0x10 + i] = s_pwnPal[i];
				}
				offsetOAM = 0;
				MoveSprite(&sprites[0], 88, 100);
				CopyOAM();
				loadSpriteGraphics((void*)t_dedTiles, 1024);
				loadSmallSpritePal((void*)t_dedPal);
				Sleep(5);
				memcpy(BGPaletteMem, 0x02000000, 256);
				SetMode(MODE_4 | BG2_ENABLE);
				FadeOut(0);
				Sleep(240);
				setbg2((u16*)bg_slowBitmap, (u16*)bg_slowPalette);
				FadeIn(2);
				Sleep(420);
				FadeOut(2);
				setbg2((u16*)bg_datasel_kBitmap, (u16*)bg_datasel_kPalette);
				FadeIn(2);
				while (!(keyDown(KEY_START)));
				Sleep(300);
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
				offsetOAM = 0;
				loadSpriteGraphics((void*)k_idleTiles, 1024);
				loadSpriteGraphics((void*)s_crouchTiles, 1024);
				loadSmallSpritePal((void*)k_idlePal);
				for (i = 0; i < 16; i++)
				{
					OBJPaletteMem[0x10 + i] = s_crouchPal[i];
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
	while (1)
	{
		if (g_newframe == 0)
		{
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
			if (bgx > 1536)
			{
				if (run_statictrigger == 0)
				{
					WaitForVblank();
					SetMode(MODE_4 | BG2_ENABLE);
					for (i = 0; i < 75; i++)
					{
						rng = rand() % 5;
						setbg2novb((u16*)bg_static_1Bitmap, (u16*)bg_static_1Palette);
						setbg2novb((u16*)bg_static_2Bitmap, (u16*)bg_static_2Palette);
						setbg2novb((u16*)bg_static_3Bitmap, (u16*)bg_static_3Palette);
					}
					WaitForVblank();
					memcpy(BGPaletteMem, bg_run_bluudPal, 512);
					memcpy(BGTileMem, bg_run_bluudTiles, 65600);
					memcpy(videoBuffer, bg_run_bluudMap, 2048);
					REG_BG2CNT = CHAR_BASE(1) | BG_COLOR_256 | BG_MOSAIC_ENABLE;
					SetMode(MODE_0 | BG2_ENABLE | OBJ_MAP_1D | OBJ_ENABLE);
					run_statictrigger = 1;
				}
			}
			offsetOAM = 0;
			g_newframe = 1;
			if (keyDown(KEY_START))
			{
				asm volatile("swi 0x26"::);
			}
		}
		while (REG_VCOUNT < 160);
		CopyOAM();
		g_newframe = 0;
		if (loopbreak == 1)
		{
			break;
		}
	}
    return 0;
}
SOURCES = src/main.c src/bg_3dot.c src/gbfs.c src/lz77.c data/title_demonic.c data/bg_disclaimer.c data/title_frame1.c data/title_frame1.c data/bg_static_1.c data/bg_static_2.c data/bg_static_3.c data/bg_datasel_r.c data/bg_dataselt.c data/bg_hill.c data/t_crai_1.c data/t_crai_2.c data/t_ded.c data/t_hey.c data/t_idle_1.c data/t_idle_2.c data/t_idle_3.c data/t_idle_4.c data/t_point.c data/t_run_1.c data/t_run_2.c data/t_run_3.c data/t_run_4.c data/t_run_5.c data/t_run_6.c data/t_run_7.c data/t_run_8.c data/t_spooked_1.c data/t_spooked_2.c data/t_surp.c data/bg_hill_bluudy.c data/s_sleep_1.c data/bg_hill_start.c data/s_sleep_2.c data/bg_hide_start.c data/bg_play.c data/bg_hide.c data/s_spook_1.c data/s_gattagofast.c data/s_crouch.c data/s_pwn.c data/bg_datasel_k.c data/bg_slow.c data/bg_run_start.c data/k_ded.c data/k_idle.c  data/k_run_1.c  data/k_run_2.c  data/k_run_3.c  data/k_run_4.c  data/k_run_5.c  data/k_run_6.c  data/k_run_7.c  data/k_run_8.c  data/k_spook.c data/bg_run.c data/bg_run_bluud.c data/k_crai_1.c data/k_crai_2.c data/bg_found.c data/s_taunt.c data/bg_souls.c data/r_idle.c data/r_run_1.c data/r_run_2.c data/r_run_3.c data/r_run_4.c data/bg_3dot_start.c data/bg_megaspoop.c  data/bg_roundii.c data/bg_gg.c data/bg_gg_spook.c data/bg_jpn.c data/s_spook_2. data/s_spook_3.c data/s_spook_4.c data/s_spook_5.c data/lifecnt_r.c data/lifecnt_k.c data/lifecnt_t.c data/snd_laugh.c data/snd_rip.c data/bgm_datasel.c data/bgm_run.c data/bgm_hill.c data/snd_msg.c data/snd_static.c data/bgm_gg.c data/bgm_hide.c data/bgm_prank.c data/bgm_jpn. data/bgm_3dot.c data/bgm_title.c data/bgm_gattagofest.c
INCLUDE = inc/defs.h inc/gbfs.h inc/lz77.h
OBJECTS = build/main.o build/bg_3dot.o build/gbfs.o build/lz77.o build/bg_disclaimer.o build/title_demonic.o build/title_frame1.o build/title_frame2.o  build/bg_datasel_r.o build/bg_datasel_t.o build/bg_hill.o build/t_crai_1.o build/t_crai_2.o build/t_ded.o build/t_hey.o build/t_idle_1.o build/t_idle_2.o build/t_idle_3.o build/t_idle_4.o build/t_point.o build/t_run_1.o build/t_run_2.o build/t_run_3.o build/t_run_4.o build/t_run_5.o build/t_run_6.o build/t_run_7.o build/t_run_8.o build/t_spooked_1.o build/t_spooked_2.o build/t_surp.o build/bg_hill_bluudy.o build/bg_static_1.o build/bg_static_2.o build/bg_static_3.o build/s_sleep_1.o build/bg_hill_start.o build/s_sleep_2.o build/bg_hide_start.o build/bg_play.o build/bg_hide.o build/s_spook_1.o build/s_gattagofast.o build/s_crouch.o build/s_pwn.o build/bg_datasel_k.o build/bg_slow.o build/bg_run_start.o build/k_ded.o build/k_idle.o  build/k_run_1.o  build/k_run_2.o  build/k_run_3.o  build/k_run_4.o  build/k_run_5.o  build/k_run_6.o  build/k_run_7.o  build/k_run_8.o  build/k_spook.o build/bg_run.o build/bg_run_bluud.o build/k_crai_1.o build/k_crai_2.o build/bg_found.o build/s_taunt.o build/bg_souls.o build/r_idle.o build/r_run_1.o build/r_run_2.o build/r_run_3.o build/r_run_4.o build/bg_3dot_start.o build/bg_megaspoop.o build/bg_roundii.o build/bg_gg.o build/bg_gg_spook.o build/bg_jpn.o build/s_spook_2.o build/s_spook_3.o build/s_spook_4.o build/s_spook_5.o build/lifecnt_t.o build/lifecnt_k.o build/lifecnt_r.o build/snd_laugh.o build/snd_rip.o build/bgm_datasel.o build/bgm_run.o build/bgm_hill.o build/snd_msg.o build/snd_static.o build/bgm_gg.o build/bgm_hide.o build/bgm_prank.o build/bgm_jpn.o build/bgm_3dot.o build/bgm_title.o build/bgm_gattagofest.o
CFLAGS = -g -MMD -MP -MF -Wall -Og -O1 -O2 -O3 -O -Os -ffast-math -mlong-calls -mcpu=arm7tdmi -mtune=arm7tdmi -fomit-frame-pointer -marm
PREFIX = C:\devkitPro\devkitARM\bin\arm-none-eabi-
ARCH = -mthumb -mthumb-interwork
SPECS = -specs=gba.specs
default: sonic.gba

build/%.o: src/%.c
	$(PREFIX)gcc $(CFLAGS) $(ARCH) -c $< -o $@
build/%.o: data/%.c
	$(PREFIX)gcc $(CFLAGS) $(ARCH) -c $< -o $@
build/main.elf: $(OBJECTS)
	$(PREFIX)gcc $(SPECS) $(ARCH) $(OBJECTS) -o build/main.elf
sonic.gba: build/main.elf
	-rm -f build/*.h
	$(PREFIX)objcopy -O binary build/main.elf sonic.gba
	C:/devkitPro/devkitARM/bin/gbafix sonic.gba -tSONICEXE_GBA -cIGOD -v1>nul

clean: 
	-rm -g build/*.o build/main.elf
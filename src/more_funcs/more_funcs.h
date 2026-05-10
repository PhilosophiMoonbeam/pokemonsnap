#ifndef _MORE_FUNCS_H_
#define _MORE_FUNCS_H_

#include "types.h"
#include "common_structs.h"

#define MD4_BLOCK_LENGTH 0x200

typedef struct {
    /* 0x00 */ u32 a;         // State variable A
    /* 0x04 */ u32 b;         // State variable B
    /* 0x08 */ u32 c;         // State variable C
    /* 0x0C */ u32 d;         // State variable D
    /* 0x10 */ u8 buffer[8];  // Buffer for bit count
    /* 0x18 */ s32 finalized; // Indicates if the digest is finalized
} MD4Ctx;                     // size = 0x1C

typedef struct SaveFile {
    /* 0x000 */ s32 a; // MD4 variable 1
    /* 0x004 */ s32 b; // MD4 variable 2
    /* 0x008 */ s32 c; // MD4 variable 3
    /* 0x00C */ s32 d; // MD4 variable 4
    struct {
        /* 0x010 */ char unk_10[4];
        /* 0x014 */ u8 pad_14[0xC];
        /* 0x020 */ s32 unk_20;
        /* 0x024 */ s32 unk_24;
        /* 0x028 */ s32 unk_28;
        /* 0x02C */ s32 unk_2C;
        /* 0x030 */ u32 unk_30;
        /* 0x034 */ s32 unk_34[7];
        /* 0x050 */ s32 unk_50;
        /* 0x054 */ PlayerName playerName;
        /* 0x064 */ u32 unk_64_29 : 3;
        /* ----- */ u32 unk_64_22 : 7;
        /* ----- */ u32 unk_64_16 : 6;
        /* 0x066 */ u32 hasApple : 1;
        /* ----- */ u32 hasPesterBall : 1;
        /* ----- */ u32 hasFlute : 1;
        /* ----- */ u32 unk_64_12 : 1;
        /* ----- */ u32 unk_64_11 : 1;
        /* ----- */ u32 hasDashEngine : 1;
        /* ----- */ u32 unk_64_09 : 1;
        /* ----- */ u32 unk_64_08 : 1;
        /* 0x067 */ u32 hasFinishedTutorial : 1;
        /* ----- */ u32 unk_64_06 : 1;
        /* ----- */ u32 unk_64_05 : 1;
        /* ----- */ u32 unk_64_04 : 1;
        /* ----- */ u32 unk_64_03 : 1;
        /* ----- */ u32 zoomSwitch : 1;
        /* ----- */ u32 invertedY : 1;
        /* ----- */ u32 unk_64_00 : 1;
        /* 0x068 */ u32 unk_68_31 : 1;
        /* ----- */ u32 unk_68_30 : 1;
        /* ----- */ u32 unk_68_29 : 1;
        /* ----- */ u32 unk_68_28 : 1;
        /* ----- */ u32 unk_68_27 : 1;
        /* ----- */ u32 unk_68_26 : 27;
        /* 0x06C */ s32 pokemonPhotoScores[69];
        /* 0x180 */ PhotoData pokemonPhotos[69];
        /* 0xFBA0 */ PhotoData reportPhotos[4];
        /* 0x10A20 */ AlbumPhotoData albumPhotos[60];
        /* 0x1f2a0 */ u8 pad_1F2A0[4];
    } data;
} SaveFile; // size = 0x1F2A4

typedef char more_funcs_assert_save_file_data_pokemon_photo_scores_at_6c[(offsetof(SaveFile, data.pokemonPhotoScores) == 0x6C) ? 1 : -1];
typedef char more_funcs_assert_save_file_data_pokemon_photos_at_180[(offsetof(SaveFile, data.pokemonPhotos) == 0x180) ? 1 : -1];
typedef char more_funcs_assert_save_file_data_report_photos_at_fba0[(offsetof(SaveFile, data.reportPhotos) == 0xFBA0) ? 1 : -1];
typedef char more_funcs_assert_save_file_data_album_photos_at_10a20[(offsetof(SaveFile, data.albumPhotos) == 0x10A20) ? 1 : -1];
typedef char more_funcs_assert_save_file_size[(sizeof(SaveFile) == 0x1F2A4) ? 1 : -1];

void func_800BF954_5C7F4(s32, PhotoData*, AlbumComment*);
s32 func_800C0224_5D0C4(s32);
void func_800C0254_5D0F4(s32, s32);
s32 func_800C06A8_5D548(uintptr_t, s32 size);
s32 func_800C09C0_5D860(uintptr_t, s32 size);
s32 func_800C0AB4_5D954(uintptr_t, s32);
MD4Ctx* MD4_Finalize(u32*, s32);
#endif

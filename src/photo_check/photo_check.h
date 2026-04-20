#include "types.h"

typedef struct Photo {
    /* 0x00 */ PhotoData* photoData;
    /* 0x04 */ u32 totalScore;
    /* 0x08 */ u8 poseID;
    /* 0x09 */ u8 samePkmnNumber;
    /* 0x0A */ s16 pkmnID;
    /* 0x0C */ s16 specialID;
    /* 0x0E */ s16 isWellFramed;
    /* 0x10 */ s16 posePts;
    /* 0x12 */ s16 completenessScore;
    /* 0x14 */ s16 proximityScore;
    /* 0x16 */ s16 specialBonus;
    /* 0x18 */ s16 samePkmnBonus;
    /* 0x1A */ s16 unk_1A_15 : 1;
    /* 0x1A */ s16 unk_1A_14 : 1;
    /* 0x1A */ s16 unk_1A_13 : 1;
    /* 0x1A */ s16 unk_1A_12 : 1;
    /* 0x1A */ s16 unk_1A_11 : 1;
    // /* 0x1A */ s16 unk_1A_10 : 1;
    // /* 0x1A */ s16 unk_1A_9 : 1;
    // /* 0x1A */ s16 unk_1A_8 : 1;
    // /* 0x1A */ s16 unk_1A_7 : 1;
    // /* 0x1A */ s16 unk_1A_6 : 1;
    // /* 0x1A */ s16 unk_1A_5 : 1;
    // /* 0x1A */ s16 unk_1A_4 : 1;
    // /* 0x1A */ s16 unk_1A_3 : 1;
    // /* 0x1A */ s16 unk_1A_2 : 1;
    // /* 0x1A */ s16 unk_1A_1 : 1;
    // /* 0x1A */ s16 unk_1A_0 : 1;
} Photo; // size = 0x1C

typedef struct ScoreData {
    /* 0x00 */ s32 totalScore;
    /* 0x04 */ u16 samePkmnBonus;
    /* 0x06 */ u8 samePkmnNumber;
    /* 0x07 */ u8 isWellFramed;
    /* 0x08 */ u8 poseID;
    /* 0x0A */ s16 pokemonInFocus;
    /* 0x0C */ u16 proximityScore;
    /* 0x0E */ u16 completenessScore;
    /* 0x10 */ u16 posePts;
    /* 0x12 */ s16 specialBonus;
    /* 0x14 */ u8 specialID;
} ScoreData; // size = 0x18

typedef struct PhotoScoreBundle {
    /* 0x000 */ PhotoData photo;
    /* 0x3A0 */ ScoreData score;
} PhotoScoreBundle; // size = 0x3B8

typedef char photo_check_assert_photo_photo_data_at_0[(offsetof(Photo, photoData) == 0x0) ? 1 : -1];
typedef char photo_check_assert_photo_total_score_at_4[(offsetof(Photo, totalScore) == 0x4) ? 1 : -1];
typedef char photo_check_assert_photo_pose_id_at_8[(offsetof(Photo, poseID) == 0x8) ? 1 : -1];
typedef char photo_check_assert_photo_same_pkmn_number_at_9[(offsetof(Photo, samePkmnNumber) == 0x9) ? 1 : -1];
typedef char photo_check_assert_photo_pkmn_id_at_a[(offsetof(Photo, pkmnID) == 0xA) ? 1 : -1];
typedef char photo_check_assert_photo_special_id_at_c[(offsetof(Photo, specialID) == 0xC) ? 1 : -1];
typedef char photo_check_assert_photo_is_well_framed_at_e[(offsetof(Photo, isWellFramed) == 0xE) ? 1 : -1];
typedef char photo_check_assert_photo_pose_pts_at_10[(offsetof(Photo, posePts) == 0x10) ? 1 : -1];
typedef char photo_check_assert_photo_completeness_score_at_12[(offsetof(Photo, completenessScore) == 0x12) ? 1 : -1];
typedef char photo_check_assert_photo_proximity_score_at_14[(offsetof(Photo, proximityScore) == 0x14) ? 1 : -1];
typedef char photo_check_assert_photo_special_bonus_at_16[(offsetof(Photo, specialBonus) == 0x16) ? 1 : -1];
typedef char photo_check_assert_photo_same_pkmn_bonus_at_18[(offsetof(Photo, samePkmnBonus) == 0x18) ? 1 : -1];
typedef char photo_check_assert_photo_size[(sizeof(Photo) == 0x1C) ? 1 : -1];

typedef char photo_check_assert_score_data_total_score_at_0[(offsetof(ScoreData, totalScore) == 0x0) ? 1 : -1];
typedef char photo_check_assert_score_data_same_pkmn_bonus_at_4[(offsetof(ScoreData, samePkmnBonus) == 0x4) ? 1 : -1];
typedef char photo_check_assert_score_data_same_pkmn_number_at_6[(offsetof(ScoreData, samePkmnNumber) == 0x6) ? 1 : -1];
typedef char photo_check_assert_score_data_is_well_framed_at_7[(offsetof(ScoreData, isWellFramed) == 0x7) ? 1 : -1];
typedef char photo_check_assert_score_data_pose_id_at_8[(offsetof(ScoreData, poseID) == 0x8) ? 1 : -1];
typedef char photo_check_assert_score_data_pokemon_in_focus_at_a[(offsetof(ScoreData, pokemonInFocus) == 0xA) ? 1 : -1];
typedef char photo_check_assert_score_data_proximity_score_at_c[(offsetof(ScoreData, proximityScore) == 0xC) ? 1 : -1];
typedef char photo_check_assert_score_data_completeness_score_at_e[(offsetof(ScoreData, completenessScore) == 0xE) ? 1 : -1];
typedef char photo_check_assert_score_data_pose_pts_at_10[(offsetof(ScoreData, posePts) == 0x10) ? 1 : -1];
typedef char photo_check_assert_score_data_special_bonus_at_12[(offsetof(ScoreData, specialBonus) == 0x12) ? 1 : -1];
typedef char photo_check_assert_score_data_special_id_at_14[(offsetof(ScoreData, specialID) == 0x14) ? 1 : -1];
typedef char photo_check_assert_score_data_size[(sizeof(ScoreData) == 0x18) ? 1 : -1];

typedef char photo_check_assert_photo_score_bundle_photo_at_0[(offsetof(PhotoScoreBundle, photo) == 0x0) ? 1 : -1];
typedef char photo_check_assert_photo_score_bundle_score_at_3a0[(offsetof(PhotoScoreBundle, score) == 0x3A0) ? 1 : -1];
typedef char photo_check_assert_photo_score_bundle_size[(sizeof(PhotoScoreBundle) == 0x3B8) ? 1 : -1];

PhotoScoreBundle* func_8037452C_847CDC(PhotoData*);

s32 func_801E41FC_993C6C(UnkStruct800BEDF8* arg0, s32* arg1);
Photo* func_801E459C_99400C(s32 idx);

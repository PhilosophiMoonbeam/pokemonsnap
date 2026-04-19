#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "types.h"
#include "ultralib/include/macros.h"
#include "common_structs.h"

#define PARTICLE_BANKS_MAX 8

enum ParticleFlags {
    PARTICLE_FLAG_CI_SHARED_PALETTE = 0x10,
    PARTICLE_FLAG_MIRROR_S = 0x20,
    PARTICLE_FLAG_MIRROR_T = 0x40,
    PARTICLE_FLAG_ENV_COMBINE = 0x80,
    PARTICLE_FLAG_NOISE_COMBINE = 0x100,
    PARTICLE_FLAG_ENV_ALPHA_THRESHOLD = 0x200,
    PARTICLE_FLAG_ALPHA_DITHER = 0x400,
    PARTICLE_FLAG_PAUSE = 0x800
};

#define FX_PHOTO_RENDER_FLAG_SHIFT 4
#define FX_PHOTO_RENDER_FLAG_MASK 0xFF0
#define FX_PACK_PHOTO_RENDER_FLAGS(flags) (((flags) & FX_PHOTO_RENDER_FLAG_MASK) >> FX_PHOTO_RENDER_FLAG_SHIFT)
#define FX_UNPACK_PHOTO_RENDER_FLAGS(packed) (((u32) (packed)) << FX_PHOTO_RENDER_FLAG_SHIFT)

typedef struct EffectScript {
    /* 0x00 */ u16 kind; // Effect kind
    /* 0x02 */ u16 textureID;
    /* 0x04 */ u16 effectLifetime;
    /* 0x06 */ u16 particleLifetime;
    /* 0x08 */ u32 flags;
    /* 0x0C */ f32 gravity;
    /* 0x10 */ f32 friction;
    /* 0x14 */ Vec3f vel;
    /* 0x20 */ f32 unk_20;
    /* 0x24 */ f32 unk_24;
    /* 0x28 */ f32 unk_28;
    /* 0x2C */ f32 size;
    /* 0x30 */ u8 bytecode[1]; // variable length
} EffectScript;                // size > 0x30

typedef char fx_assert_effect_script_kind_at_0[(offsetof(EffectScript, kind) == 0x0) ? 1 : -1];
typedef char fx_assert_effect_script_texture_id_at_2[(offsetof(EffectScript, textureID) == 0x2) ? 1 : -1];
typedef char fx_assert_effect_script_effect_lifetime_at_4[(offsetof(EffectScript, effectLifetime) == 0x4) ? 1 : -1];
typedef char fx_assert_effect_script_particle_lifetime_at_6[(offsetof(EffectScript, particleLifetime) == 0x6) ? 1 : -1];
typedef char fx_assert_effect_script_flags_at_8[(offsetof(EffectScript, flags) == 0x8) ? 1 : -1];
typedef char fx_assert_effect_script_gravity_at_c[(offsetof(EffectScript, gravity) == 0xC) ? 1 : -1];
typedef char fx_assert_effect_script_friction_at_10[(offsetof(EffectScript, friction) == 0x10) ? 1 : -1];
typedef char fx_assert_effect_script_vel_at_14[(offsetof(EffectScript, vel) == 0x14) ? 1 : -1];
typedef char fx_assert_effect_script_unk_20_at_20[(offsetof(EffectScript, unk_20) == 0x20) ? 1 : -1];
typedef char fx_assert_effect_script_unk_24_at_24[(offsetof(EffectScript, unk_24) == 0x24) ? 1 : -1];
typedef char fx_assert_effect_script_unk_28_at_28[(offsetof(EffectScript, unk_28) == 0x28) ? 1 : -1];
typedef char fx_assert_effect_script_size_at_2c[(offsetof(EffectScript, size) == 0x2C) ? 1 : -1];
typedef char fx_assert_effect_script_bytecode_at_30[(offsetof(EffectScript, bytecode) == 0x30) ? 1 : -1];

typedef struct ParticleScriptDesc {
    /* 0x00 */ s32 count;
    /* 0x04 */ EffectScript* scripts[1]; // variable length
} ParticleScriptDesc;                    // size > 0x4

typedef char fx_assert_particle_script_desc_count_at_0[(offsetof(ParticleScriptDesc, count) == 0) ? 1 : -1];
typedef char fx_assert_particle_script_desc_scripts_at_4[(offsetof(ParticleScriptDesc, scripts) == sizeof(s32)) ? 1 : -1];

typedef struct EffectSprites {
    /* 0x00 */ u32 numFrames;
    /* 0x04 */ s32 fmt;
    /* 0x08 */ s32 siz;
    /* 0x0C */ s32 width;
    /* 0x10 */ s32 height;
    /* 0x14 */ s32 flags;
    /* 0x18 */ u8* data[1]; // variable length
} EffectSprites;

typedef char fx_assert_effect_sprites_num_frames_at_0[(offsetof(EffectSprites, numFrames) == 0x0) ? 1 : -1];
typedef char fx_assert_effect_sprites_fmt_at_4[(offsetof(EffectSprites, fmt) == 0x4) ? 1 : -1];
typedef char fx_assert_effect_sprites_siz_at_8[(offsetof(EffectSprites, siz) == 0x8) ? 1 : -1];
typedef char fx_assert_effect_sprites_width_at_c[(offsetof(EffectSprites, width) == 0xC) ? 1 : -1];
typedef char fx_assert_effect_sprites_height_at_10[(offsetof(EffectSprites, height) == 0x10) ? 1 : -1];
typedef char fx_assert_effect_sprites_flags_at_14[(offsetof(EffectSprites, flags) == 0x14) ? 1 : -1];
typedef char fx_assert_effect_sprites_data_at_18[(offsetof(EffectSprites, data) == 0x18) ? 1 : -1];

typedef struct ParticleSpritesDesc {
    /* 0x00 */ s32 count;
    /* 0x04 */ EffectSprites* sprites[1]; // variable length
} ParticleSpritesDesc;                    // size > 0x4

typedef char fx_assert_particle_sprites_desc_count_at_0[(offsetof(ParticleSpritesDesc, count) == 0) ? 1 : -1];
typedef char fx_assert_particle_sprites_desc_sprites_at_4[(offsetof(ParticleSpritesDesc, sprites) == sizeof(s32)) ? 1 : -1];

typedef char fx_assert_effect_vars_size[(sizeof(EffectVars) == 0xC) ? 1 : -1];
typedef char fx_assert_effect_next_at_0[(offsetof(Effect, next) == 0x0) ? 1 : -1];
typedef char fx_assert_effect_effect_id_at_4[(offsetof(Effect, effectID) == 0x4) ? 1 : -1];
typedef char fx_assert_effect_flags_at_6[(offsetof(Effect, flags) == 0x6) ? 1 : -1];
typedef char fx_assert_effect_kind_at_8[(offsetof(Effect, kind) == 0x8) ? 1 : -1];
typedef char fx_assert_effect_bank_id_at_9[(offsetof(Effect, bankID) == 0x9) ? 1 : -1];
typedef char fx_assert_effect_texture_id_at_a[(offsetof(Effect, textureID) == 0xA) ? 1 : -1];
typedef char fx_assert_effect_particle_lifetime_at_c[(offsetof(Effect, particleLifetime) == 0xC) ? 1 : -1];
typedef char fx_assert_effect_effect_lifetime_at_e[(offsetof(Effect, effectLifetime) == 0xE) ? 1 : -1];
typedef char fx_assert_effect_bytecode_at_10[(offsetof(Effect, bytecode) == 0x10) ? 1 : -1];
typedef char fx_assert_effect_pos_at_14[(offsetof(Effect, pos) == 0x14) ? 1 : -1];
typedef char fx_assert_effect_vel_at_20[(offsetof(Effect, vel) == 0x20) ? 1 : -1];
typedef char fx_assert_effect_gravity_at_2c[(offsetof(Effect, gravity) == 0x2C) ? 1 : -1];
typedef char fx_assert_effect_friction_at_30[(offsetof(Effect, friction) == 0x30) ? 1 : -1];
typedef char fx_assert_effect_size_at_34[(offsetof(Effect, size) == 0x34) ? 1 : -1];
typedef char fx_assert_effect_unk_38_at_38[(offsetof(Effect, unk_38) == 0x38) ? 1 : -1];
typedef char fx_assert_effect_unk_3c_at_3c[(offsetof(Effect, unk_3C) == 0x3C) ? 1 : -1];
typedef char fx_assert_effect_unk_40_at_40[(offsetof(Effect, unk_40) == 0x40) ? 1 : -1];
typedef char fx_assert_effect_unk_44_at_44[(offsetof(Effect, unk_44) == 0x44) ? 1 : -1];
typedef char fx_assert_effect_dobj_at_48[(offsetof(Effect, dobj) == 0x48) ? 1 : -1];
typedef char fx_assert_effect_effect_vars_at_4c[(offsetof(Effect, effectVars) == 0x4C) ? 1 : -1];
typedef char fx_assert_effect_sizeof_58[(sizeof(Effect) == 0x58) ? 1 : -1];

typedef struct Particle {
    /* 0x00 */ struct Particle* next;
    /* 0x04 */ u16 effectID;
    /* 0x06 */ u16 flags;
    /* 0x08 */ u8 bankID;
    /* 0x09 */ u8 loopCount;
    /* 0x0A */ u8 textureID;
    /* 0x0B */ u8 dataID;
    /* 0x0C */ u16 bytecodeTimer;
    /* 0x0E */ u16 sizeTargetLength;
    /* 0x10 */ u16 targetPrimColorLength;
    /* 0x12 */ u16 targetEnvColorLength;
    /* 0x14 */ u8* bytecode;
    /* 0x18 */ u16 bytecodePos;
    /* 0x1A */ u16 returnPtr;
    /* 0x1C */ u16 loopPtr;
    /* 0x1E */ u16 lifetime;
    /* 0x20 */ Vec3f pos;
    /* 0x2C */ Vec3f vel;
    /* 0x38 */ f32 gravity;
    /* 0x3C */ f32 friction;
    /* 0x40 */ f32 size;
    /* 0x44 */ f32 sizeTarget;
    /* 0x48 */ ucolor primColor;
    /* 0x4C */ ucolor targetPrimColor;
    /* 0x50 */ ucolor envColor;
    /* 0x54 */ ucolor targetEnvColor;
    /* 0x58 */ Effect* fx;
} Particle; // size = 0x5C

typedef char fx_assert_particle_next_at_0[(offsetof(Particle, next) == 0x0) ? 1 : -1];
typedef char fx_assert_particle_effect_id_at_4[(offsetof(Particle, effectID) == 0x4) ? 1 : -1];
typedef char fx_assert_particle_flags_at_6[(offsetof(Particle, flags) == 0x6) ? 1 : -1];
typedef char fx_assert_particle_bank_id_at_8[(offsetof(Particle, bankID) == 0x8) ? 1 : -1];
typedef char fx_assert_particle_loop_count_at_9[(offsetof(Particle, loopCount) == 0x9) ? 1 : -1];
typedef char fx_assert_particle_texture_id_at_a[(offsetof(Particle, textureID) == 0xA) ? 1 : -1];
typedef char fx_assert_particle_data_id_at_b[(offsetof(Particle, dataID) == 0xB) ? 1 : -1];
typedef char fx_assert_particle_bytecode_timer_at_c[(offsetof(Particle, bytecodeTimer) == 0xC) ? 1 : -1];
typedef char fx_assert_particle_size_target_length_at_e[(offsetof(Particle, sizeTargetLength) == 0xE) ? 1 : -1];
typedef char fx_assert_particle_target_prim_color_length_at_10[(offsetof(Particle, targetPrimColorLength) == 0x10) ? 1 : -1];
typedef char fx_assert_particle_target_env_color_length_at_12[(offsetof(Particle, targetEnvColorLength) == 0x12) ? 1 : -1];
typedef char fx_assert_particle_bytecode_at_14[(offsetof(Particle, bytecode) == 0x14) ? 1 : -1];
typedef char fx_assert_particle_bytecode_pos_at_18[(offsetof(Particle, bytecodePos) == 0x18) ? 1 : -1];
typedef char fx_assert_particle_return_ptr_at_1a[(offsetof(Particle, returnPtr) == 0x1A) ? 1 : -1];
typedef char fx_assert_particle_loop_ptr_at_1c[(offsetof(Particle, loopPtr) == 0x1C) ? 1 : -1];
typedef char fx_assert_particle_lifetime_at_1e[(offsetof(Particle, lifetime) == 0x1E) ? 1 : -1];
typedef char fx_assert_particle_pos_at_20[(offsetof(Particle, pos) == 0x20) ? 1 : -1];
typedef char fx_assert_particle_vel_at_2c[(offsetof(Particle, vel) == 0x2C) ? 1 : -1];
typedef char fx_assert_particle_gravity_at_38[(offsetof(Particle, gravity) == 0x38) ? 1 : -1];
typedef char fx_assert_particle_friction_at_3c[(offsetof(Particle, friction) == 0x3C) ? 1 : -1];
typedef char fx_assert_particle_size_at_40[(offsetof(Particle, size) == 0x40) ? 1 : -1];
typedef char fx_assert_particle_size_target_at_44[(offsetof(Particle, sizeTarget) == 0x44) ? 1 : -1];
typedef char fx_assert_particle_prim_color_at_48[(offsetof(Particle, primColor) == 0x48) ? 1 : -1];
typedef char fx_assert_particle_target_prim_color_at_4c[(offsetof(Particle, targetPrimColor) == 0x4C) ? 1 : -1];
typedef char fx_assert_particle_env_color_at_50[(offsetof(Particle, envColor) == 0x50) ? 1 : -1];
typedef char fx_assert_particle_target_env_color_at_54[(offsetof(Particle, targetEnvColor) == 0x54) ? 1 : -1];
typedef char fx_assert_particle_fx_at_58[(offsetof(Particle, fx) == 0x58) ? 1 : -1];
typedef char fx_assert_particle_sizeof_5c[(sizeof(Particle) == 0x5C) ? 1 : -1];

extern Particle* D_800BE1A8[16];
extern s32 fx_ScriptBanksNum[PARTICLE_BANKS_MAX];
extern s32 fx_SpriteBanksNum[PARTICLE_BANKS_MAX];
extern EffectSprites** fx_SpriteBanks[PARTICLE_BANKS_MAX];

void fx_setupBankID(s32 bankID, UNK_PTR scriptDesc, UNK_PTR spritesDesc);
void fx_setDitherModes(s32 color, s32 alpha);
GObj* fx_allocEffects(s32 num);
void fx_getPosVelDObj(Vec3f* pos, Vec3f* vel, DObj* dobj);
Effect* fx_createEffect(s32 bankID, s32 scriptID);
void fx_ejectEffectDObj(GObj* obj);
void fx_translatePosAll(f32, f32, f32);

#endif

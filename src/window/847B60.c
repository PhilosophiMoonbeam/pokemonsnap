#include "common.h"
#include "sys/gtl.h"
#include "app_render/app_render.h"
#include "photo_check/photo_check.h"

typedef struct WindowPhotoBuffer {
    u16* buf;
    s32 width;
    s32 height;
    s32 unkC;
    u16* unk10;
    s32 unk14;
    s32 unk18;
} WindowPhotoBuffer;

extern s32 D_803A6660_879E10;
extern s32 D_803A6664_879E14;
extern f32 D_803A6668_879E18;
extern GObj* D_803A666C_879E1C;
extern bool D_803A6670_879E20;
extern s32 D_803A6674_879E24;
extern s32 D_803A6678_879E28;
extern PhotoData* D_803A667C_879E2C;
extern WindowPhotoBuffer D_803A6684_879E34;

static u16* D_803A6C10_87A3C0;
static u16* D_803A6C14_87A3C4;
static Unk803A6C18 D_803A6C18_87A3C8;

void func_803743B0_847B60(f32 arg0) {
    D_803A6668_879E18 = arg0;
}

void func_803743BC_847B6C(GObj* camGobj) {
    GObj* gobj;
    OMCamera* cam;

    if (D_803A6670_879E20) {
        D_803A6670_879E20 = false;
        gtlWaitAllGfxTasksDone();
        initObjectsOnPhoto(D_803A666C_879E1C->data.cam, D_803A667C_879E2C);

        gobj = omGObjListHead[LINK_18];
        while (gobj != NULL) {
            ohPauseObjectProcesses(gobj);
            gobj = gobj->next;
        }

        gobj = omGObjListHead[LINK_17];
        while (gobj != NULL) {
            ohPauseObjectProcesses(gobj);
            gobj = gobj->next;
        }

        omCurrentCamera = camGobj;
        cam = camGobj->data.cam;
        cam->flags |= CAMERA_FLAG_10;
        cam->perspMtx.ortho.r /= D_803A6668_879E18;
        func_800A844C(cam, 0, 0, D_803A6674_879E24, D_803A6678_879E28);
        renInitCameraEx(gMainGfxPos, cam, 0, D_803A6C10_87A3C0, D_803A6660_879E10, D_803A6664_879E14, D_803A6C14_87A3C4);
        renPrepareCameraMatrix(gMainGfxPos, cam);
        renCameraPreRender(cam, 0);
        renCameraRenderObjects(camGobj, 0);
        renCameraPostRender(cam);
        cam->flags &= ~CAMERA_FLAG_10;
    }
}

Unk803A6C18* func_8037452C_847CDC(PhotoData* arg0) {
    ScoreData* temp_v0;

    if (arg0 == NULL) {
        return NULL;
    }
    temp_v0 = func_800A0EA4(D_803A666C_879E1C, arg0, D_803A6C10_87A3C0, D_803A6660_879E10, D_803A6664_879E14, D_803A6C14_87A3C4);

    D_803A6C18_87A3C8.photo = *arg0;
    D_803A6C18_87A3C8.score = *temp_v0;

    return &D_803A6C18_87A3C8;
}

WindowPhotoBuffer* func_80374608_847DB8(s32 arg0, s32 arg1, PhotoData* arg2) {
    D_803A6684_879E34.buf = D_803A6C10_87A3C0;

    if (arg0 < 16) {
        arg0 = 16;
    }
    if (arg0 > 320) {
        arg0 = 320;
    }
    if (arg1 < 16) {
        arg1 = 16;
    }
    if (arg1 > 210) {
        arg1 = 210;
    }
    if (arg2 == 0) {
        return NULL;
    }
    D_803A6674_879E24 = arg0;
    D_803A6678_879E28 = arg1;
    D_803A667C_879E2C = arg2;
    D_803A6670_879E20 = true;
    gtlDrawOne(D_803A666C_879E1C);
    return &D_803A6684_879E34;
}

static void nullsub() {
}

PhotoData* func_803746B4_847E64(s32 arg0) {
    if (arg0 < 0 || arg0 >= func_8009BC68()) {
        return NULL;
    }
    return &func_8009BC74()[arg0];
}

#ifdef NON_MATCHING

s32 func_80374714_847EC4(PhotoData* photo, Sprite* sprite) {
    WindowPhotoBuffer* src;

    if (photo != NULL) {
        src = func_80374608_847DB8(sprite->width * 2, sprite->height * 2, photo);
    } else {
        src = NULL;
    }

    if (src == NULL) {
        Bitmap* bitmap = sprite->bitmap;
        u16* dst = bitmap->buf;
        s32 texelCount = bitmap->width_img * sprite->height;
        s32 i;

        for (i = 0; i < texelCount; i++) {
            dst[i] = 0;
        }
        osWritebackDCache(bitmap->buf, texelCount);
        return 1;
    }

    {
        Bitmap* bitmap = sprite->bitmap;
        s32 rowInBitmap = 0;
        s32 tileHeight = bitmap->actualHeight;
        s32 y;
        s32 x;

        for (y = 0; y < sprite->height; y++) {
            u16* srcRow0;
            u16* srcRow1;
            u16* dst;

            if (rowInBitmap >= tileHeight) {
                rowInBitmap = 0;
                bitmap++;
                tileHeight = bitmap->actualHeight;
            }

            srcRow0 = src->buf + ((y * src->width) * 2);
            srcRow1 = srcRow0 + src->width;
            dst = (u16*) bitmap->buf + (rowInBitmap * bitmap->width_img);

            x = 0;
            if (sprite->width & 1) {
            s32 r;
            s32 g;
            s32 bl;

            r = (srcRow0[0] & 0xF800) + (srcRow0[1] & 0xF800) + (srcRow1[0] & 0xF800) + (srcRow1[1] & 0xF800);
            g = (srcRow0[0] & 0x07C0) + (srcRow0[1] & 0x07C0) + (srcRow1[0] & 0x07C0) + (srcRow1[1] & 0x07C0);
            bl = (srcRow0[0] & 0x003E) + (srcRow0[1] & 0x003E) + (srcRow1[0] & 0x003E) + (srcRow1[1] & 0x003E);
            if (rowInBitmap & 1) {
                *(u16*) (((uintptr_t) dst) ^ 4) = ((r / 4) & 0xF800) | ((g / 4) & 0x07C0) | ((bl / 4) & 0x003E) | 1;
            } else {
                dst[0] = ((r / 4) & 0xF800) | ((g / 4) & 0x07C0) | ((bl / 4) & 0x003E) | 1;
            }
            srcRow0 += 2;
            srcRow1 += 2;
            dst++;
            x = 1;
        }

        if (rowInBitmap & 1) {
            for (; x < sprite->width; x += 2) {
                s32 r;
                s32 g;
                s32 bl;

                r = (srcRow0[0] & 0xF800) + (srcRow0[1] & 0xF800) + (srcRow1[0] & 0xF800) + (srcRow1[1] & 0xF800);
                g = (srcRow0[0] & 0x07C0) + (srcRow0[1] & 0x07C0) + (srcRow1[0] & 0x07C0) + (srcRow1[1] & 0x07C0);
                bl = (srcRow0[0] & 0x003E) + (srcRow0[1] & 0x003E) + (srcRow1[0] & 0x003E) + (srcRow1[1] & 0x003E);
                *(u16*) (((uintptr_t) dst) ^ 4) = ((r / 4) & 0xF800) | ((g / 4) & 0x07C0) | ((bl / 4) & 0x003E) | 1;
                srcRow0 += 2;
                srcRow1 += 2;
                dst++;

                r = (srcRow0[0] & 0xF800) + (srcRow0[1] & 0xF800) + (srcRow1[0] & 0xF800) + (srcRow1[1] & 0xF800);
                g = (srcRow0[0] & 0x07C0) + (srcRow0[1] & 0x07C0) + (srcRow1[0] & 0x07C0) + (srcRow1[1] & 0x07C0);
                bl = (srcRow0[0] & 0x003E) + (srcRow0[1] & 0x003E) + (srcRow1[0] & 0x003E) + (srcRow1[1] & 0x003E);
                *(u16*) (((uintptr_t) dst) ^ 4) = ((r / 4) & 0xF800) | ((g / 4) & 0x07C0) | ((bl / 4) & 0x003E) | 1;
                srcRow0 += 2;
                srcRow1 += 2;
                dst++;
            }
        } else {
            for (; x < sprite->width; x += 2) {
                s32 r;
                s32 g;
                s32 bl;

                r = (srcRow0[0] & 0xF800) + (srcRow0[1] & 0xF800) + (srcRow1[0] & 0xF800) + (srcRow1[1] & 0xF800);
                g = (srcRow0[0] & 0x07C0) + (srcRow0[1] & 0x07C0) + (srcRow1[0] & 0x07C0) + (srcRow1[1] & 0x07C0);
                bl = (srcRow0[0] & 0x003E) + (srcRow0[1] & 0x003E) + (srcRow1[0] & 0x003E) + (srcRow1[1] & 0x003E);
                dst[0] = ((r / 4) & 0xF800) | ((g / 4) & 0x07C0) | ((bl / 4) & 0x003E) | 1;
                srcRow0 += 2;
                srcRow1 += 2;
                dst++;

                r = (srcRow0[0] & 0xF800) + (srcRow0[1] & 0xF800) + (srcRow1[0] & 0xF800) + (srcRow1[1] & 0xF800);
                g = (srcRow0[0] & 0x07C0) + (srcRow0[1] & 0x07C0) + (srcRow1[0] & 0x07C0) + (srcRow1[1] & 0x07C0);
                bl = (srcRow0[0] & 0x003E) + (srcRow0[1] & 0x003E) + (srcRow1[0] & 0x003E) + (srcRow1[1] & 0x003E);
                dst[0] = ((r / 4) & 0xF800) | ((g / 4) & 0x07C0) | ((bl / 4) & 0x003E) | 1;
                srcRow0 += 2;
                srcRow1 += 2;
                dst++;
            }
        }

            rowInBitmap++;
        }
    }

    osWritebackDCache(sprite->bitmap->buf, sprite->bitmap->width_img * sprite->height);
    return 0;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/window/847B60/func_80374714_847EC4.s")
#endif

void func_80374D20_8484D0(void) {
    func_8009FBC4();
}

void func_80374D40_8484F0(void) {
    GObj* camera;
    UNUSED s32 pad;
    u8 r, g, b;
    UNUSED s32 pad2;
    OMCamera* cam;

    D_803A6C10_87A3C0 = gtlMalloc(0x20D00, 0x80);
    D_803A6C14_87A3C4 = gtlMalloc(0x20D00, 0x80);
    camera = ohCreateCamera(0x200, ohUpdateDefault, LINK_5, 5, func_803743BC_847B6C, 19, 1 << 19, -1, 0, 0, NULL, 0, 1);
    cam = camera->data.cam;
    omCameraAddMtx(cam, MTX_TYPE_PERSP_FAST, 0);
    omCameraAddMtx(cam, MTX_TYPE_LOOKAT_REFLECT, 0);
    cam->flags |= CAMERA_FLAG_10 | CAMERA_FLAG_4 | CAMERA_FLAG_2 | CAMERA_FLAG_1;
    func_800A844C(cam, 0, 0, SCREEN_WIDTH, 210);
    cam->viewMtx.lookAt.up.x = 0.0f;
    cam->viewMtx.lookAt.up.y = 0.0f;
    cam->viewMtx.lookAt.up.z = 1.0f;
    cam->viewMtx.lookAt.at = D_800AF0C4;
    cam->viewMtx.lookAt.eye = D_800AF094;
    func_8009FB50(0x13, 0x12, 0x11);
    D_803A666C_879E1C = camera;
    D_803A6670_879E20 = false;
    cam->perspMtx.persp.far = 8000.0f;
    cam->perspMtx.persp.fovy = 33.0f;
    cam->perspMtx.persp.near = 8000.0f;
    cam->perspMtx.persp.near *= 0.0078125;
    cam->perspMtx.persp.scale = 1.0f;
    getBackgroundColor(&r, &g, &b);
    cam->bgColor = (r << 0x18) | (g << 0x10) | (b << 8) | 1;
    D_803A666C_879E1C = camera;
}

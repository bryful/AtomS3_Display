#pragma once
#ifndef M5AOMS3FACE
#define M5AOMS3FACE

#include <LittleFS.h>
#include <arduino.h>
#include <M5Unified.h>

extern m5::M5Unified M5;

#define offscreen_bit 4
#if (offscreen_bit == 4)
#define offscreen_transport 0
#else
#define offscreen_transport TFT_BLUE
#endif
#define skin_number 1
namespace face
{
#define EYE_POS 0
#define hoho_pos 75
#define mouth_xpos 30
#define mouth_ypos 98 - 5

    class M5AtomS3Face
    {
    private:
        bool _isSetup;
        m5::M5Unified *_m5;
        // 目パチ用の裏画面
        m5gfx::M5Canvas *Comp;
        m5gfx::M5Canvas *EyeC0;
        m5gfx::M5Canvas *EyeC1;
        m5gfx::M5Canvas *EyeR0;
        m5gfx::M5Canvas *EyeR1;
        m5gfx::M5Canvas *EyeL0;
        m5gfx::M5Canvas *EyeL1;
        m5gfx::M5Canvas *EyeBig;
        m5gfx::M5Canvas *Hoho;
        m5gfx::M5Canvas *EyeClose;
        m5gfx::M5Canvas *MouthOpen;
        m5gfx::M5Canvas *MouthClose1;
        m5gfx::M5Canvas *MouthClose2;
        m5gfx::M5Canvas *MouthBig;
        int _SkinColor;
        int _EyeMode;
        int _MouthMode;
        int CountMM;
        void setupFace();
        void DrawMouth();
        void DrawEyeClose();
        void DrawEyeInt();
        void DrawEye();
        void DrawHoho();
        void DrawBig();
        void ChangeFace()
        {
            _EyeMode = random(0, 3);
            _MouthMode = random(0, 4);
        }

    public:
        M5AtomS3Face();
        ~M5AtomS3Face();
        void Begin(m5::M5Unified *m);
        void Redraw();
        void DrawNormal();
        void blink();
        void DrawBigEye();
        void SetPalette(int idx, uint16_t color);
        void SetSkinColor(int color)
        {
            _SkinColor = color;
            _EyeMode = 3;
            SetPalette(skin_number, _SkinColor);
            DrawNormal();
        }
        int GetSkinColor()
        {
            return _SkinColor;
        }
    };
}
#endif
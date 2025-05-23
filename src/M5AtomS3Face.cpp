#include "M5AtomS3Face.hpp"

namespace face
{
    M5AtomS3Face::M5AtomS3Face()
    {
        _isSetup = false;
        _SkinColor = M5.Display.color565(0xD1, 0xA7, 0x81);
        _EyeMode = 0;
        _MouthMode = 0;
        CountMM = 0;
    }
    M5AtomS3Face::~M5AtomS3Face()
    {
        if (_isSetup == true)
        {
            delete Comp;
            delete EyeC0;
            delete EyeC1;
            delete EyeR0;
            delete EyeR1;
            delete EyeL0;
            delete EyeL1;
            delete EyeBig;
            delete Hoho;
            delete EyeClose;
            delete MouthOpen;
            delete MouthClose1;
            delete MouthClose2;
            delete MouthBig;
        }
    }
    void M5AtomS3Face::Begin(m5::M5Unified *m)
    {
        _m5 = m;
        Comp = new m5gfx::M5Canvas(&(m->Display));
        EyeC0 = new m5gfx::M5Canvas(Comp);
        EyeC1 = new m5gfx::M5Canvas(Comp);
        EyeR0 = new m5gfx::M5Canvas(Comp);
        EyeR1 = new m5gfx::M5Canvas(Comp);
        EyeL0 = new m5gfx::M5Canvas(Comp);
        EyeL1 = new m5gfx::M5Canvas(Comp);
        EyeBig = new m5gfx::M5Canvas(Comp);
        Hoho = new m5gfx::M5Canvas(Comp);
        EyeClose = new m5gfx::M5Canvas(Comp);
        MouthOpen = new m5gfx::M5Canvas(Comp);
        MouthClose1 = new m5gfx::M5Canvas(Comp);
        MouthClose2 = new m5gfx::M5Canvas(Comp);
        MouthBig = new m5gfx::M5Canvas(Comp);

        _isSetup = true;
        setupFace();
    }
    // -----------------------------------------------------------------------------
    void M5AtomS3Face::SetPalette(int idx, uint16_t color)
    {
        Comp->setPaletteColor(idx, color);
        EyeC0->setPaletteColor(idx, color);
        EyeC1->setPaletteColor(idx, color);
        EyeR0->setPaletteColor(idx, color);
        EyeR1->setPaletteColor(idx, color);
        EyeL0->setPaletteColor(idx, color);
        EyeL1->setPaletteColor(idx, color);
        EyeBig->setPaletteColor(idx, color);
        Hoho->setPaletteColor(idx, color);
        EyeClose->setPaletteColor(idx, color);
        MouthOpen->setPaletteColor(idx, color);
        MouthClose1->setPaletteColor(idx, color);
        MouthClose2->setPaletteColor(idx, color);
        MouthBig->setPaletteColor(idx, color);
    }
    // -----------------------------------------------------------------------------
    bool LoadPng(m5gfx::M5Canvas *c, const char *f)
    {

        bool res = c->drawBmpFile(LittleFS, f, 0, 0);
        return res;
    }
    void M5AtomS3Face::setupFace()
    {
        if (_isSetup == false)
            return;

        EyeC0->setColorDepth(offscreen_bit);
        EyeC1->setColorDepth(offscreen_bit);
        EyeR0->setColorDepth(offscreen_bit);
        EyeR1->setColorDepth(offscreen_bit);
        EyeL0->setColorDepth(offscreen_bit);
        EyeL1->setColorDepth(offscreen_bit);
        EyeBig->setColorDepth(offscreen_bit);
        Hoho->setColorDepth(offscreen_bit);
        EyeClose->setColorDepth(offscreen_bit);
        MouthOpen->setColorDepth(offscreen_bit);
        MouthClose1->setColorDepth(offscreen_bit);
        MouthClose2->setColorDepth(offscreen_bit);
        MouthBig->setColorDepth(offscreen_bit);
        Comp->setColorDepth(offscreen_bit);

        Comp->createSprite(128, 128);
        EyeC0->createSprite(128, 80);
        EyeC1->createSprite(128, 80);
        EyeR0->createSprite(128, 80);
        EyeR1->createSprite(128, 80);
        EyeL0->createSprite(128, 80);
        EyeL1->createSprite(128, 80);
        EyeBig->createSprite(128, 80);
        EyeClose->createSprite(128, 80);
        MouthOpen->createSprite(68, 24);
        MouthClose1->createSprite(68, 24);
        MouthClose2->createSprite(68, 24);
        MouthBig->createSprite(68, 24);
        Hoho->createSprite(128, 14);

        LittleFS.begin();
        LoadPng(EyeC0, "/eyec0.bmp");
        LoadPng(EyeC1, "/eyec1.bmp");
        LoadPng(EyeR0, "/eyer0.bmp");
        LoadPng(EyeR1, "/eyer1.bmp");
        LoadPng(EyeL0, "/eyel0.bmp");
        LoadPng(EyeL1, "/eyel1.bmp");
        LoadPng(EyeBig, "/eyebig.bmp");
        LoadPng(EyeClose, "/eyeclose.bmp");
        LoadPng(MouthOpen, "/mouthopen.bmp");
        LoadPng(MouthClose1, "/mouthclose1.bmp");
        LoadPng(MouthClose2, "/mouthclose2.bmp");
        LoadPng(MouthBig, "/mouthbig.bmp");
        LoadPng(Hoho, "/hoho.bmp");
        LittleFS.end();
        _isSetup = true;
        SetSkinColor(_SkinColor);
        DrawNormal();
    }
    // -----------------------------------------------------------------------------
    void M5AtomS3Face::Redraw()
    {
        if (_isSetup == false)
            return;
        _m5->Display.startWrite();
        Comp->pushSprite(0, 0);
        _m5->Display.endWrite();
    }
    // -----------------------------------------------------------------------------
    void M5AtomS3Face::DrawNormal()
    {
        if (_isSetup == false)
            return;
#if (offscreen_bit == 4)
        Comp->fillScreen(skin_number);
#else
        Comp->fillScreen(_SkinColor);
#endif
        Hoho->pushSprite(0, hoho_pos, offscreen_transport);
        EyeC1->pushSprite(0, EYE_POS, offscreen_transport);
        MouthOpen->pushSprite(mouth_xpos, mouth_ypos, offscreen_transport);
        Redraw();
    }
    // -----------------------------------------------------------------------------
    void M5AtomS3Face::DrawMouth()
    {
        switch (_MouthMode)
        {
        case 1:
            MouthClose1->pushSprite(mouth_xpos, mouth_ypos, offscreen_transport);
            break;
        case 2:
            MouthClose2->pushSprite(mouth_xpos, mouth_ypos, offscreen_transport);
            break;
        case 3:
            MouthBig->pushSprite(mouth_xpos, mouth_ypos, offscreen_transport);
            break;
        case 0:
        default:
            MouthOpen->pushSprite(mouth_xpos, mouth_ypos, offscreen_transport);
            break;
        }
    }
    // -----------------------------------------------------------------------------
    void M5AtomS3Face::DrawEyeClose()
    {
        EyeClose->pushSprite(0, EYE_POS, offscreen_transport);
    }
    // -----------------------------------------------------------------------------
    void M5AtomS3Face::DrawEyeInt()
    {
        switch (_EyeMode)
        {
        case 1:
            EyeL0->pushSprite(0, EYE_POS, offscreen_transport);
            break;
        case 2:
            EyeR0->pushSprite(0, EYE_POS, offscreen_transport);
            break;
        case 0:
        default:
            EyeC0->pushSprite(0, EYE_POS, offscreen_transport);
            break;
        }
    }
    // -----------------------------------------------------------------------------
    void M5AtomS3Face::DrawEye()
    {
        switch (_EyeMode)
        {
        case 1:
            EyeL1->pushSprite(0, EYE_POS, offscreen_transport);
            break;
        case 2:
            EyeR1->pushSprite(0, EYE_POS, offscreen_transport);
            break;
        case 0:
        default:
            EyeC1->pushSprite(0, EYE_POS, offscreen_transport);
            break;
        }
    }
    // -----------------------------------------------------------------------------
    void M5AtomS3Face::DrawHoho()
    {
#if (offscreen_bit == 4)
        Comp->fillScreen(skin_number);
#else
        Comp->fillScreen(_SkinColor);
#endif
        Hoho->pushSprite(0, hoho_pos, offscreen_transport);
    }
    // -----------------------------------------------------------------------------
    void M5AtomS3Face::DrawBig()
    {
        if (_isSetup == false)
            return;
#if (offscreen_bit == 4)
        Comp->fillScreen(skin_number);
#else
        Comp->fillScreen(_SkinColor);
#endif
        Hoho->pushSprite(0, hoho_pos, offscreen_transport);
        MouthBig->pushSprite(mouth_xpos, mouth_ypos, offscreen_transport);
        EyeBig->pushSprite(0, EYE_POS, offscreen_transport);
    }
    // -----------------------------------------------------------------------------
    void M5AtomS3Face::DrawBigEye()
    {
        if (_isSetup == false)
            return;
#if (offscreen_bit == 4)
        Comp->fillScreen(skin_number);
#else
        Comp->fillScreen(_SkinColor);
#endif
        Hoho->pushSprite(0, hoho_pos, offscreen_transport);
        MouthOpen->pushSprite(mouth_xpos, mouth_ypos, offscreen_transport);
        EyeBig->pushSprite(0, EYE_POS, offscreen_transport);
        Redraw();
    }
    // -----------------------------------------------------------------------------

    void M5AtomS3Face::blink()
    {
        if (_isSetup == false)
            return;

        _EyeMode = random(0, 3);
        _MouthMode = random(0, 4);
        if (_EyeMode == 3)
        {
            DrawBigEye();
            return;
        }

        DrawHoho();
        DrawMouth();
        DrawEyeClose();
        Redraw();
        delay(50);

        DrawHoho();
        DrawMouth();
        DrawEyeInt();
        Redraw();
        delay(50);

        DrawHoho();
        DrawMouth();
        DrawEye();
        Redraw();
    }
}
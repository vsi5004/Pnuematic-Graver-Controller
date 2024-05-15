#include "TFTDisplay.h"
#include <SPI.h>
#include <TFT_eSPI.h>
#include "NotoSansBold15.h"
#include "NotoSansBold36.h"
#include "NotoSansMonoSCB20.h"

extern const uint8_t NotoSansBold15[];
extern const uint8_t NotoSansBold36[];
extern const uint8_t NotoSansMonoSCB20[];

// The font names are arrays references, thus must NOT be in quotes ""
#define AA_FONT_SMALL NotoSansBold15
#define AA_FONT_LARGE NotoSansBold36
#define AA_FONT_MONO NotoSansMonoSCB20

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite sprLabel = TFT_eSprite(&tft);
TFT_eSprite sprPedal = TFT_eSprite(&tft);

TFTDisplay::TFTDisplay()
{
    m_uiBGColor = TFT_BLACK;
    m_fTempValue = 0.0f;
    m_iPedalValue = 0;
}

TFTDisplay::~TFTDisplay()
{
    sprLabel.unloadFont();
    sprLabel.deleteSprite();
    sprPedal.unloadFont();
    sprPedal.deleteSprite();
}

void TFTDisplay::initDisplay()
{
    tft.begin();
    tft.setRotation(1);
    tft.fillScreen(m_uiBGColor);
    sprLabel.setColorDepth(16);
    sprPedal.setColorDepth(16);
    sprLabel.createSprite(100, 50);
    sprPedal.createSprite(100, 50);
    sprLabel.loadFont(AA_FONT_LARGE);
    sprPedal.loadFont(AA_FONT_LARGE);
    sprLabel.setTextDatum(MC_DATUM);
    sprPedal.setTextDatum(TC_DATUM);
    sprPedal.setTextColor(TFT_WHITE, m_uiBGColor);
    _drawLabels();
}

void TFTDisplay::updateDisplay(int iPedalValue, float iTempValue)
{
    if(iPedalValue != m_iPedalValue)
    {
        _updatePedalDisplay(iPedalValue);
        m_iPedalValue = iPedalValue;
    }
    if(iTempValue != m_fTempValue)
    {
        _updateTempDisplay(iTempValue);
        m_fTempValue = iTempValue;
    }
}

void TFTDisplay::_drawLabels()
{
    sprLabel.fillSprite(TFT_GREEN);
    sprLabel.setTextColor(TFT_BLACK, TFT_GREEN); // Set the font colour and the background colour
    sprLabel.drawString("Fits", 50, 25); // Make sure text fits in the Sprite!
    sprLabel.pushSprite(10, 10);         // Push to TFT screen coord 10, 10
}

void TFTDisplay::_updatePedalDisplay(int iDisplayValue)
{
    sprPedal.fillSprite(TFT_BLACK);
    sprPedal.drawNumber(iDisplayValue, 50, 10);
    sprPedal.pushSprite((tft.width() / 2) - (sprPedal.width() / 2), tft.height() - sprPedal.height());
}

void TFTDisplay::_updateTempDisplay(float iDisplayValue)
{
    sprPedal.fillSprite(TFT_BLACK);
    sprPedal.drawFloat(iDisplayValue, 2, 50, 10);
    sprPedal.pushSprite((tft.width() / 2) - (sprPedal.width() / 2), tft.height()/2);
}
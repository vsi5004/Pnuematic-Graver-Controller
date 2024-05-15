#include <cstdint>

class TFTDisplay
{
public:
    TFTDisplay();
    ~TFTDisplay();
    void initDisplay();
    void updateDisplay(int iPedalValue, float iTempValue);

private:
    uint16_t m_uiBGColor;
    float m_fTempValue;
    int m_iPedalValue;
    void _drawLabels();
    void _updatePedalDisplay(int iDisplayValue);
    void _updateTempDisplay(float iDisplayValue);
};
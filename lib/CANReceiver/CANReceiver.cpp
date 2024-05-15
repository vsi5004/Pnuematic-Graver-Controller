#include "CANReceiver.h"
#include <Arduino.h>
#include <mcp_can.h>
#include <SPI.h>

constexpr uint8_t CAN_TX_PIN = 11;
constexpr uint8_t CAN_RX_PIN = 12;
constexpr uint8_t CAN_CS_PIN = 13;
constexpr uint8_t CAN_SCK_PIN = 14;
constexpr uint8_t CAN_INT_PIN = 15;

static MCP_CAN *can;

long unsigned int m_rxId;
unsigned char m_len = 0;
unsigned char m_rxBuf[8];
char m_msgString[128];

CANReceiver::CANReceiver()
{
}

CANReceiver::~CANReceiver()
{
}

void CANReceiver::initCAN()
{
    SPI1.setCS(CAN_CS_PIN);
    SPI1.setSCK(CAN_SCK_PIN);
    SPI1.setTX(CAN_TX_PIN);
    SPI1.setRX(CAN_RX_PIN);
    can = new MCP_CAN(&SPI1, CAN_CS_PIN);
    
    if (can->begin(MCP_ANY, CAN_500KBPS, MCP_16MHZ) == CAN_OK)
    {
        Serial.println("MCP2515 Initialized Successfully!");
    }
    else
    {
        Serial.println("Error Initializing MCP2515...");
    }

    can->setMode(MCP_NORMAL);

    pinMode(CAN_INT_PIN, INPUT);
}

void CANReceiver::readCAN()
{
    //if (!digitalRead(CAN_INT_PIN))
    //{
        
        can->readMsgBuf(&m_rxId, &m_len, m_rxBuf);

        if ((m_rxId & 0x80000000) == 0x80000000)
        {
            sprintf(m_msgString, "Extended ID: 0x%.8lX  DLC: %1d  Data:", (m_rxId & 0x1FFFFFFF), m_len);
        }
        else
        {
            sprintf(m_msgString, "Standard ID: 0x%.3lX       DLC: %1d  Data:", m_rxId, m_len);
        }

        Serial.print(m_msgString);

        if ((m_rxId & 0x40000000) == 0x40000000)
        {
            sprintf(m_msgString, " REMOTE REQUEST FRAME");
            Serial.print(m_msgString);
        }
        else
        {
            for (byte i = 0; i < m_len; i++)
            {
                sprintf(m_msgString, " 0x%.2X", m_rxBuf[i]);
                Serial.print(m_msgString);
            }
        }

        Serial.println();
    //}
}

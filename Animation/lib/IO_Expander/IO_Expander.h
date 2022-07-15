#include <Wire.h>
#define IO_SCL      4
#define IO_SDA      16
/*
//Port A
#define TFT1_CS     1
#define LITE6       2
#define TFT6_CS     4
#define TFT5_CS     8
#define LITE5       16
#define LITE1       32
#define LITE4       64
#define TFT2_CS     128
//Port B
#define LITE2       208
#define TFT4_CS     202
#define LITE3       204
#define TFT3_CS     216
*/
//Port A
#define TFT1_CS     1
#define LITE6       2
#define TFT6_CS     3
#define TFT5_CS     4
#define POWER_ON    5
#define LITE1       6
#define TFT2_CS     7
#define LITE4       8
//Port B

#define LITE5       9
#define LITE2       10
#define TFT4_CS     11
#define LITE3       12
#define TFT3_CS     13
#define NC          14
#define LIS3DH_CS   15
#define VIBRATOR    16

#define PIN_OFF     0
#define PIN_ON      1

/*
#define ALL_PIN_A_OFF 0x00
#define ALL_PIN_A_ON  0xff
#define ALL_PIN_B_OFF 0x80
#define ALL_PIN_B_ON  0xa0
*/
class IO_Expander {
public:
  void begin(uint8_t addr);
  void begin(TwoWire *theWire = &Wire);

  void pinMode(uint8_t p, uint8_t d);
  void digitalWrite(uint8_t p, uint8_t d);
  void pullUp(uint8_t p, uint8_t d);
  uint8_t digitalRead(uint8_t p);

  void writeGPIOAB(uint16_t);
  uint16_t readGPIOAB();
  uint8_t readGPIO(uint8_t b);

  void setupInterrupts(uint8_t mirroring, uint8_t open, uint8_t polarity);
  void setupInterruptPin(uint8_t p, uint8_t mode);
  uint8_t getLastInterruptPin();
  uint8_t getLastInterruptPinValue();

private:
  uint8_t i2caddr;
  TwoWire *_wire; //!< pointer to a TwoWire object

  uint8_t bitForPin(uint8_t pin);
  uint8_t regForPin(uint8_t pin, uint8_t portAaddr, uint8_t portBaddr);

  uint8_t readRegister(uint8_t addr);
  void writeRegister(uint8_t addr, uint8_t value);

  /**
   * Utility private method to update a register associated with a pin (whether
   * port A/B) reads its value, updates the particular bit, and writes its
   * value.
   */
  void updateRegisterBit(uint8_t p, uint8_t pValue, uint8_t portAaddr,
                         uint8_t portBaddr);
};
#define MCP23017_ADDRESS 0x20 //!< MCP23017 Address

// registers
#define MCP23017_IODIRA 0x00   //!< I/O direction register A
#define MCP23017_IPOLA 0x02    //!< Input polarity port register A
#define MCP23017_GPINTENA 0x04 //!< Interrupt-on-change pins A
#define MCP23017_DEFVALA 0x06  //!< Default value register A
#define MCP23017_INTCONA 0x08  //!< Interrupt-on-change control register A
#define MCP23017_IOCONA 0x0A   //!< I/O expander configuration register A
#define MCP23017_GPPUA 0x0C    //!< GPIO pull-up resistor register A
#define MCP23017_INTFA 0x0E    //!< Interrupt flag register A
#define MCP23017_INTCAPA 0x10  //!< Interrupt captured value for port register A
#define MCP23017_GPIOA 0x12    //!< General purpose I/O port register A
#define MCP23017_OLATA 0x14    //!< Output latch register 0 A

#define MCP23017_IODIRB 0x01   //!< I/O direction register B
#define MCP23017_IPOLB 0x03    //!< Input polarity port register B
#define MCP23017_GPINTENB 0x05 //!< Interrupt-on-change pins B
#define MCP23017_DEFVALB 0x07  //!< Default value register B
#define MCP23017_INTCONB 0x09  //!< Interrupt-on-change control register B
#define MCP23017_IOCONB 0x0B   //!< I/O expander configuration register B
#define MCP23017_GPPUB 0x0D    //!< GPIO pull-up resistor register B
#define MCP23017_INTFB 0x0F    //!< Interrupt flag register B
#define MCP23017_INTCAPB 0x11  //!< Interrupt captured value for port register B
#define MCP23017_GPIOB 0x13    //!< General purpose I/O port register B
#define MCP23017_OLATB 0x15    //!< Output latch register 0 B

#define MCP23017_INT_ERR 255 //!< Interrupt error



/*
void IO_Expander_Init();
void IO_Expander(int Pin,bool ON_OFF);
void IO_Expander_ALL(int8_t Port,bool ON_OFF);
*/
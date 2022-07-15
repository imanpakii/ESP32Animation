#include <Wire.h>
#include <IO_Expander.h>
#include <Arduino.h>
/*!
 * Initializes the MCP23017 given its HW selected address, see datasheet for
 * Address selection.
 * @param addr Selected address
 */
void IO_Expander::begin(uint8_t addr) {
Wire.begin(IO_SDA ,IO_SCL); // wake up I2C bus
// set I/O pins to outputs
Wire.beginTransmission(0x27);
Wire.write(0x00); // IODIRA register
Wire.write(0x00); // set all of port A to outputs
Wire.write(0x00); // IODIRB register
Wire.write(0x00); // set all of port B to outputs
Wire.endTransmission();
digitalWrite(MCP23017_GPIOA,0x00);
digitalWrite(MCP23017_GPIOB,0x00);
}

/**
 * Sets the pin mode to either INPUT or OUTPUT
 * @param p Pin to set
 * @param d Mode to set the pin
 */
void IO_Expander::pinMode(volatile uint8_t p, uint8_t d) {
   volatile uint8_t read_LatchA=0;
   volatile uint8_t read_LatchB=0;
   volatile uint16_t byte2Word=0;
   volatile uint16_t bit2Word_High=0;
   volatile uint16_t bit2Word_Low=0;
   volatile uint16_t Word_High=0x0000;
   volatile uint16_t Word_Low=0x0000;
   uint8_t write_LatchA=0;
   uint8_t write_LatchB=0;

  // bool ON_OFF=0;
    read_LatchA = digitalRead(MCP23017_OLATA);
    read_LatchB = digitalRead(MCP23017_OLATB);
    
    byte2Word=(read_LatchB<<8) + read_LatchA;
    bit2Word_High=0x01<<(p-1);
    bit2Word_Low=~(0x01<<(p-1));
    
    Word_High=bit2Word_High|byte2Word;
    Word_Low=bit2Word_Low&byte2Word;
    
    if(d==HIGH) {write_LatchA=Word_High & 0xff;write_LatchB=Word_High>>8;}
    else {write_LatchA=Word_Low & 0xff;write_LatchB=Word_Low>>8;}
   digitalWrite(MCP23017_GPIOA,write_LatchA);
   digitalWrite(MCP23017_GPIOB,write_LatchB);

}
/**
 * Sets the pin mode to either INPUT or OUTPUT
 * @param p Pin to set
 * @param d Mode to set the pin
 */
void IO_Expander::digitalWrite(uint8_t p, uint8_t d) {
  
    Wire.begin(IO_SDA ,IO_SCL);
    Wire.beginTransmission(0x27);
    Wire.write(p); // IODIRB registe
    Wire.write(d); // set all of port B to outputs
    Wire.endTransmission();
}
/**
 * Sets the pin mode to either INPUT or OUTPUT
 * @param p read from Port 
 */
uint8_t IO_Expander:: digitalRead(uint8_t p){
Wire.beginTransmission(0x27);
Wire.write(p);
Wire.endTransmission();
Wire.requestFrom(0x27,2);
return(Wire.read());

}
#include <Arduino.h>
#include "SD.h"
#include <Cubipax_SD.h>
/*//SPI V
#define SD_CS         21
#define SD_MOSI      23
#define SD_MISO      19
#define SD_SCK       18
*/
///SPI H

//#define SD_CS         5
#define SD_MOSI      13
#define SD_MISO      5
#define SD_SCK       14

#define SD_CS_PIN 34

File myFile;
SPIClass SPISD(HSPI);

void SD_Init()
{
  //  SPIClass SPI2(HSPI);

    SPISD.begin(SD_SCK, SD_MISO, SD_MOSI);
    if (!SD.begin(SD_CS_PIN,SPISD)) {
    Serial.println(F("failed!"));
    return;
    }
    else Serial.println(F("SD read!"));
    

  /*  myFile = SD.open("/test.txt", "a"); //append to file
  if (myFile)
  {
    Serial.print("Writing to test.txt...");
    myFile.println("testing 1, 2, 3.");
    myFile.close();
    Serial.println("done.");
  }
  else
  {
    Serial.println("error opening test.txt to write");
  }
  
  
  
  myFile = SD.open("/test.txt", "r"); //read from file
  if (myFile)
  {
    Serial.println("test.txt:");
    String inString;  //need to use Strings because of the ESP32 webserver
    while (myFile.available())
    {
      inString += myFile.readString();
    }
    myFile.close();
    Serial.print(inString);
  }
  else
  {
    Serial.println("error opening test.txt to read");
  }

  while (!Serial);
  Serial.println();

  Serial.print("Initializing SD card...");
*/
  /*   pinMode(SD_CS, OUTPUT);
    digitalWrite(SD_CS, HIGH);
  //  SD.begin(SD_CS);
    SPISD.begin(SD_SCK, SD_MISO, SD_MOSI);
  if (!SD.begin(SD_CS_PIN,SPISD))
  {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
  myFile = SD.open("/test.txt", "a"); //append to file
  if (myFile)
  {
    Serial.print("Writing to test.txt...");
    myFile.println("testing 1, 2, 3.");
    myFile.close();
    Serial.println("done.");
  }
  else
  {
    Serial.println("error opening test.txt to write");
  }
  myFile = SD.open("/test.txt", "r"); //read from file
  if (myFile)
  {
    Serial.println("test.txt:");
    String inString;  //need to use Strings because of the ESP32 webserver
    while (myFile.available())
    {
      inString += myFile.readString();
    }
    myFile.close();
    Serial.print(inString);
  }
  else
  {
    Serial.println("error opening test.txt to read");
  }
  */
}
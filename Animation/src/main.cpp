#include <Arduino.h>
#include <Cubipax_SD.h>
#include <Adafruit_MCP23017.h>
#include <DisplayManager240240.h> // Graphics and font library for ST7735 driver chip
#include <IO_Expander.h>
#include <Cubipax_SD.h>
#include <Audio.h>
#include <Cubipax_Audio.h>
//#include <TJpg_Decoder.h>
#include <WiFi.h>
#include <SD_MMC.h>
#include <driver/i2s.h>
#include <SD.h>
#include <Arduino_ESP32SPI.h>
//#include <Arduino_Display.h>
#include <SPI.h>
//#include "MjpegClass.h"
// static MjpegClass mjpeg;
#define FPS 30
#define AUDIO_FILENAME "/44100_u16le.pcm"
#define MJPEG_FILENAME "/220_24fps.mjpeg"

#define MOVIE1_AUDIO "/Movie/movie1.pcm"
#define MOVIE1_MJPEG "/Movie/movie1.mjpeg"

#define MOVIE2_AUDIO "/Movie/movie2.pcm"
#define MOVIE2_MJPEG "/Movie/movie2.mjpeg"

#define MOVIE3_AUDIO "/Movie/movie3.pcm"
#define MOVIE3_MJPEG "/Movie/movie3.mjpeg"

#define MOVIE4_AUDIO "/Movie/movie4.pcm"
#define MOVIE4_MJPEG "/Movie/movie4.mjpeg"

#define MOVIE5_AUDIO "/Movie/movie5.pcm"
#define MOVIE5_MJPEG "/Movie/movie5.mjpeg"

#define MOVIE6_AUDIO "/Movie/movie6.pcm"
#define MOVIE6_MJPEG "/Movie/movie6.mjpeg"

#define EQULIZER_AUDIO "/Movie/Equlizer.pcm"
#define EQULIZER_MJPEG "/Movie/Equlizer.mjpeg"

#define EQULIZER2_AUDIO "/Movie/Equlizer2.pcm"
#define EQULIZER2_MJPEG "/Movie/Equlizer2.mjpeg"
#define MJPEG_BUFFER_SIZE (240 * 240 * 2 / 4)
int LoopCnt = 0;
#define SD_MOSI 13
#define SD_MISO 5
#define SD_SCK 14
//#define SD_CS 34
uint8_t Cube_Orientation;
IO_Expander IO;
//GYRO _gyro;
#include <Arduino_ESP32SPI.h>
#include <Arduino_ST7789.h>
Arduino_ESP32SPI *bus = new Arduino_ESP32SPI(27 /* DC */, 17 /* CS */, 18 /* SCK */, 23 /* MOSI */, 19 /* MISO */);
Arduino_ST7789 *gfx = new Arduino_ST7789(bus, 33 /* RST */, 2 /* rotation */, true /* IPS */, 240 /* width */, 240 /* height */, 0 /* col offset 1 */, 80 /* row offset 1 */);
#include "MjpegClass.h"
static MjpegClass mjpeg;
int next_frame = 0;
int skipped_frames = 0;
unsigned long total_read_audio = 0;
unsigned long total_play_audio = 0;
unsigned long total_read_video = 0;
unsigned long total_play_video = 0;
unsigned long total_remain = 0;
unsigned long start_ms, curr_ms, next_frame_ms;
void setup()
{
  Serial.begin(115200);
  IO.begin(0x27);
  SPIClass spi = SPIClass(HSPI);
  spi.begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS);
  // Init Video
  IO.pinMode(LIS3DH_CS,HIGH);
  //_gyro.Init();


  IO.pinMode(LITE1, HIGH);
  IO.pinMode(TFT1_CS, LOW);
  delay(10);
  IO.pinMode(LITE2, HIGH);
  IO.pinMode(TFT2_CS, LOW);
  delay(10);
  IO.pinMode(LITE3, HIGH);
  IO.pinMode(TFT3_CS, LOW);
  delay(10);
  IO.pinMode(LITE4, HIGH);
  IO.pinMode(TFT4_CS, LOW);
  delay(10);
  IO.pinMode(LITE5, HIGH);
  IO.pinMode(TFT5_CS, LOW);
  delay(10);
  IO.pinMode(LITE6, HIGH);
  IO.pinMode(TFT6_CS, LOW);
  delay(10);
  
  gfx->begin();
  gfx->fillScreen(BLACK);
  if (!SD.begin(SD_CS, spi, 80000000)) /* SPI bus mode */
  {
    Serial.println(F("ERROR: SD card mount failed!"));
    gfx->println(F("ERROR: SD card mount failed!"));
  }
  


else
  {
    // Init Audio
    i2s_config_t i2s_config_dac = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
        .sample_rate = 44100,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT,
        .communication_format = (i2s_comm_format_t)(I2S_COMM_FORMAT_PCM | I2S_COMM_FORMAT_I2S_MSB),
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1, // lowest interrupt priority
        .dma_buf_count = 9, // 30 FPS
        // .dma_buf_count = 9, // 15 FPS
        .dma_buf_len = 490,
        .use_apll = false,
        
    };
    Serial.printf("%p\n", &i2s_config_dac);
    if (i2s_driver_install(I2S_NUM_0, &i2s_config_dac, 0, NULL) != ESP_OK)
    {
      Serial.println(F("ERROR: Unable to install I2S drives!"));
      gfx->println(F("ERROR: Unable to install I2S drives!"));
    }
    else
    {
      static const i2s_pin_config_t pin_config = {
    .bck_io_num = 26,
    .ws_io_num = 15,
    .data_out_num = I2S_PIN_NO_CHANGE,
    .data_in_num = 25
};
      i2s_set_pin((i2s_port_t)0, &pin_config);
      i2s_set_dac_mode(I2S_DAC_CHANNEL_RIGHT_EN);
      i2s_zero_dma_buffer((i2s_port_t)0);

      File aFile = SD.open(EQULIZER2_AUDIO);
      // File aFile = SD_MMC.open(AUDIO_FILENAME);
      if (!aFile || aFile.isDirectory())
      {
        Serial.println(F("ERROR: Failed to open " AUDIO_FILENAME " file for reading!"));
        gfx->println(F("ERROR: Failed to open " AUDIO_FILENAME " file for reading!"));
      }
      else
      {
        File vFile = SD.open(EQULIZER2_MJPEG);
        // File vFile = SD_MMC.open(MJPEG_FILENAME);
        if (!vFile || vFile.isDirectory())
        {
          Serial.println(F("ERROR: Failed to open " MJPEG_FILENAME " file for reading"));
          gfx->println(F("ERROR: Failed to open " MJPEG_FILENAME " file for reading"));
        }
        else
        {
          uint8_t *aBuf = (uint8_t *)malloc(2940); // 30 FPS
          // uint8_t *aBuf = (uint8_t *)malloc(5880); // 15 FPS
          if (!aBuf)
          {
            Serial.println(F("aBuf malloc failed!"));
          }
          else
          {
            uint8_t *mjpeg_buf = (uint8_t *)malloc(MJPEG_BUFFER_SIZE);
            if (!mjpeg_buf)
            {
              Serial.println(F("mjpeg_buf malloc failed!"));
            }
            else
            {
              Serial.println(F("PCM audio MJPEG video start"));
              start_ms = millis();
              curr_ms = millis();
              mjpeg.setup(vFile, mjpeg_buf, gfx, true);
              next_frame_ms = start_ms + (++next_frame * 1000 / FPS);

              // prefetch first audio buffer
              aFile.read(aBuf, 980);
              i2s_write((i2s_port_t)0, (char *)aBuf, 980,(size_t *)1, 0);
              while (vFile.available() && aFile.available())
              {
                 // IO.pinMode(TFT1_CS, HIGH);   delay(5);
                 // IO.pinMode(LIS3DH_CS,LOW);  delay(5);
                 // _gyro.Tilt_Detection(&Cube_Orientation,true); 
               //     IO.pinMode(TFT1_CS, LOW); delay(5);
                 //   IO.pinMode(LIS3DH_CS,HIGH); delay(5);
                // Read audio
                aFile.read(aBuf, 2940); // 30 FPS
                // aFile.read(aBuf, 5880); // 15 FPS
                total_read_audio += millis() - curr_ms;
                curr_ms = millis();

                // Play audio
                i2s_write((i2s_port_t)0, (char *)aBuf, 980,(size_t *)1, 0);
                i2s_write((i2s_port_t)0, (char *)(aBuf + 980), 980,(size_t *)1, 0);
                i2s_write((i2s_port_t)0, (char *)(aBuf + 1960), 980,(size_t *)1, 0);
                // below for 15 FPS only
                // i2s_write_bytes((i2s_port_t)0, (char *)(aBuf + 2940), 980, 0);
                // i2s_write_bytes((i2s_port_t)0, (char *)(aBuf + 3920), 980, 0);
                // i2s_write_bytes((i2s_port_t)0, (char *)(aBuf + 4900), 980, 0);
                total_play_audio += millis() - curr_ms;
                curr_ms = millis();

                // Read video
                mjpeg.readMjpegBuf();
                total_read_video += millis() - curr_ms;
                curr_ms = millis();


                if (millis() < next_frame_ms) // check show frame or skip frame
                {
                  // Play video
                  mjpeg.drawJpg();
                  total_play_video += millis() - curr_ms;

                  int remain_ms = next_frame_ms - millis();
                  total_remain += remain_ms;
                  if (remain_ms > 0)
                  {
                    delay(remain_ms);
                  }
                }
                else
                {
                  ++skipped_frames;
                  Serial.println(F("Skip frame"));
                }
                curr_ms = millis();
                next_frame_ms = start_ms + (++next_frame * 1000 / FPS);
              }
              int time_used = millis() - start_ms;
              Serial.println(F("PCM audio MJPEG video end"));
              vFile.close();
              aFile.close();
              int played_frames = next_frame - 1 - skipped_frames;
              float fps = 1000.0 * played_frames / time_used;
              Serial.printf("Played frames: %d\n", played_frames);
              Serial.printf("Skipped frames: %d (%0.1f %%)\n", skipped_frames, 100.0 * skipped_frames / played_frames);
              Serial.printf("Time used: %d ms\n", time_used);
              Serial.printf("Expected FPS: %d\n", FPS);
              Serial.printf("Actual FPS: %0.1f\n", fps);
              Serial.printf("SD read PCM: %d ms (%0.1f %%)\n", total_read_audio, 100.0 * total_read_audio / time_used);
              Serial.printf("Play audio: %d ms (%0.1f %%)\n", total_play_audio, 100.0 * total_play_audio / time_used);
              Serial.printf("SD read MJPEG: %d ms (%0.1f %%)\n", total_read_video, 100.0 * total_read_video / time_used);
              Serial.printf("Play video: %d ms (%0.1f %%)\n", total_play_video, 100.0 * total_play_video / time_used);
              Serial.printf("Remain: %d ms (%0.1f %%)\n", total_remain, 100.0 * total_remain / time_used);

#define CHART_MARGIN 24
#define LEGEND_A_COLOR 0xE0C3
#define LEGEND_B_COLOR 0x33F7
#define LEGEND_C_COLOR 0x4D69
#define LEGEND_D_COLOR 0x9A74
#define LEGEND_E_COLOR 0xFBE0
#define LEGEND_F_COLOR 0xFFE6
#define LEGEND_G_COLOR 0xA2A5
              gfx->setCursor(0, 0);
              gfx->setTextColor(WHITE);
              gfx->printf("Played frames: %d\n", played_frames);
              gfx->printf("Skipped frames: %d (%0.1f %%)\n", skipped_frames, 100.0 * skipped_frames / played_frames);
              gfx->printf("Actual FPS: %0.1f\n\n", fps);
              int16_t r1 = ((gfx->height() - CHART_MARGIN - CHART_MARGIN) / 2);
              int16_t r2 = r1 / 2;
              int16_t cx = gfx->width() - gfx->height() + CHART_MARGIN + CHART_MARGIN - 1 + r1;
              int16_t cy = r1 + CHART_MARGIN;
              float arc_start = 0;
              float arc_end = max(2.0, 360.0 * total_read_audio / time_used);
              for (int i = arc_start + 1; i < arc_end; i += 2)
              {
                gfx->fillArc(cx, cy, r1, r2, arc_start - 90.0, i - 90.0, LEGEND_D_COLOR);
              }
              gfx->fillArc(cx, cy, r1, r2, arc_start - 90.0, arc_end - 90.0, LEGEND_D_COLOR);
              gfx->setTextColor(LEGEND_D_COLOR);
              gfx->printf("Read PCM:\n%0.1f %%\n", 100.0 * total_read_audio / time_used);

              arc_start = arc_end;
              arc_end += max(2.0, 360.0 * total_play_audio / time_used);
              for (int i = arc_start + 1; i < arc_end; i += 2)
              {
                gfx->fillArc(cx, cy, r1, r2, arc_start - 90.0, i - 90.0, LEGEND_C_COLOR);
              }
              gfx->fillArc(cx, cy, r1, r2, arc_start - 90.0, arc_end - 90.0, LEGEND_C_COLOR);
              gfx->setTextColor(LEGEND_C_COLOR);
              gfx->printf("Play audio:\n%0.1f %%\n", 100.0 * total_play_audio / time_used);

              arc_start = arc_end;
              arc_end += max(2.0, 360.0 * total_read_video / time_used);
              for (int i = arc_start + 1; i < arc_end; i += 2)
              {
                gfx->fillArc(cx, cy, r1, r2, arc_start - 90.0, i - 90.0, LEGEND_B_COLOR);
              }
              gfx->fillArc(cx, cy, r1, r2, arc_start - 90.0, arc_end - 90.0, LEGEND_B_COLOR);
              gfx->setTextColor(LEGEND_B_COLOR);
              gfx->printf("Read MJPEG:\n%0.1f %%\n", 100.0 * total_read_video / time_used);

              arc_start = arc_end;
              arc_end += max(2.0, 360.0 * total_play_video / time_used);
              for (int i = arc_start + 1; i < arc_end; i += 2)
              {
                gfx->fillArc(cx, cy, r1, 0, arc_start - 90.0, i - 90.0, LEGEND_A_COLOR);
              }
              gfx->fillArc(cx, cy, r1, 0, arc_start - 90.0, arc_end - 90.0, LEGEND_A_COLOR);
              gfx->setTextColor(LEGEND_A_COLOR);
              gfx->printf("Play video:\n%0.1f %%\n", 100.0 * total_play_video / time_used);

              i2s_driver_uninstall((i2s_port_t)0); //stop & destroy i2s driver
              // avoid unexpected output at audio pins
              pinMode(25, OUTPUT);
              digitalWrite(25, LOW);
              pinMode(26, OUTPUT);
              digitalWrite(26, LOW);
            }
          }
        }
      }
    }
  }
 /* else
  {
    // Init Audio
    i2s_config_t i2s_config_dac = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX | I2S_MODE_DAC_BUILT_IN),
        .sample_rate = 44100,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT,
        .communication_format = (i2s_comm_format_t)(I2S_COMM_FORMAT_PCM | I2S_COMM_FORMAT_I2S_MSB),
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1, // lowest interrupt priority
        .dma_buf_count = 5,                       // 30 FPS
        // .dma_buf_count = 9, // 15 FPS
        .dma_buf_len = 490,
        .use_apll = false,
    };
    Serial.printf("%p\n", &i2s_config_dac);
    if (i2s_driver_install(I2S_NUM_0, &i2s_config_dac, 0, NULL) != ESP_OK)
    {
      Serial.println(F("ERROR: Unable to install I2S drives!"));
      gfx->println(F("ERROR: Unable to install I2S drives!"));
    }
    else
    {
      Serial.println(F("SD card mounted"));
      gfx->println(F("SD card mounted!"));
      File vFile = SD.open(MJPEG_FILENAME);
      // File vFile = SD_MMC.open(MJPEG_FILENAME);
      if (!vFile || vFile.isDirectory())
      {
        Serial.println(F("ERROR: Failed to open " MJPEG_FILENAME " file for reading"));
        gfx->println(F("ERROR: Failed to open " MJPEG_FILENAME " file for reading"));
      }
      else
      {
        uint8_t *mjpeg_buf = (uint8_t *)malloc(MJPEG_BUFFER_SIZE);
        if (!mjpeg_buf)
        {
          Serial.println(F("mjpeg_buf malloc failed!"));
        }
        else
        {
          Serial.println(F("MJPEG video start"));
          mjpeg.setup(vFile, mjpeg_buf, gfx, true);
          // Read video
          while (mjpeg.readMjpegBuf())
          {
            // Play video
            mjpeg.drawJpg();
          }
          Serial.println(F("MJPEG video end"));
          vFile.close();
        }
      }
    }
  }
  */


  IO.pinMode(TFT1_CS, HIGH);
  IO.pinMode(TFT2_CS, HIGH);
  IO.pinMode(TFT3_CS, HIGH);
  IO.pinMode(TFT4_CS, HIGH);
  IO.pinMode(TFT5_CS, HIGH);
  IO.pinMode(TFT6_CS, HIGH);
}

void loop()
{
}

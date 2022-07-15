#include <Arduino.h>
#include <Audio.h>
#include <Cubipax_Audio.h>
#include <Cubipax_SD.h>

/*
// optional
void audio_info(const char *info){
    Serial.print("info        "); Serial.println(info);
}
void audio_id3data(const char *info){  //id3 metadata
    Serial.print("id3data     ");Serial.println(info);
}
void audio_eof_mp3(const char *info){  //end of file
    Serial.print("eof_mp3     ");Serial.println(info);
}
void audio_showstation(const char *info){
    Serial.print("station     ");Serial.println(info);
}
void audio_showstreaminfo(const char *info){
    Serial.print("streaminfo  ");Serial.println(info);
}
void audio_showstreamtitle(const char *info){
    Serial.print("streamtitle ");Serial.println(info);
}
void audio_bitrate(const char *info){
    Serial.print("bitrate     ");Serial.println(info);
}
void audio_commercial(const char *info){  //duration in sec
    Serial.print("commercial  ");Serial.println(info);
}
void audio_icyurl(const char *info){  //homepage
    Serial.print("icyurl      ");Serial.println(info);
}
void audio_lasthost(const char *info){  //stream URL played
    Serial.print("lasthost    ");Serial.println(info);
}
void audio_eof_speech(const char *info){
    Serial.print("eof_speech  ");Serial.println(info);
}
*/
Audio audio;
void AudioInit()
{
    audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
    audio.setVolume(21); // 0...21
    Serial.println(F("I2S Init"));
   // audio.connecttoFS(SD,"bensound-moose.mp3");
     //audio.loop();
}
void SpeachAlphabet(int Alphabet)
{
static bool Enter_Bit=0;
if(Alphabet==1000) Enter_Bit=0;
if(Enter_Bit==0){
    switch (Alphabet)
    {
    case 1:audio.connecttoFS(SD, "Ebi-Setarehaye-Sorbi.mp3");Serial.print("Ebi-Setarehaye-Sorbi.mp3");Enter_Bit=1;break;
    case 2:audio.connecttoFS(SD, "Ebi-Ey-Yar-Begoo.mp3");Serial.print("Ebi-Ey-Yar-Begoo.mp3");Enter_Bit=1;break;
    case 6:audio.connecttoFS(SD, "Ebi-Khanom-Gol.mp3");Serial.print("Ebi-Khanom-Gol.mp3");Enter_Bit=1;break;
    case 4:audio.connecttoFS(SD, "Ebi-Adat.mp3");Serial.print("Ebi-Adat.mp3");Enter_Bit=1;break;
    case 5:audio.connecttoFS(SD, "Ebi-Atal-Matal.mp3");Serial.print("Ebi-Atal-Matal.mp3");Enter_Bit=1;break;
    case 3:audio.connecttoFS(SD, "bensound-moose.mp3");Serial.print("bensound-moose.mp3");Enter_Bit=1;break;
    case 7:audio.connecttoFS(SD, "Ebi-Ey-Yar-Begoo.mp3");Serial.print("Ebi-Ey-Yar-Begoo.mp3");Enter_Bit=1;break;
    case 8:audio.connecttoFS(SD, "fairytales.mp3");Serial.print("fairytales.mp3");Enter_Bit=1;break;
    case 9:audio.connecttoFS(SD, "in-the-now.mp3");Serial.print("in-the-now.mp3");Enter_Bit=1;break;
    case 10:audio.connecttoFS(SD, "rebel.mp3");Serial.print("rebel.mp3");Enter_Bit=1;break;
    case 11:audio.connecttoFS(SD, "game-over.mp3");Serial.print("game-over.mp3");Enter_Bit=1;break;
    default:break;
    }
}
  
   AudioPlay();
}
void AudioPlay()
{
    audio.loop();
}
bool AudioCheck()
{
    bool temp=false;
    temp=audio.isRunning();
return (temp);
}
void AudioStop()
{
    audio.stopSong();
}

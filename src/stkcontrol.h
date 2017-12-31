#ifndef _STKTOY_
#define _STKTOY_

extern "C" void initialize();

extern "C" void pushOn(int instrumentId, int time, float freq, float amplitude);
extern "C" void pushOff(int instrumentId, int time, float amplitude);
extern "C" void pushFreq(int instrumentId, int time, float freq);
extern "C" void pushStop(int time);

extern "C" void writeWav(const char* fileName);

#endif
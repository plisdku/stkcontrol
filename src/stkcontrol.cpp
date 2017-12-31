#include <iostream>
#include <cstdint>
#include <cstdlib>

#include "stkcontrol.h"

#include "stk/RtAudio.h"

#include "stk/FileLoop.h"
#include "stk/FileWvOut.h"
#include "stk/Clarinet.h"
#include "stk/BlowHole.h"
#include "stk/Saxofony.h"
#include "stk/Flute.h"
#include "stk/Brass.h"
#include "stk/BlowBotl.h"
#include "stk/Bowed.h"
#include "stk/Plucked.h"
#include "stk/StifKarp.h"
#include "stk/Sitar.h"
#include "stk/Mandolin.h"
#include "stk/Rhodey.h"
#include "stk/Wurley.h"
#include "stk/TubeBell.h"
#include "stk/HevyMetl.h"
#include "stk/PercFlut.h"
#include "stk/BeeThree.h"
#include "stk/FMVoices.h"
#include "stk/VoicForm.h"
#include "stk/Moog.h"
#include "stk/Simple.h"
#include "stk/Drummer.h"
#include "stk/BandedWG.h"
#include "stk/Shakers.h"
#include "stk/ModalBar.h"
#include "stk/Mesh2D.h"
#include "stk/Resonate.h"
#include "stk/Whistle.h"

using namespace stk;

    // initialize();
    // blockUntilDone();
    // shutdown();

enum {kOn, kOff, kFreq, kStop};
struct Cmd
{
    int instr_;
    int type_;
    int time_;
    float arg0_;
    float arg1_;

    Cmd() : instr_(0), type_(kOn), time_(0), arg0_(0.0), arg1_(0.0)
    {
    }

    Cmd(int instrumentId, int type, int time, float arg0, float arg1) :
        instr_(instrumentId), type_(type), time_(time), arg0_(arg0), arg1_(arg1)
    {
    }

    static Cmd on(int instrumentId, int time, float freq, float amplitude)
    {
        return Cmd(instrumentId, kOn, time, freq, amplitude);
    }

    static Cmd off(int instrumentId, int time, float amplitude)
    {
        return Cmd(instrumentId, kOff, time, amplitude, 0.0);
    }

    static Cmd frequency(int instrumentId, int time, float freq)
    {
        return Cmd(instrumentId, kFreq, time, freq, 0.0);
    }

    static Cmd stop(int time)
    {
        return Cmd(0, kStop, time, 0.0, 0.0);
    }
};

std::vector<Instrmnt*> gInstruments;
std::vector<Cmd> gCommands;

void initialize()
{
    Stk::showWarnings(true);
    Stk::setSampleRate(44100.0);
    Stk::setRawwavePath("/Users/paul/Documents/Projects/Music/STK/stk/rawwaves/");

    gInstruments = std::vector<Instrmnt*>();
    gInstruments.push_back(new Clarinet());             // 0
    gInstruments.push_back(new BlowHole(440.0));
    gInstruments.push_back(new Saxofony(220.0));
    gInstruments.push_back(new Flute(220.0));
    gInstruments.push_back(new Brass());
    gInstruments.push_back(new BlowBotl());
    gInstruments.push_back(new Bowed());
    gInstruments.push_back(new Plucked());
    gInstruments.push_back(new StifKarp());
    gInstruments.push_back(new Sitar());
    gInstruments.push_back(new Mandolin(220.0));        // 10
    gInstruments.push_back(new Rhodey());
    gInstruments.push_back(new Wurley());
    gInstruments.push_back(new TubeBell());
    gInstruments.push_back(new HevyMetl());
    gInstruments.push_back(new PercFlut());
    gInstruments.push_back(new BeeThree());
    gInstruments.push_back(new FMVoices());
    gInstruments.push_back(new VoicForm());
    gInstruments.push_back(new Moog());
    gInstruments.push_back(new Simple());               // 20
    gInstruments.push_back(new Drummer());
    gInstruments.push_back(new BandedWG());
    gInstruments.push_back(new Shakers());
    gInstruments.push_back(new ModalBar());
    gInstruments.push_back(new Mesh2D(2, 2));
    gInstruments.push_back(new Resonate());
    gInstruments.push_back(new Whistle());

    // For reasons I have not yet determined, some of the instruments are on
    // after initialization.  Using noteOff() seems to have no effect on them.

    gInstruments[10]->noteOn(440.0, 0.00000001); // turn off the mandolin (lol?)
    gInstruments[27]->noteOn(440.0, 0.00000001); // turn off the whistle
    gInstruments[18]->noteOn(440.0, 0.00000001); // turn off the VoicForm
    
    gCommands = std::vector<Cmd>();
}


struct TickData
{
    int counter_;
    int cmdCounter_;
    std::vector<Cmd> *cmds_;
    std::vector<Instrmnt*> *instruments_;

    TickData() : counter_(0), cmdCounter_(0), cmds_(0L), instruments_(0L)
    {
    }
};

// Called automatically when the system needs a new buffer of audio samples
int tick(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
    double streamTime, RtAudioStreamStatus status, void *dataPointer)
{
    TickData *data = (TickData*) dataPointer;
    StkFloat *samples = (StkFloat*) outputBuffer;

    for (int ii = 0; ii < nBufferFrames; ii++)
    {
        // Handle command queue
        while (data->cmdCounter_ < data->cmds_->size() && data->counter_ >= data->cmds_->at(data->cmdCounter_).time_)
        {
            const Cmd & cmd = data->cmds_->at(data->cmdCounter_);
            Instrmnt* instrument = data->instruments_->at(cmd.instr_);
            switch (cmd.type_)
            {
                case kOn:
                    instrument->noteOn(cmd.arg0_, cmd.arg1_);
                    break;
                case kOff:
                    instrument->noteOff(cmd.arg0_);
                    break;
                case kFreq:
                    instrument->setFrequency(cmd.arg0_);
                    break;
                default:
                    break;
            }
            data->cmdCounter_++;
        }


        StkFloat sum = 0.0;
        for (int ii = 0; ii < gInstruments.size(); ii++)
        {
            sum += gInstruments[ii]->tick();
        }

        *samples++ = sum;

        data->counter_++;
    }

    return 0;
}



void pushOn(int instrumentId, int time, float freq, float amplitude)
{
    gCommands.push_back(Cmd::on(instrumentId, time, freq, amplitude));
}

void pushOff(int instrumentId, int time, float amplitude)
{
    gCommands.push_back(Cmd::off(instrumentId, time, amplitude));
}

void pushFreq(int instrumentId, int time, float freq)
{
    gCommands.push_back(Cmd::frequency(instrumentId, time, freq));
}

void pushStop(int time)
{
    gCommands.push_back(Cmd::stop(time));
}

void writeWav(const char* fileName)
{
    FileWvOut outputFile;
    outputFile.openFile(fileName, 1, FileWrite::FILE_WAV, Stk::STK_SINT16);

    int iCmd = 0;

    int maxSamples = 44100 * 3600;
    bool isDone = false;
    for (int ii = 0; ii < maxSamples && !isDone; ii++)
    {
        // Handle command queue
        while (iCmd < gCommands.size() && ii >= gCommands[iCmd].time_)
        {
            const Cmd & cmd = gCommands[iCmd];
            Instrmnt* instrument = gInstruments.at(cmd.instr_);
            switch (cmd.type_)
            {
                case kOn:
                    instrument->noteOn(cmd.arg0_, cmd.arg1_);
                    break;
                case kOff:
                    instrument->noteOff(cmd.arg0_);
                    break;
                case kFreq:
                    instrument->setFrequency(cmd.arg0_);
                    break;
                case kStop:
                    isDone = true;
                    break;
                default:
                    break;
            }
            iCmd++;
        }

        StkFloat sum = 0.0;
        for (int ii = 0; ii < gInstruments.size(); ii++)
        {
            sum += gInstruments[ii]->tick();
        }
        outputFile.tick(sum);
    }

    outputFile.closeFile();
}


// int main()
// {
//     std::cout << "Hello, world!\n";

//     initialize();

//     pushOn(0, 0, 440.0, 0.25);
//     pushOn(10, 22050, 550.0, 1.0);
//     pushOff(0, 44100, 0.5);
//     pushOff(10, 66150, 1.0);

//     bool isRealTime = true;
//     if (isRealTime)
//     {
//         TickData data;
//         data.cmds_ = &gCommands;
//         data.instruments_ = &gInstruments;

//         RtAudio dac;

//         RtAudio::StreamParameters parameters;
//         parameters.deviceId = dac.getDefaultOutputDevice();
//         parameters.nChannels = 1;
//         RtAudioFormat format = (sizeof(StkFloat)==8) ? RTAUDIO_FLOAT64 : RTAUDIO_FLOAT32;

//         unsigned int numBufferFrames = RT_BUFFER_SIZE;

//         dac.openStream(&parameters, NULL, format, (unsigned int)Stk::sampleRate(),
//             &numBufferFrames, &tick, (void*)&data );

//         dac.startStream();

//         Stk::sleep(100000);
//     }
//     else
//     {
//         writeWav();
//     }
// }

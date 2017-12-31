#ifndef _INSTRUMENTBANK_
#define _INSTRUMENTBANK_

#include <map>
#include <set>

#include "stk/Instrmnt.h"

class InstrumentBank
{
public:
    InstrumentBank();
    virtual ~InstrumentBank(); // don't need it I think

    template<class Instr>
    void init(int numInstruments);

    template<class Instr>
    void init(int numInstruments, float argument);

    float tick(); // calculate sounds and move inactive instruments around

    void noteOn(float freq, float amplitude);

    void noteOff(float freq, float amplitude);

private:
    int ringdownSamples_; // time to become EFFECTIVELY SILENT after noteOff
    int currentTick_;

    enum {kOn, kOff};
    struct Instrument
    {
        Instrument() : instrument_(0L), state_(kOff), offTime_(0) {}
        Instrument(stk::Instrmnt* instr) :
            instrument_(instr), state_(kOff), offTime_(0)
        {
            instrument_->noteOn(440.0, 1e-6); // some instruments are on by default...?!
        }

        virtual ~Instrument()
        {
            if (instrument_)
            {
                delete instrument_;
            }
        }

        stk::Instrmnt* instrument_;
        int state_;
        int offTime_;
    };

    std::map<double, Instrument*> active_;
    std::vector<Instrument*> inactive_;
};


template<class Instr>
void InstrumentBank::init(int numInstruments)
{
    for (int nn = 0; nn < numInstruments; nn++)
    {
        inactive_.push_back(new Instrument(new Instr()));
    }
}

template<class Instr>
void InstrumentBank::init(int numInstruments, float argument)
{
    for (int nn = 0; nn < numInstruments; nn++)
    {
        inactive_.push_back(new Instrument(new Instr(argument)));
    }
}

#endif
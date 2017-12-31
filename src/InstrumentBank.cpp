#include "InstrumentBank.h"


InstrumentBank::InstrumentBank() : ringdownSamples_(88200), currentTick_(0)
{
}

InstrumentBank::~InstrumentBank()
{
    // std::cerr << "Destroy\n";
    for (std::map<double, Instrument*>::iterator itr = active_.begin();
        itr != active_.end(); itr++)
    {
        delete itr->second;
    }

    for (int ii = 0; ii < inactive_.size(); ii++)
    {
        delete inactive_[ii];
    }
}

float InstrumentBank::tick()
{
    // std::cerr << "Tick " << currentTick_ << "\n";
    float sum = 0.0;

    std::set<double> inactiveFreqs;

    for (std::map<double, Instrument*>::iterator itr = active_.begin();
        itr != active_.end(); itr++)
    {
        sum += itr->second->instrument_->tick();

        if (itr->second->state_ == kOff &&
            itr->second->offTime_ + ringdownSamples_ < currentTick_)
        {
            // std::cerr << "Turn off!\n";
            inactiveFreqs.insert(itr->first);
        }
    }

    for (std::set<double>::iterator itr = inactiveFreqs.begin();
        itr != inactiveFreqs.end(); itr++)
    {
        double freq = *itr;
        std::cerr << "IB push inactive " << freq << ".\n";
        inactive_.push_back(active_[freq]);
        active_.erase(freq);
    }

    currentTick_++;

    return sum;
}

void InstrumentBank::noteOn(float freq, float amplitude)
{
    std::cerr << "IB On " << freq << ", " << amplitude << "\n";

    if (active_.count(freq))
    {
        // Instrument is already active.  Reuse it.

        Instrument* instr = active_[freq];

        instr->instrument_->noteOn(freq, amplitude);
        instr->state_ = kOn;
    }
    else if (inactive_.size() > 0)
    {
        // Need to fetch an available inactive instrument

        Instrument* instr = inactive_.back();
        inactive_.pop_back();

        instr->instrument_->noteOn(freq, amplitude);
        instr->state_ = kOn;

        active_[freq] = instr;
    }
    else
    {
        std::cerr << "IB no instruments available for frequency " << freq << ".\n";
    }
}

void InstrumentBank::noteOff(float freq, float amplitude)
{
    std::cerr << "IB off " << freq << ", " << amplitude << "\n";
    std::map<double, Instrument*>::iterator itr = active_.find(freq);

    if (itr != active_.end())
    {
        Instrument* instr = itr->second;
        instr->instrument_->noteOff(amplitude);
        instr->state_ = kOff;
        instr->offTime_ = currentTick_;
    }
    else
    {
        std::cerr << "IB no active instrument with frequency " << freq << ".\n";
    }
}





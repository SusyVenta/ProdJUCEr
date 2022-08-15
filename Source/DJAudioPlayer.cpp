/*
  ==============================================================================

    DJAudioPlayer.cpp
    Created: 10 Jul 2022 5:45:23pm
    Author:  ventafri

  ==============================================================================
*/

#include "DJAudioPlayer.h"

DJAudioPlayer::DJAudioPlayer(juce::AudioFormatManager& _formatManager)
    : formatManager(_formatManager)
{
    // Set default reverb settings
    reverbParameters.roomSize = 0;
    reverbParameters.damping = 0;
    reverbParameters.wetLevel = 0;
    reverbParameters.dryLevel = 1.0;
    reverbSource.setParameters(reverbParameters);
};

DJAudioPlayer::~DJAudioPlayer() {

};

void DJAudioPlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    reverbSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
};

void DJAudioPlayer::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    reverbSource.getNextAudioBlock(bufferToFill);
};

void DJAudioPlayer::releaseResources()
{
    transportSource.releaseResources();
    resampleSource.releaseResources();
    reverbSource.releaseResources();
};

void DJAudioPlayer::loadURL(juce::URL audioURL)
{
    // takes audio url input string, passes it to formatManager, and creates a Reader
    auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));
    // check if it successfully created the reader aka the file is readable
    if (reader != nullptr) 
    {
        std::unique_ptr<juce::AudioFormatReaderSource> newSource(new juce::AudioFormatReaderSource(reader, true));
        transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
        readerSource.reset(newSource.release());

    }
};

void DJAudioPlayer::setGain(double gain)
{
    if (gain <= 0 || gain > 1.0) {
        if (gain < 0)
        {
            // Set minimum to avoid error
            gain = 1; 
        }
    }
    else {
        transportSource.setGain(gain);
    }
};

void DJAudioPlayer::setSpeed(double ratio)
{
    if (ratio < 0 || ratio > 3.0 || ratio == 0) {
        if (ratio < 0)
        {
            // Set minimum to avoid error
            ratio = 1; 
        }
    }
    else {
        resampleSource.setResamplingRatio(ratio);
    }
};

void DJAudioPlayer::setPosition(double posInSecs)
{
    transportSource.setPosition(posInSecs);
};

void DJAudioPlayer::setPositionRelative(double pos)
{
    if (pos < 0 || pos > 1 || pos == 0) {
        if (pos < 0)
        {
            // Set minimum to avoid error
            pos = 0.01;
        }
    }
    else {
        double posInSecs = transportSource.getLengthInSeconds() * pos;
        setPosition(posInSecs);
    }
}

void DJAudioPlayer::start()
{
    transportSource.start();
};

void DJAudioPlayer::stop()
{
    transportSource.stop();
};


double DJAudioPlayer::getPositionRelative()
{
    // check for division by zero otherwise get error
    if (transportSource.getLengthInSeconds() != 0) {
        // if we dont divide, it will return the position in seconds which is not relative
        return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
    }
}


double DJAudioPlayer::getLengthInSeconds()
{
    return transportSource.getLengthInSeconds();
}


void DJAudioPlayer::setFreeze(float freezeAmt)
{
    if (freezeAmt >= 0 && freezeAmt <= 1.0)
    {
        reverbParameters.freezeMode = freezeAmt;
        reverbSource.setParameters(reverbParameters);
    }
}

void DJAudioPlayer::setWetLevel(float wetLevel)
{
    if (wetLevel >= 0 && wetLevel <= 1.0)
    {
        reverbParameters.wetLevel = wetLevel;
        reverbSource.setParameters(reverbParameters);
    }
}

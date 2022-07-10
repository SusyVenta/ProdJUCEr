/*
  ==============================================================================

    DJAudioPlayer.cpp
    Created: 10 Jul 2022 5:45:23pm
    Author:  ventafri

  ==============================================================================
*/

#include "DJAudioPlayer.h"

DJAudioPlayer::DJAudioPlayer() {};
DJAudioPlayer::~DJAudioPlayer() {};

void DJAudioPlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate) {
    formatManager.registerBasicFormats();
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
};

void DJAudioPlayer::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) {
    resampleSource.getNextAudioBlock(bufferToFill);
};

void DJAudioPlayer::releaseResources() {
    transportSource.releaseResources();
    resampleSource.releaseResources();
};

void DJAudioPlayer::loadURL(juce::URL audioURL) {
    auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));
    if (reader != nullptr) { // good file
        std::unique_ptr<juce::AudioFormatReaderSource> newSource(new juce::AudioFormatReaderSource(reader, true));
        transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
        readerSource.reset(newSource.release());
    };
};

void DJAudioPlayer::setGain(double gain) {
    if (gain < 0 || gain > 1) {
        DBG("DJAudioPlayer::setGain: gain should be between 0 and 1");
    }
    else {
        transportSource.setGain(gain);
    }
};

void DJAudioPlayer::setSpeed(double ratio) {
    if (ratio < 0 || ratio > 100.0) {
        DBG("DJAudioPlayer::setSpeed: ratio should be between 0 and 100");
    }
    else {
        resampleSource.setResamplingRatio(ratio);
    }
};

void DJAudioPlayer::setPosition(double posInSeconds) {
    transportSource.setPosition(posInSeconds);
};

void DJAudioPlayer::setRelativePosition(double pos) {
    if (pos < 0 || pos > 1) {
        DBG("DJAudioPlayer::setRelativePosition: pos should be between 0 and 1");
    }
    else {
        double posInSeconds = transportSource.getLengthInSeconds() * pos;
        setPosition(posInSeconds);
    }
};

void DJAudioPlayer::start() {
    transportSource.start();
};

void DJAudioPlayer::stop() {
    transportSource.stop();
};
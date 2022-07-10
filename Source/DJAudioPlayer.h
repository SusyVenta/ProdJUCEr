/*
  ==============================================================================

    DJAudioPlayer.h
    Created: 10 Jul 2022 5:45:23pm
    Author:  ventafri

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class DJAudioPlayer : public juce::AudioSource {
public:
    DJAudioPlayer();
    ~DJAudioPlayer();

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    void loadURL(juce::URL audioURL);
    void setGain(double gain);
    void setSpeed(double ratio);
    void setPosition(double posInSeconds);
    void setRelativePosition(double pos);

    void start();
    void stop();

private:
    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;

    juce::ResamplingAudioSource resampleSource{ &transportSource, false, 2 };
};

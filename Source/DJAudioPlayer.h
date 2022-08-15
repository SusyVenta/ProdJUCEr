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
    /**
     * Constructor
     *
     * @param _formatManager: object for keeping a list of available audio formats, and for 
                              deciding which one to use to open a given file.
     */
    DJAudioPlayer(juce::AudioFormatManager& _formatManager);

    /**
     * Destructor
     */
    ~DJAudioPlayer();

    /**
     * Pure virtual function.
     * Tells the source to prepare for playing. An AudioSource has two states: prepared and unprepared.
     * 
     * @param samplesPerBlockExpected: the number of samples that the source will be expected to supply each time its getNextAudioBlock() method is called
     * @param sampleRate: the sample rate that the output will be used at - this is needed by sources such as tone generators.
     */
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;

    /**
     * Pure virtual function.
     * Called repeatedly to fetch subsequent blocks of audio data.
     *
     * @param bufferToFill: buffer to fill with new audio data
     */
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    
    /**
     * Pure virtual function.
     * Allows the source to release anything it no longer needs after playback has stopped.
     */
    void releaseResources() override;

    /**
     * Loads file URL into a reader object. 
     * @param audioURL of the file to read
     */
    void loadURL(juce::URL audioURL);

    /**
     * Sets the volume output of the player.
     *
     * @param gain: 0 <= double <= 1
     */
    void setGain(double gain);

    /**
     * Controls the playback speed of the song. 
     * Controlled by a slider created in DeckGUI. 
     *
     * @param ratio: value to be used to set the resampleSource, i.e. playback speed.
     *               values are restricted between 0.2 and 2 from the slider. 
     */
    void setSpeed(double ratio);

    /**
     * Changes the current playback position in the source stream.
     *
     * @param posInSecs: the new playback position in seconds
     */
    void setPosition(double posInSecs);

    /**
     * Sets the track position relative to song length
     *
     * @param pos: the relative position
     */
    void setPositionRelative(double pos); 

    /**
     * Plays a loaded track
     */
    void start();

    /**
     * Stops a playing track
     */
    void stop();

    /** Get the relative position of the playhead */
    double getPositionRelative();

    /**
     * Gets track duration in seconds
     */
    double getLengthInSeconds();

    /** Sets the amount of reverb freeze */
    void setFreeze(float freezeAmt);

    /** Sets the amount of reverb wetLevel */
    void setWetLevel(float wetLevel);

private:
    juce::AudioFormatManager& formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;
    juce::ResamplingAudioSource resampleSource{ &transportSource, false, 2 };

    juce::ReverbAudioSource reverbSource{ &resampleSource, false };
    juce::Reverb::Parameters reverbParameters;
};

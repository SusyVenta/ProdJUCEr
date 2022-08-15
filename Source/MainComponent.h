#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "DeckGUI.h"
#include "PlaylistComponent.h"


class MainComponent : public juce::AudioAppComponent
{
public:
    /**
     * Constructor
     */
    MainComponent(); 

    /**
     * Destructor
     */
    ~MainComponent() override;

    /**
     * Tells the source to prepare for playing.
     * An AudioSource has two states: prepared and unprepared.
     * 
     * Pure virtual function
     *
     * @param samplesPerBlockExpected: the number of samples that the source will be expected 
     *                                 to supply each time its getNextAudioBlock() method is 
     *                                 called. This number may vary slightly, because it will 
     *                                 be dependent on audio hardware callbacks, and these 
     *                                 aren't guaranteed to always use a constant block size, 
     *                                 so the source should be able to cope with small variations.
     * @param sampleRate: the sample rate that the output will be used at - this is needed by sources such as tone generators.
     */
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;

    /**
     * Called repeatedly to fetch subsequent blocks of audio data.
     * After calling the prepareToPlay() method, this callback will be made each time the audio 
       playback hardware (or whatever other destination the audio data is going to) needs another block of data.
     *
     * Pure virtual function 
     * 
     * @param bufferToFill: buffer to fill with new audio data
     */
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;

    /**
     * Allows the source to release anything it no longer needs after playback has stopped.
     *
     * Pure virtual function
     */
    void releaseResources() override;

    /**
     * Components can override this method to draw their content.

        The paint() method gets called when a region of a component needs redrawing, 
        either because the component's repaint() method has been called, or because 
        something has happened on the screen that means a section of a window needs to 
        be redrawn.

        @param juce::Graphics& g: the graphics context that must be used to do the drawing operations.

     */
    void paint(juce::Graphics& g) override;

    /**
     * Called when this component's size has been changed.

        A component can implement this method to do things such as laying out its child 
        components when its width or height changes.
     */
    void resized() override;

private:
    // creates left deck
    DJAudioPlayer player1{ formatManager };
    DeckGUI deckGUI1{ 1, &player1 , formatManager , thumbCache };

    // creates right deck
    DJAudioPlayer player2{ formatManager };
    DeckGUI deckGUI2{ 2, &player2 , formatManager , thumbCache };

    juce::MixerAudioSource mixerSource;
    juce::AudioFormatManager formatManager;
    juce::AudioThumbnailCache thumbCache{ 100 }; // stores max 100 cached waveforms at a time

    DJAudioPlayer metadataParser{ formatManager };
    PlaylistComponent playlistComponent{ &deckGUI1, &deckGUI2, &metadataParser };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
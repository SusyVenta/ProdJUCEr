/*
  ==============================================================================

    DeckGUI.h
    Created: 10 Jul 2022 7:38:58pm
    Author:  ventafri

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"
#include "KnobsLookAndFeel.h"


class DeckGUI : public juce::Component,
    public juce::Button::Listener,
    public juce::Slider::Listener,
    public juce::FileDragAndDropTarget,
    public juce::Timer
{
public:
    /**
     * Constructor.

        @param _id: unique id for the object
        @param _player: unique player for the deck
        @param formatManagerToUse: shared AudioFormatManager object
        @param cacheToUse: shared AudioThumbnailCache
     */
    DeckGUI(int _id,
        DJAudioPlayer* player,
        juce::AudioFormatManager& formatManagerToUse,
        juce::AudioThumbnailCache& cacheToUse
    );

    /**
     * Destructor.
     */
    ~DeckGUI() override;

    /**
     * Components can override this method to draw their content.

        The paint() method gets called when a region of a component needs redrawing,
        either because the component's repaint() method has been called, or because
        something has happened on the screen that means a section of a window needs to
        be redrawn.

        @param juce::Graphics& g: the graphics context that must be used to do the drawing operations.

     */
    void paint(juce::Graphics&) override;

    /**
     * Called when this component's size has been changed.

        A component can implement this method to do things such as laying out its child
        components when its width or height changes.
     */
    void resized() override;

    /**
     * Checks which button is pressed and runs different functions.
     *
     * @param button: the button that was clicked
     */
    void buttonClicked(juce::Button* button);

    /**
     * Pure virtual function. Called when the slider's value is changed.
     *
     * @param slider: the slider which value was changed
     */
    void sliderValueChanged(juce::Slider* slider);

    /**
     * Pure virtual function. 
     * Callback to check whether this target is interested in the set of files being offered.
     *
     * @param files: the set of (absolute) pathnames of the files that the user is dragging
     */
    bool isInterestedInFileDrag(const juce::StringArray& files) override;
    
    /**
     * Pure virtual function.
     * Callback to indicate that the user has dropped the files onto this component.
     *
     * @param files: the set of (absolute) pathnames of the files that the user is dragging
     * @param x: the mouse x position, relative to this component
     * @param y: the mouse y position, relative to this component
     */
    void filesDropped(const juce::StringArray& files, int x, int y) override;
    
    /**
     * Pure virtual function.
     * The user-defined callback routine that actually gets called periodically.
     */
    void timerCallback() override;

private:

    int id;

    // to toggle between play and pause state of button
    bool isOn = false;

    juce::LookAndFeel_V4 sliderLookAndFeel; //slider styles
    
    juce::Slider speedSlider;
    juce::Slider posSlider;

    // custom knowbs style
    KnobsLookAndFeel knobsLookAndFeel;

    juce::Slider wetSlider;
    juce::Slider freezeSlider;
    juce::Slider volSlider;

    juce::SharedResourcePointer<juce::TooltipWindow> sharedTooltip;

    DJAudioPlayer* player;
    WaveformDisplay waveformDisplay;

    void loadFile(juce::URL audioURL);
    // allow access from PlaylistComponent to private members of this class DeckGUI
    friend class PlaylistComponent; 

    // play button
    juce::ImageButton playButton;
    juce::File playButtonFile = juce::File::getSpecialLocation
    (juce::File::SpecialLocationType::userDesktopDirectory).getChildFile("playpause.png");
    juce::Image playPauseImage = juce::ImageCache::getFromFile(playButtonFile);

    // fast forward button
    juce::ImageButton forwardButton;
    juce::File forwardButtonFile = juce::File::getSpecialLocation
    (juce::File::SpecialLocationType::userDesktopDirectory).getChildFile("forward-button.png");
    juce::Image forwardButtonImage = juce::ImageCache::getFromFile(forwardButtonFile);

    // fast rewind button
    juce::ImageButton rewindButton;
    juce::File rewindButtonFile = juce::File::getSpecialLocation
    (juce::File::SpecialLocationType::userDesktopDirectory).getChildFile("rewind.png");
    juce::Image rewindButtonImage = juce::ImageCache::getFromFile(rewindButtonFile);


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DeckGUI);
};
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


//==============================================================================
/*
*/
class DeckGUI : public juce::Component,
    public juce::Button::Listener,
    public juce::Slider::Listener,
    public juce::FileDragAndDropTarget,
    public juce::Timer
{
public:
    DeckGUI(int _id,
        DJAudioPlayer* player,
        juce::AudioFormatManager& formatManagerToUse,
        juce::AudioThumbnailCache& cacheToUse
    );
    ~DeckGUI() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void buttonClicked(juce::Button* button);
    void sliderValueChanged(juce::Slider* slider);
    bool isInterestedInFileDrag(const juce::StringArray& files) override;
    void filesDropped(const juce::StringArray& files, int x, int y) override;
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
    friend class PlaylistComponent; //allow access from PlaylistComponent to private members of this class DeckGUI

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
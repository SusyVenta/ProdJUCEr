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
#include "AxisModifier.h"
#include "PlaybackBar.h"


//==============================================================================
/*
*/
class DeckGUI : public juce::Component,
    public juce::Button::Listener,
    public juce::Slider::Listener,
    public juce::FileDragAndDropTarget,
    public juce::Timer,
    public AxisModifier::EventListener
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
    void AxisModifierValueChange(AxisModifier* AxisModifier) override;

private:

    int id;
    // to toggle between play and pause state of button
    bool isOn = false;

    juce::LookAndFeel_V4 sliderLookAndFeel; //slider styles
    PlaybackBar playbackBarLAF; //play progress bar look and feel

    juce::TextButton forwardButton{ "forward 0xE2 0x96 0xB6" };

    juce::TextButton reverseButton{ "reverse 0xE2 0x96 0xB6" };

    juce::TextButton playButton{ "PLAY 0xE2 0x96 0xB6" };

    juce::Label volLabel;
    juce::Label speedLabel;
    juce::Label posLabel;
    juce::Slider volSlider;
    juce::Slider speedSlider;
    juce::Slider posSlider;

    juce::Slider axisSlider;
    AxisModifier AxisModifier1;
    AxisModifier AxisModifier2;

    juce::SharedResourcePointer<juce::TooltipWindow> sharedTooltip;

    DJAudioPlayer* player;
    WaveformDisplay waveformDisplay;

    void loadFile(juce::URL audioURL);
    friend class PlaylistComponent; //allow access from PlaylistComponent to private members of this class DeckGUI

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DeckGUI);
};
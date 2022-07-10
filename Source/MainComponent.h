#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::AudioAppComponent,
                       public juce::Button::Listener,
                       public juce::Slider::Listener
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;

    /** implement button listener */
    void buttonClicked(juce::Button*) override;

    void sliderValueChanged(juce::Slider* slider) override;
private:
    //==============================================================================
    // Your private member variables go here...
    /** sound effects - dropdown menu */
    juce::Label effectsMenuTextLabel{ {}, "Sound effects" };
    juce::Font textFont{ 12.0f };
    juce::ComboBox effectsMenu;

    juce::TextButton playButton{"PLAY"};
    juce::TextButton stopButton{ "STOP" };
    juce::Slider gainSlider;

    juce::Random rand; // for random sampling aka white noise

    /* phase var. So we could remember where we were in the synthesised waveform between calls to getNextAudioBlock. 
    This allows us to avoid discontinuities*/
    double phase;
    double dphase; // change in phase
    double gain; 

    bool playing;
    std::string chosenEffect; 

    void effectsMenuChanged();


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};

/*
  ==============================================================================

    DeckGUI.cpp
    Created: 10 Jul 2022 7:38:58pm
    Author:  ventafri

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DeckGUI.h"


DeckGUI::DeckGUI(int _id,
    DJAudioPlayer* _player,
    juce::AudioFormatManager& formatManagerToUse,
    juce::AudioThumbnailCache& cacheToUse
) : id(_id),
    player(_player),
    waveformDisplay(formatManagerToUse, cacheToUse)
{
    addAndMakeVisible(waveformDisplay);
    addAndMakeVisible(wetSlider);
    addAndMakeVisible(freezeSlider);
    addAndMakeVisible(volSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(posSlider);
    addAndMakeVisible(playButton);
    addAndMakeVisible(forwardButton);
    addAndMakeVisible(rewindButton);

    playButton.addListener(this);
    forwardButton.addListener(this);
    rewindButton.addListener(this);
    volSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);
    wetSlider.addListener(this);
    freezeSlider.addListener(this);

    // track position slider
    sliderLookAndFeel.setColour(juce::Slider::thumbColourId, juce::Colours::coral); // set slider control color
    posSlider.setLookAndFeel(&sliderLookAndFeel);
    posSlider.setRange(0.0, 1.0);
    posSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 200, 20);
    posSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    posSlider.setTooltip("Drag slider to control track position");

    // vol slider
    volSlider.setLookAndFeel(&knobsLookAndFeel);
    volSlider.setSliderStyle(juce::Slider::Rotary);
    volSlider.setTextBoxStyle(juce::Slider::TextBoxAbove, true, 80, 20);
    volSlider.setTooltip("Turn knowb to control volume");
    volSlider.setTextValueSuffix(" Vol.");

    double vol = 0.5;
    volSlider.setValue(vol);
    volSlider.setRange(0.0, 1.0);
    volSlider.setNumDecimalPlacesToDisplay(2);

    // speed slider
    speedSlider.setLookAndFeel(&knobsLookAndFeel);
    speedSlider.setSliderStyle(juce::Slider::Rotary);
    speedSlider.setTextBoxStyle(juce::Slider::TextBoxAbove, true, 80, 20);
    speedSlider.setTooltip("Turn knowb to control speed");
    speedSlider.setTextValueSuffix(" Speed");

    double speed = 1.0;
    speedSlider.setValue(speed);
    speedSlider.setRange(0.2, 2.0);
    speedSlider.setNumDecimalPlacesToDisplay(2);

    // wet slider
    wetSlider.setLookAndFeel(&knobsLookAndFeel);
    wetSlider.setSliderStyle(juce::Slider::Rotary);
    wetSlider.setTextBoxStyle(juce::Slider::TextBoxAbove, true, 80, 20);
    wetSlider.setTooltip("Turn knowb to control wet level");
    wetSlider.setTextValueSuffix(" Wet");

    double wet = 0.5;
    wetSlider.setValue(wet);
    wetSlider.setRange(0.0, 1.0);
    wetSlider.setNumDecimalPlacesToDisplay(2);

    // freeze slider
    freezeSlider.setLookAndFeel(&knobsLookAndFeel);
    freezeSlider.setSliderStyle(juce::Slider::Rotary);
    freezeSlider.setTextBoxStyle(juce::Slider::TextBoxAbove, true, 80, 20);
    freezeSlider.setTooltip("Turn knowb to control freeze level");
    freezeSlider.setTextValueSuffix(" Freeze");

    double freeze = 0.0;
    freezeSlider.setValue(freeze);
    freezeSlider.setRange(0.0, 1.0);
    freezeSlider.setNumDecimalPlacesToDisplay(2);

    // track controls - button images
    playButton.setImages(true, true, true,
        playPauseImage, 1.0f, juce::Colours::coral,
        playPauseImage, 1.0f, juce::Colour(0x33000000),
        playPauseImage, 1.0f, juce::Colour(0x55000000));

    forwardButton.setImages(true, true, true,
        forwardButtonImage, 1.0f, juce::Colours::coral,
        forwardButtonImage, 1.0f, juce::Colour(0x33000000),
        forwardButtonImage, 1.0f, juce::Colour(0x55000000));

    rewindButton.setImages(true, true, true,
        rewindButtonImage, 1.0f, juce::Colours::coral,
        rewindButtonImage, 1.0f, juce::Colour(0x33000000),
        rewindButtonImage, 1.0f, juce::Colour(0x55000000));

    // every 300 miliseconds
    startTimer(300);
}

DeckGUI::~DeckGUI()
{
    stopTimer();
}

void DeckGUI::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));   // clear the background
    g.setColour(juce::Colours::coral);
    g.setFont(14.0f);
}

void DeckGUI::resized()
{
    double rowH = getHeight() / 20;
    waveformDisplay.setBounds(0, 0, getWidth(), 4 * rowH);
    posSlider.setBounds(0, 4 * rowH, getWidth(), rowH);
    
    wetSlider.setBounds(0, 6 * rowH, getWidth() / 2,4 * rowH);
    freezeSlider.setBounds(getWidth() / 2, 6 * rowH, getWidth() / 2, 4 * rowH);
    volSlider.setBounds(0, 11 * rowH, getWidth() / 2, 4 * rowH);
    speedSlider.setBounds(getWidth() / 2, 11 * rowH, getWidth() / 2, 4 * rowH);

    rewindButton.setBounds(0, 16.5 * rowH, getWidth() / 4, 3 * rowH);
    playButton.setBounds(getWidth() / 4, 16.5 * rowH, 2 * getWidth() / 4, 3 * rowH);
    forwardButton.setBounds(3 * getWidth() / 4, 16.5 * rowH, getWidth() / 4, 3 * rowH);
}

void DeckGUI::buttonClicked(juce::Button* button)
{
    if (button == &playButton)
    {
        
        isOn = !isOn;
        if (isOn)
        {
            player->start();
        }
        else if (!isOn) {
            player->stop();
        }

        playButton.setToggleState(isOn, juce::NotificationType::dontSendNotification);
        repaint();
    }

    else if (button == &forwardButton)
    {
        if (player->getPositionRelative() < 0.95)
        {
            player->setPositionRelative(player->getPositionRelative() + 0.05);
        }
    }
    else if (button == &rewindButton)
    {
        // Only allow if song has been playing long ehough
        if (player->getPositionRelative() > 0.05)
        {
            player->setPositionRelative(player->getPositionRelative() - 0.05);
        }
    }
}


void DeckGUI::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volSlider)
    {
        player->setGain(slider->getValue());
    }
    if (slider == &speedSlider)
    {
        player->setSpeed(slider->getValue());
    }
    if (slider == &posSlider)
    {
        player->setPositionRelative(slider->getValue());
    }
    if (slider == &wetSlider)
    {
        player->setWetLevel(slider->getValue());
    }
    if (slider == &freezeSlider)
    {
        player->setFreeze(slider->getValue());
    }
}

bool DeckGUI::isInterestedInFileDrag(const juce::StringArray& files)
{
    return true;
}

void DeckGUI::filesDropped(const juce::StringArray& files, int x, int y)
{
    if (files.size() == 1)
    {
        player->loadURL(juce::URL{ juce::File{ files[0] } });
    }
}

void DeckGUI::timerCallback()
{
    waveformDisplay.setPositionRelative(player->getPositionRelative());
    posSlider.setValue(player->getPositionRelative());
}

void DeckGUI::loadFile(juce::URL audioURL)
{
    player->loadURL(audioURL);
    waveformDisplay.loadURL(audioURL);

}

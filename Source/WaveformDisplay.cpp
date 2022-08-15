/*
  ==============================================================================

    WaveformDisplay.cpp
    Created: 19 Jul 2022 7:39:01pm
    Author:  ventafri

  ==============================================================================
*/

#include "WaveformDisplay.h"
#include <JuceHeader.h>


WaveformDisplay::WaveformDisplay(juce::AudioFormatManager& formatManagerToUse,
    juce::AudioThumbnailCache& cacheToUse) :  
    audioThumb(1000, formatManagerToUse, cacheToUse), // 1000 points to build waveplot aka downsampling
    fileLoaded(false),
    position(0)
{
    audioThumb.addChangeListener(this);
}

WaveformDisplay::~WaveformDisplay()
{
}

void WaveformDisplay::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour(juce::Colours::coral); //colour of the wave
    g.drawRect(getLocalBounds(), 1);   // draw an outline around the component
    g.setColour(juce::Colours::coral); // colour of box outline

    if (fileLoaded) {
        audioThumb.drawChannel(g,   //graphics
            getLocalBounds(),  //size where to draw
            0,   // start time
            audioThumb.getTotalLength(),  //end time
            0,  // channel
            1.0f);  //vertical zoom factor 

        g.setColour(juce::Colours::coral);
        if (position > 0 && getWidth() > 0) {
            g.drawRect(position * getWidth(), 0, getWidth() / 20, getHeight());
            //check for this to avoid errors
            DBG("Nothing negative in WaveformDisplay::paint");
        }
        else if (position < 0 && getWidth() > 0) {

            DBG("position is negative  in WaveformDisplay::paint ");
        }
        else if (position > 0 && getWidth() < 0) {
            DBG("getWidth() is negative  in WaveformDisplay::paint");
        }
        else {
            //avoid exception
            g.drawRect(0, 0, getWidth() / 20, getHeight());
            DBG("both are negative  in WaveformDisplay::paint");
        }
    }
    else {
        g.setFont(20.0f);
        g.drawText("Add a track!", getLocalBounds(),
            juce::Justification::centred, true);   // draw some placeholder text
    }
}

void WaveformDisplay::resized()
{
    // Nothing to add here.
}

void WaveformDisplay::loadURL(juce::URL audioURL)
{
    audioThumb.clear(); //clear previous audio
    //check if the new audio file is loaded successfully
    fileLoaded = audioThumb.setSource(new juce::URLInputSource(audioURL));

    if (fileLoaded) {
        DBG("File was loaded successfully");
        // paint waveform 
        repaint();
    }
    else {
        DBG("File was not loaded");
    }
}

void WaveformDisplay::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    repaint();
}

void WaveformDisplay::setPositionRelative(double pos)
{
    //only repaint if the position has changed
    if (pos != position) {
        position = pos; 
        repaint();
    }
}
/*
  ==============================================================================

    WaveformDisplay.h
    Created: 19 Jul 2022 7:39:01pm
    Author:  ventafri

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>


class WaveformDisplay : public juce::Component,
    public juce::ChangeListener
{
public:
    /**
     * Constructor. draws a waveform based on the audio file's properties.
     *
     * @param formatManagerToUse: An instance of the juce::AudioFormatManager to use for this
     * @param cacheToUse: The current cache loaded from the audio file
     */
    WaveformDisplay(juce::AudioFormatManager& formatManagerToUse,
        juce::AudioThumbnailCache& cacheToUse);

    /**
     * Destructor.
     */
    ~WaveformDisplay() override;

    /**
     * Paints colours, fonts, background. Evrything that can be done with the Graphics class.
     */
    void paint(juce::Graphics&) override;

    /**
     * Sets the bounds and sizes of each component inside PlaylistComponent.
     */
    void resized() override;

    /**
     * Repaints.
     * 
     * @param source: Holds a list of ChangeListeners, and sends messages to them when instructed. 
     */
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;

    /**
     * Loads the URL that is passed to this function and uses it to paint the waveform.
     * @param audioURL of the file to display waveform of
     */
    void loadURL(juce::URL audioURL);

    /**
     * Changes the relative position of the playhead, i.e. the waveform rectangle which tracks the position of current song.
     *
     * @param pos: the relative position
     */
    void setPositionRelative(double pos);

private:
    juce::AudioThumbnail audioThumb;
    bool fileLoaded;
    double position;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveformDisplay)
};
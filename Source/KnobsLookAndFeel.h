/*
  ==============================================================================

    KnobsLookAndFeel.h
    Created: 14 Aug 2022 2:01:51pm
    Author:  ventafri

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

/** Defines custom style for knobs */
class KnobsLookAndFeel : public juce::LookAndFeel_V4
{
public:
    KnobsLookAndFeel();

    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
        float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider) override;

private:
    juce::Image img1;

};
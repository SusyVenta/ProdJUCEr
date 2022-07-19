/*
  ==============================================================================

    PlaybackBar.h
    Created: 19 Jul 2022 7:41:39pm
    Author:  ventafri

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class PlaybackBar : public juce::LookAndFeel_V4 {
public:
	void drawLinearSlider(juce::Graphics& g,
		int x,
		int y,
		int width,
		int height,
		float sliderPos,
		float minSliderPos,
		float maxSliderPos,
		const juce::Slider::SliderStyle style,
		juce::Slider& slider)
		override;
};
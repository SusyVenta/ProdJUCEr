/*
  ==============================================================================

    Song.h
    Created: 19 Jul 2022 7:38:36pm
    Author:  ventafri

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class Song
{
public:
    Song(juce::File _file);

    juce::File file;
    juce::URL URL;

    //will be the name of the file
    juce::String songname;

    //will show how long the song is
    juce::String playtime;

    // compare Song objects by their name
    bool operator==(const juce::String& other) const;
};
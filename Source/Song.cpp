/*
  ==============================================================================

    Song.cpp
    Created: 19 Jul 2022 7:38:36pm
    Author:  ventafri

  ==============================================================================
*/

#include "Song.h"
#include <filesystem>


Song::Song(juce::File _file) : file(_file),
                               songName(_file.getFileNameWithoutExtension()),
                               URL(juce::URL{ _file })
{
    DBG("Added a song with name: " << songName);
}


bool Song::operator==(const juce::String& otherSongName) const
{
    return songName == otherSongName;
}
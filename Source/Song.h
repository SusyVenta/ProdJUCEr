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
    /**
     * The Song class is used to create song object, characterized by having a track title and
     * track duration.
     *
     * Initializes the song name as the file name without extension
     *
     * @param _file: the song file read from disk.
     */
    Song(juce::File _file);

    juce::File file;
    juce::URL URL;

    // same as file name
    juce::String songName;

    juce::String trackDuration;

    /**
     * Compares names of different Song objects
     *
     * @param otherSongName: the file name of a song
     * @returns True or False
     */
    bool operator==(const juce::String& other) const;
};
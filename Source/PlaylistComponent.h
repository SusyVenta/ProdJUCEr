/*
  ==============================================================================

    PlaylistComponent.h
    Created: 19 Jul 2022 7:37:32pm
    Author:  ventafri

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Song.h" // be able to instantiate a Song object and create a vector of Song objects called songs
#include <vector>
#include <string>
#include <fstream> // have access to files
#include "DeckGUI.h" // to add songs to Deck1 or Deck2
#include "DJAudioPlayer.h" //have access to loadURL() func

//==============================================================================
/*
*/
class PlaylistComponent : public juce::Component,
    public juce::TableListBoxModel,
    public juce::Button::Listener
{
public:
    // add the two Decks in the constructor and the DJAudioPlayer so that we can access their properties and functions
    PlaylistComponent(DeckGUI* _deckGUI1,
        DeckGUI* _deckGUI2,
        DJAudioPlayer* _playerForParsingMetaData
    );

    ~PlaylistComponent() override;

    void paint(juce::Graphics&) override;
    void resized() override;

    // TableListBoxModel virtual functions - pure minimum to inherit the class is to override those functions
    int getNumRows() override;
    void paintRowBackground(juce::Graphics&,
        int rowNumber,
        int width,
        int height,
        bool rowIsSelected) override;

    void paintCell(juce::Graphics&,
        int rowNumber,
        int columnId,
        int width,
        int height,
        bool rowIsSelected) override;
    // ------------------------------------------------------
    juce::Component* refreshComponentForCell(int rowNumber,
        int columnId,
        bool isRowSelected,
        Component* existingComponentToUpdate
    );

    /**
     * Checks what button is clicked and calls different functions accordingly.
     *
     * @param button: the button that was clicked
     */
    void buttonClicked(juce::Button* button) override;

private:
    juce::TableListBox tableComponent;

    //list of songs
    std::vector<Song> songs;

    //have private access to these once they are instantiated from the constructor
    DeckGUI* deckGUI1;
    DeckGUI* deckGUI2;
    DJAudioPlayer* metadataParser;

    //GUI components
    juce::TextButton importSongsButton{ "IMPORT SONGS" };
    juce::TextEditor searchBox;
    juce::TableListBox playlist;
    juce::Label decksLabel;
    juce::TextButton addToPlayer1Button{ "ADD TO LEFT" };
    juce::TextButton addToPlayer2Button{ "ADD TO RIGHT" };

    //playlist functionality
    juce::String getLength(juce::URL audioURL);
    juce::String secondsToMinutes(double seconds);

    /**
     * When the user closes the app, it saves a .csv file containing all songs added to the playlist.
     *
     * @param button: the button that was clicked
     */
    void savePlaylist();
    void loadPlaylist();
    void deleteFromPlaylist(int id);
    void importToPlaylist();
    void loadInPlayer(DeckGUI* deckGUI);

    //search functionality
    void searchPlaylist(juce::String query);
    int whereInPlaylist(juce::String query);
    bool isInPlaylist(juce::String fileNameWithoutExtension);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlaylistComponent);
};
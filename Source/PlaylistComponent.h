/*
  ==============================================================================

    PlaylistComponent.h
    Created: 19 Jul 2022 7:37:32pm
    Author:  ventafri

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Song.h" 
#include <vector>
#include <string>
#include <fstream> 
#include "DeckGUI.h" 
#include "DJAudioPlayer.h" 


class PlaylistComponent : public juce::Component,
    public juce::TableListBoxModel,
    public juce::Button::Listener
{
public:
    /**
     * Add the two Decks in the constructor and the DJAudioPlayer so that we can access 
     their properties and functions

     @param _deckGUI1: DeckGUI object (left)
     @param _deckGUI2: DeckGUI object (right)
     @param _playerForParsingMetaData: An instance of the DJAudioPlayer to process metadata. A DJAudioPlayer object to use to parse metadata of files.
     */
    PlaylistComponent(DeckGUI* _deckGUI1,
        DeckGUI* _deckGUI2,
        DJAudioPlayer* _playerForParsingMetaData
    );

    ~PlaylistComponent() override;

    /**
     * Paints colours, fonts, background. Evrything that can be done with the Graphics class.
     */
    void paint(juce::Graphics&) override;

    /**
     * Sets the bounds and sizes of each component inside PlaylistComponent.
     */
    void resized() override;
    
    /**
     * @returns: an integer corresponding to the number of songs in the playlist.
     */
    int getNumRows() override;

    /**
     * Set colours for the TableListBox that contains the songs information.
     * 
     * @param juce::Graphics&: Graphics class
     * @param rowNumber: the playlist space on the GUI is divided into rows. This indicates which row. 
     * @param columnId: the playlist space on the GUI is divided into columns. This indicates which column. 
     * @param width: width of the cell
     * @param height: height of the cell
     * @param rowIsSelected: bool stating if the user selected the cell
     */
    void paintRowBackground(
        juce::Graphics&,
        int rowNumber,
        int width,
        int height,
        bool rowIsSelected) override;

    /**
     * Fills each cell in the TableListBox with content and styles it.
     * 
     * @param juce::Graphics&: Graphics class
     * @param rowNumber: the playlist space on the GUI is divided into rows. This indicates which row. 
     * @param columnId: the playlist space on the GUI is divided into columns. This indicates which column. 
     * @param width: width of the cell
     * @param height: height of the cell
     * @param rowIsSelected: bool stating if the user selected the cell
     */
    void paintCell(
        juce::Graphics&,
        int rowNumber,
        int columnId,
        int width,
        int height,
        bool rowIsSelected) override;


    /**
     * Runs every time content is added or removed to TableListBox and refreshes the cells' content.
     *
     * @param rowNumber: the playlist space on the GUI is divided into rows. This indicates which row. 
     * @param columnId: the playlist space on the GUI is divided into columns. This indicates which column. 
     * @param isRowSelected: True if the user has selected the row, else False
     * @param existingComponentToUpdate: the component that needs to be updated. 
     * 
     * @returns: the updated Juce component
     */
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

    // the playlist
    std::vector<Song> songs;

    // we have private access to these once they are instantiated from the constructor
    DeckGUI* deckGUI1;
    DeckGUI* deckGUI2;
    DJAudioPlayer* metadataParser;

    // GUI components
    juce::TextButton importSongsButton{ "IMPORT SONGS" };
    juce::TextEditor searchBox;
    juce::TableListBox playlist;
    juce::Label decksLabel;
    juce::TextButton addSongToLeftDeckButton{ "ADD TO LEFT" };
    juce::TextButton addSongToRightDeckButton{ "ADD TO RIGHT" };


    /**
     * Loads the audio file from URL, checks duration in seconds, converts it to minutes, and returns duration.
     *
     * @param juce::URL audioURL: URL for an audio file
     * @returns: duration in minutes of the song, in string format.
     */
    juce::String getLength(juce::URL audioURL);

    /**
     * Converts seconds to minutes
     *
     * @param seconds: duration of a song in seconds
     * @returns: duration of a song in minutes, in string format
     */
    juce::String secondsToMinutes(double seconds);

    /**
     * When the user closes the app, it saves a .csv file containing all songs added to the playlist.
     *
     * @param button: the button that was clicked
     */
    void savePlaylist();

    /**
     * Checks if there exists a .csv file containing the playlist. 
     * If file exists, create a Song object for each song contained in it and add this Song object to the 
     * playlist vector. 
     */
    void loadPlaylist();


    /**
     * Removes a song from the playlist
     * 
     * @param id: the id of the song to remove
     */
    void deleteFromPlaylist(int id);

    /**
     * Allows user to browse for songs on their laptop and loads each selected song in a Song object.
     * The Song object is then added to the songs vector aka the playlist. 
     *
     * @param button: the button that was clicked
     */
    void importSongToPlaylist();

    /**
     * The selected song is picked from the playlist vector and loaded to a DeckGUI object,
     * which loads the song to be played.
     *
     * @param DeckGUI* deckGUI: the deck (left or right) into which the song is loaded
     */
    void loadSongInDeck(DeckGUI* deckGUI);

    /**
     * Checks if a song is in the playlist
     *
     * @param juce::String query: the user input into the search bar, as string
     */
    void searchPlaylist(juce::String query);

    /**
     * Given the song name, returns the song ID
     *
     * @param query: the user input into the search bar, as string
     * @returns: the song ID if found, else -1
     */
    int whereInPlaylist(juce::String query);

    /**
     * Checks if a song name is already present in the playlist
     *
     * @param fileNameWithoutExtension: the name of the song to look for
     * @returns: True if the song is found, else False
     */
    bool songIsInPlaylist(juce::String fileNameWithoutExtension);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlaylistComponent);
};
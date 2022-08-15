/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 19 Jul 2022 7:38:13pm
    Author:  ventafri

  ==============================================================================
*/

#include "PlaylistComponent.h"

#include <JuceHeader.h>
#include "PlaylistComponent.h"


PlaylistComponent::PlaylistComponent(DeckGUI* _deckGUI1,
                                     DeckGUI* _deckGUI2,
                                     DJAudioPlayer* _metadataParser) : deckGUI1(_deckGUI1),
                                                                       deckGUI2(_deckGUI2),
                                                                       metadataParser(_metadataParser)
{
    // track title
    addAndMakeVisible(tableComponent);
    tableComponent.getHeader().addColumn("Track title", 1, 400);
    
    // search songs
    addAndMakeVisible(searchBox);
    searchBox.setTextToShowWhenEmpty("Search for song name and press Enter.",
                           juce::Colours::lightcoral);
    searchBox.onReturnKey = [this] { searchPlaylist(searchBox.getText()); };

    // list of songs
    addAndMakeVisible(playlist);
    playlist.getHeader().addColumn("Song", 1, 1);
    playlist.getHeader().addColumn("Duration", 2, 1);
    playlist.getHeader().addColumn("", 3, 1);
    playlist.setModel(this);

    // load playlist, if any songs were alredy added to it
    loadPlaylist();

    // import songs button
    addAndMakeVisible(importSongsButton);
    importSongsButton.addListener(this);

    // description 
    addAndMakeVisible(decksLabel);
    decksLabel.setFont(juce::Font(16.0f, juce::Font::bold));
    decksLabel.setText("Add song to left or right deck:", juce::dontSendNotification);
    decksLabel.setColour(juce::Label::textColourId, juce::Colours::lightcoral);
    decksLabel.setJustificationType(juce::Justification::centred);

    // add songs to left and right buttons
    addAndMakeVisible(addSongToLeftDeckButton);
    addAndMakeVisible(addSongToRightDeckButton);
    addSongToLeftDeckButton.addListener(this);
    addSongToRightDeckButton.addListener(this);
}

PlaylistComponent::~PlaylistComponent()
{
    savePlaylist();
}

void PlaylistComponent::paint(juce::Graphics& g)
{
    importSongsButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::grey);
    addSongToLeftDeckButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::coral);
    addSongToRightDeckButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::coral);
}

void PlaylistComponent::resized()
{
    double rowH = getHeight() / 20;

    tableComponent.setBounds(0, 0, getWidth(), getHeight());

    searchBox.setBounds(0, 0, getWidth(), 2 * rowH);

    playlist.setBounds(0, 2 * rowH, getWidth(), 13 * rowH);
    playlist.getHeader().setColumnWidth(1, 5 * getWidth() / 10);
    playlist.getHeader().setColumnWidth(2, 3 * getWidth() / 10);
    playlist.getHeader().setColumnWidth(3, 2 * getWidth() / 10);

    importSongsButton.setBounds(0, 15 * rowH, getWidth(), 2 * rowH);
    decksLabel.setBounds(0, 17 * rowH, getWidth(), rowH);
    addSongToLeftDeckButton.setBounds(0, 18 * rowH, getWidth() / 2, 2 * rowH);
    addSongToRightDeckButton.setBounds(getWidth() / 2, 18 * rowH, getWidth() / 2, 2 * rowH);
}

int PlaylistComponent::getNumRows()
{
    return songs.size();
}

void PlaylistComponent::paintRowBackground(juce::Graphics& g,
    int rowNumber,
    int width,
    int height,
    bool rowIsSelected)
{
    if (rowIsSelected)
    {
        g.setColour(juce::Colours::white);
        g.fillAll(juce::Colours::coral);
    }
    else {
        g.fillAll(juce::Colours::lightgrey);
    }
}

void PlaylistComponent::paintCell(juce::Graphics& g,
    int rowNumber,
    int columnId,
    int width,
    int height,
    bool rowIsSelected)
{
    if (rowNumber < getNumRows())
    {
        if (columnId == 1)
        {
            g.drawText(songs[rowNumber].songName,
                2,
                0,
                width - 4,
                height,
                juce::Justification::centredLeft,
                true
            );
        }
        if (columnId == 2)
        {
            g.drawText(songs[rowNumber].trackDuration,
                2,
                0,
                width - 4,
                height,
                juce::Justification::centred,
                true
            );
        }
    }
}

juce::Component* PlaylistComponent::refreshComponentForCell(int rowNumber,
    int columnId,
    bool isRowSelected,
    Component* existingComponentToUpdate)
{
    if (columnId == 3)
    {
        if (existingComponentToUpdate == nullptr)
        {
            /* Adds a button next to each track to remove the track from the playlist.  */
            juce::TextButton* btn = new juce::TextButton{ "X" };
            juce::String id{ std::to_string(rowNumber) };
            btn->setComponentID(id);
            btn->addListener(this);
            existingComponentToUpdate = btn;
        }
    }
    return existingComponentToUpdate;
}

void PlaylistComponent::buttonClicked(juce::Button* button)
{
    if (button == &importSongsButton)
    {
        importSongToPlaylist();
        playlist.updateContent();
    }
    else if (button == &addSongToLeftDeckButton)
    {
        loadSongInDeck(deckGUI1);
    }
    else if (button == &addSongToRightDeckButton)
    {
        loadSongInDeck(deckGUI2);
    }
    else
    {
        int id = std::stoi(button->getComponentID().toStdString());
        deleteFromPlaylist(id);
        playlist.updateContent();
    }
}


void PlaylistComponent::savePlaylist()
{
    std::ofstream myPlaylist("playlist.csv");

    for (Song& t : songs)
    {
        myPlaylist << t.file.getFullPathName() << "," << t.trackDuration << "\n";
    }
}

void PlaylistComponent::loadPlaylist()
{
    // create input stream from saved playlist
    std::ifstream myPlaylist("playlist.csv");
    std::string filePath;
    std::string length;

    // Read data, line by line
    if (myPlaylist.is_open())
    {
        while (getline(myPlaylist, filePath, ',')) {
            juce::File file{ filePath };
            Song newTrack{ file };
            getline(myPlaylist, length);
            newTrack.trackDuration = length;
            songs.push_back(newTrack);
        }
    }
    myPlaylist.close();
}

void PlaylistComponent::loadSongInDeck(DeckGUI* deckGUI)
{
    int selectedRow{ playlist.getSelectedRow() };
    if (selectedRow != -1)
    {
        deckGUI->loadFile(songs[selectedRow].URL);
    }
    else
    {
        juce::AlertWindow::showMessageBox(juce::AlertWindow::AlertIconType::InfoIcon,
            "Add to Deck Information:",
            "Please select a track to add to deck",
            "OK",
            nullptr
        );
    }
}

void PlaylistComponent::importSongToPlaylist()
{
    juce::FileChooser chooser{ "Select files" };
    if (chooser.browseForMultipleFilesToOpen())
    {
        for (const juce::File& file : chooser.getResults())
        {
            juce::String fileNameWithoutExtension{ file.getFileNameWithoutExtension() };
            // load songs if not already loaded
            if (!songIsInPlaylist(fileNameWithoutExtension)) 
            {
                Song newTrack{ file };
                juce::URL audioURL{ file };
                newTrack.trackDuration = getLength(audioURL);
                songs.push_back(newTrack);
            }
            // If a song was already loaded, alert user and don't import
            else 
            {
                juce::AlertWindow::showMessageBox(juce::AlertWindow::AlertIconType::WarningIcon,
                    "Warning:",
                    fileNameWithoutExtension + " was already loaded. Not loading again.",
                    "OK",
                    nullptr
                );
            }
        }
    }
}

bool PlaylistComponent::songIsInPlaylist(juce::String fileNameWithoutExtension)
{
    return (std::find(songs.begin(), songs.end(), fileNameWithoutExtension) != songs.end());
}

void PlaylistComponent::deleteFromPlaylist(int id)
{
    songs.erase(songs.begin() + id);
}

juce::String PlaylistComponent::getLength(juce::URL audioURL)
{
    metadataParser->loadURL(audioURL);
    double seconds{ metadataParser->getLengthInSeconds() };
    juce::String minutes{ secondsToMinutes(seconds) };
    return minutes;
}

juce::String PlaylistComponent::secondsToMinutes(double seconds)
{
    // find seconds and minutes and make into string
    int secondsRounded{ int(std::round(seconds)) };
    juce::String min{ std::to_string(secondsRounded / 60) };
    juce::String sec{ std::to_string(secondsRounded % 60) };

    if (sec.length() < 2) // if seconds is 1 digit or less
    {
        // add '0' to seconds until seconds is length 2
        sec = sec.paddedLeft('0', 2);
    }
    return juce::String{ min + ":" + sec };
}

void PlaylistComponent::searchPlaylist(juce::String query)
{
    if (query != "")
    {
        int rowNumber = whereInPlaylist(query);
        playlist.selectRow(rowNumber);
    }
    else
    {
        playlist.deselectAllRows();
    }
}

int PlaylistComponent::whereInPlaylist(juce::String query)
{
    // finds index where track title contains searchText
    auto it = find_if(songs.begin(), songs.end(),
        [&query](const Song& obj) {return obj.songName.contains(query); });
    int i = -1;

    if (it != songs.end())
    {
        i = std::distance(songs.begin(), it);
    }

    return i;
}
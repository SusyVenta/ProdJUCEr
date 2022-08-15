#pragma once
#include "MainComponent.h"

MainComponent::MainComponent()
{
    // size of app window
    setSize(920, 640);

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired(juce::RuntimePermissions::recordAudio)
        && !juce::RuntimePermissions::isGranted(juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request(juce::RuntimePermissions::recordAudio,
            [&](bool granted) { if (granted) setAudioChannels(2, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels(0, 2);  //zero inputs (no mic), 2 outputs left and right channels
    }

    addAndMakeVisible(deckGUI1);
    addAndMakeVisible(deckGUI2);
    addAndMakeVisible(playlistComponent);

    // otherwise app won't know formats e.g. mp3
    formatManager.registerBasicFormats(); 
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
    player2.prepareToPlay(samplesPerBlockExpected, sampleRate);
    mixerSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    mixerSource.addInputSource(&player1, false);
    mixerSource.addInputSource(&player2, false);
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    mixerSource.getNextAudioBlock(bufferToFill);
}


void MainComponent::releaseResources()
{
    // Called when the audio device stops or when it is being restarted due to a setting change.
    player1.releaseResources();
    player2.releaseResources();
    mixerSource.releaseResources();
}


void MainComponent::paint(juce::Graphics& g)
{
    // Our component is opaque, so we must fill the background with a solid colour
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void MainComponent::resized()
{
    // left deck
    deckGUI1.setBounds(
        0, //start at X
        0,  //start Y
        5 * getWidth() / 14, // width
        getHeight()); // height

    // playlist
    playlistComponent.setBounds(
        5 * getWidth() / 14,
        0,
        4 * getWidth() / 14,
        getHeight());

    // right deck
    deckGUI2.setBounds(
        9 * getWidth() / 14,
        0,
        5 * getWidth() / 14,
        getHeight());
}
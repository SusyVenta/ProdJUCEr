#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (0, 2); // initially (2, 2)
    }
    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(gainSlider);
    addAndMakeVisible(loadButton);
    

    playButton.addListener(this);
    stopButton.addListener(this);
    gainSlider.addListener(this);
    loadButton.addListener(this);

    gainSlider.setRange(0.00001, 1);

    /* sound effects menu */
    addAndMakeVisible(effectsMenu);
    effectsMenuTextLabel.setFont(textFont);
    effectsMenu.addItem("Plain", 1);
    effectsMenu.addItem("Siren", 2);

    effectsMenu.onChange = [this] { effectsMenuChanged(); };
    effectsMenu.setSelectedId(1);
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.

    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.

    // For more details, see the help for AudioProcessor::prepareToPlay()
    playing = false;
    phase = 0.0;
    dphase = 0.00001;
    gain = 0.5;

    chosenEffect = "Plain";

    formatManager.registerBasicFormats();
    juce::URL audioURL{ "file:///C:\\Users\\ventafri\\Desktop\\Uni\\OOP\\uni_year2_OOP\\programming\\DJApp\\tracks/aon_inspired.mp3"};

    auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));
    if (reader != nullptr) { // good file
        std::unique_ptr<juce::AudioFormatReaderSource> newSource(new juce::AudioFormatReaderSource(reader, true));
        transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
        readerSource.reset(newSource.release());
        transportSource.start();
    };
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
};

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{   
    transportSource.getNextAudioBlock(bufferToFill);
    /*if (!playing)
    {
        bufferToFill.clearActiveBufferRegion();
        return;
    }

    auto* leftChan = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
    auto* rightChan = bufferToFill.buffer->getWritePointer(1, bufferToFill.startSample);

    for (auto i = 0; i < bufferToFill.numSamples; ++i) {
        if (chosenEffect == "Plain") {
            double sample = sin(phase);
            leftChan[i] = sample * 0.125;
            rightChan[i] = sample * 0.125;

            phase += dphase;
        } 
        if (chosenEffect == "Siren") {
            auto sample = fmod(phase, 1.0f);
            phase += fmod(dphase, 0.01f);
            dphase += 0.0000005f;
            leftChan[i] = sample * 0.125 * gain;
            rightChan[i] = sample * 0.125 * gain;

        }
        
    };*/

    //bufferToFill.clearActiveBufferRegion();
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
    transportSource.releaseResources();
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setFont(20.0f);
    g.drawText("Hello from ", getLocalBounds(), juce::Justification::centred, true);
}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    double rowHeight = getHeight() / 10;

    effectsMenu.setBounds(0, rowHeight, getWidth() / 4, rowHeight / 3);

    playButton.setBounds(0, rowHeight * 2, getWidth() / 8, rowHeight / 2);
    stopButton.setBounds(getWidth() / 7, rowHeight * 2, getWidth() / 8, rowHeight / 2);
    gainSlider.setBounds(0, rowHeight * 3, getWidth(), rowHeight);
    loadButton.setBounds(getWidth() * 0.8, rowHeight * 2, getWidth() / 8, rowHeight / 2);
    
    DBG("MainComponent::resized");

}


void MainComponent::buttonClicked(juce::Button* button) {
    if (button == &playButton)
    {
        playing = true;
        if (chosenEffect == "Siren") {
            dphase = 0;
        }
    }
    if (button == &stopButton)
    {
        playing = false;
    }
    if (button == &loadButton)
    {   
        chooser = std::make_unique<juce::FileChooser>(
            "Select a file to play..",
            juce::File{},
            "*.mp3"
            );
        auto chooserFlags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;
        chooser->launchAsync(juce::FileBrowserComponent::canSelectFiles, [this](const juce::FileChooser& fileChooser)
            {
                juce::File file = fileChooser.getResult();
                if (file.existsAsFile())
                {
                    loadURL(juce::URL{ fileChooser.getResult() });
                }
            });
    }
}

void MainComponent::sliderValueChanged(juce::Slider* slider) {
    if (slider == &gainSlider) {
        DBG("MainComponent::sliderValueChanged: gainSlider " << gainSlider.getValue());
        dphase = gainSlider.getValue();
        gain = gainSlider.getValue();
    }
}

void MainComponent::effectsMenuChanged() {
    switch (effectsMenu.getSelectedId())
    {
    case 1:      chosenEffect = "Plain";      break;
    case 2:      chosenEffect = "Siren";      break;
    }
    DBG("chosenEffect: " << effectsMenu.getText());
}

void MainComponent::loadURL(juce::URL audioURL) {
    auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));
    if (reader != nullptr) { // good file
        std::unique_ptr<juce::AudioFormatReaderSource> newSource(new juce::AudioFormatReaderSource(reader, true));
        transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
        readerSource.reset(newSource.release());
        transportSource.start();
    };
};

/*
  ==============================================================================

    AxisModifier.h
    Created: 19 Jul 2022 7:41:19pm
    Author:  ventafri

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

//==============================================================================
/*
*/
class AxisModifier : public juce::Component,
    public juce::SettableTooltipClient //Class allows to implement pop up with help
{
public:
    AxisModifier();
    ~AxisModifier() override;

    void paint(juce::Graphics&) override;
    void resized() override;

    void mouseDown(const juce::MouseEvent& event) override;
    void mouseUp(const juce::MouseEvent& event) override;
    void mouseDrag(const juce::MouseEvent& event) override;

    // set the grid in AxisModifier
    void createGrid(int lineCount = 4);

    // set range
    void setRange(float min = 0.0f, float max = 1.0f);

    // get current x coordinates
    float getX();

    // get current y coordinates
    float getY();

    // create sub class for an EventListener for the Axis Modificator
    class EventListener
    {
    public:
        EventListener();
        ~EventListener();

        //Event listener callback
        virtual void AxisModifierValueChange(AxisModifier* AxisModifier) = 0;
    };

    //calls listener callback
    void listenerCallback();

    //add component to Listener list
    void addEventListener(EventListener* listener);

    //remove component from Listener list
    void removeEventListener(EventListener* listener);

private:

    //create a list of event listeners
    juce::ListenerList<EventListener> eventlisteners;

    // create map to hold key-value pairs with coordinates from AxisModifier
    std::map<char, float> coordinates;

    // create map to hold key-value pairs with range from AxisModifier
    std::map<char, float> originalRange;

    void setOriginalRange();
    void initCoordinates(float origX, float origY);
    void setCoordinates(float origX, float origY);
    void updateCoords();

    //variables for position
    float middleY;
    float middleX;
    float left;
    float right;
    float top;
    float bottom;
    void initProporties();

    //User settings
    int gridLines;
    std::map<char, float> range;

    void drawPlot(juce::Graphics& g);
    void drawAxis(juce::Graphics& g);
    void drawGrid(juce::Graphics& g);
    void drawMarker(juce::Graphics& g);
    void drawMarkerValueText(juce::Graphics& g);
    bool markerRelocated{ false };

    float constrain(float coordinates);
    float invertCoordinates(float coordinates, float min, float max);
    bool checkIfInRawRange(float origX, float origY);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AxisModifier);
};
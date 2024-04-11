/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class CustomLookAndFeel : public juce::LookAndFeel_V4
{
public:
    CustomLookAndFeel()
    {
        setColour(juce::Slider::thumbColourId, juce::Colours::purple);
    }

private:
    juce::Path dialTick;
    juce::AffineTransform transform;
};

//===============================================================================
//FILMSTRIP
class FilmStripSlider
{
public:
    FilmStripSlider(juce::Image* _knobStrip);
    void drawFrame(juce::Graphics &g, int x, int y, int width, int height, juce::Slider& slider, double position);
    juce::Image* knobStrip;

private:
    int frameCount, frameSize;
    bool isVerticalStrip;
};

class FilmKnobLookAndFeel : public juce::LookAndFeel_V4
{
public:
    FilmKnobLookAndFeel(){}
    ~FilmKnobLookAndFeel(){}

    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider) override
    {
        float diameter = juce::jmin(width, height);
        float radius = diameter / 2;
        float centerx = x + width / 2;
        float centery = y + height / 2;
        float rx = centerx - radius;
        float ry = centery - radius;
        float angle = rotaryStartAngle + (sliderPos * (rotaryEndAngle - rotaryStartAngle));
    }
};
//==================================================================================
//TEMPLATE CODE
class JustAKnobAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    JustAKnobAudioProcessorEditor (JustAKnobAudioProcessor&);
    ~JustAKnobAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JustAKnobAudioProcessor& audioProcessor;
    CustomLookAndFeel knobLookAndFeel;

    juce::Slider knob;
    juce::Image knobFilmRoll;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (JustAKnobAudioProcessorEditor)
};

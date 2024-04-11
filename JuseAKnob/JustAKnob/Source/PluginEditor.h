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
    //juce::Slider knob, secondKnob;
    CustomLookAndFeel knobLookAndFeel;

    juce::Slider knob;
    juce::Image knobFilmRoll;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (JustAKnobAudioProcessorEditor)
};

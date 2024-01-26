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
};

class JustAKnobAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    JustAKnobAudioProcessorEditor (JustAKnobAudioProcessor&);
    ~JustAKnobAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    JustAKnobAudioProcessor& audioProcessor;
    juce::Slider knob;

    CustomLookAndFeel customLookAndFeel;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (JustAKnobAudioProcessorEditor)
};

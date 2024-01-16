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
class GainAAudioProcessorEditor : public juce::AudioProcessorEditor, private juce::Slider::Listener
{
public:
    GainAAudioProcessorEditor (GainAAudioProcessor&);
    ~GainAAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // Create variable and method for slider feedback
    void sliderValueChanged(juce::Slider* slider) override;

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    GainAAudioProcessor& audioProcessor;

    // Slider GUI
    juce::Slider midiVolume; // [1]

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GainAAudioProcessorEditor)
};

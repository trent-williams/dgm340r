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

    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider) override
    {
        juce::Rectangle<float> dialArea(x, y, width, height);
        //knobGraphic = juce::ImageCache::getFromMemory(BinaryData::Knob_png, BinaryData::Knob_pngSize);
        //knobGraphic.rescaled(200, 200, juce::Graphics::mediumResamplingQuality);

        g.drawImage(knobGraphic, 20, 20, 400, 400, 0, 0, 1000, 1000, false);
    }

private:
    juce::Image knobGraphic = juce::ImageCache::getFromMemory(BinaryData::Knob_png, BinaryData::Knob_pngSize);
    juce::File knobImagePath;
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

    CustomLookAndFeel knobLookAndFeel;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (JustAKnobAudioProcessorEditor)
};

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

    /*void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider) override
    {
        float diameter = juce::jmin(width, height);
        float radius = diameter / 2;
        float centerX = x + width / 2;
        float centerY = y + height / 2;
        float rx = centerX - radius;
        float ry = centerY - radius;
        float angle = rotaryStartAngle + (sliderPos* (rotaryEndAngle - rotaryStartAngle));

        float scaleFactor = diameter/knobGraphic.getWidth();
        float imageCenterX = (width - knobGraphic.getWidth()) / 2;
        float imageCenterY = (height - knobGraphic.getHeight()) / 2;

        //juce::Image knobImage = juce::ImageFileFormat::loadFrom(knobImagePath);
        
        
        transform = juce::AffineTransform::scale(scaleFactor);
        //transform = juce::AffineTransform::translation(0, 0);
        transform = juce::AffineTransform::rotation(angle, knobGraphic.getWidth() / 2, knobGraphic.getHeight() / 2);
        
        g.drawImageTransformed(knobGraphic, transform);//.translated(centerX, centerY));
        //g.drawImageTransformed(knobImage, transform);//.translated(centerX, centerY));

        dialTick.addRectangle(0, -radius, 5.0, 200);
        g.fillPath(dialTick, juce::AffineTransform::rotation(angle).translated(centerX, centerY));
    }*/

private:
    //juce::Image knobGraphic = juce::ImageCache::getFromMemory(BinaryData::Knob_png, BinaryData::Knob_pngSize);
    //juce::File knobImagePath = "C:\\Users\\trent\\OneDrive - Utah Valley Universit\\School Docs\\02 Spring 2024\\DGM 340R\\Knob.png";
    juce::Path dialTick;
    juce::AffineTransform transform;
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
    juce::Slider knob, secondKnob;

    CustomLookAndFeel knobLookAndFeel;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (JustAKnobAudioProcessorEditor)
};

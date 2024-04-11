/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once
#define _USE_MATH_DEFINES

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include <cmath>
#include <math.h>

//==============================================================================
/**
*/
//NEW LOOK AND FEEL
class SliderLookAndFeel : public juce::LookAndFeel_V4
{
public:
    SliderLookAndFeel()
    {
        setColour(juce::Slider::textBoxTextColourId, juce::Colours::black);
    }
    ~SliderLookAndFeel(){}

    void drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, const juce::Slider::SliderStyle style, juce::Slider& slider) override
    {
        g.drawImage(thumbImage, x, sliderPos - (thumbImage.getHeight() / 2) + 475, thumbImage.getWidth(), thumbImage.getHeight(), 0, 0, thumbImage.getWidth() * 10, thumbImage.getHeight() * 10);
    }

private:
    juce::Image thumbImage = juce::ImageCache::getFromMemory(BinaryData::Smily_png, BinaryData::Smily_pngSize);
};
//=================================================================================
//NEW SLIDER FUNCTION
class FilmStripKnob : public Slider
{
public:
    FilmStripKnob(juce::File const& image, const int numFrames, const bool stripIsHorizontal) : juce::Slider(image.getFullPathName() + T("FilmStripKnob")), filmStrip(image.exists() ? ImageFileFormat::loadFrom(image) : 0), numFrames_(numFrames), isHorizontal_(stripIsHorizontal)
    {
        if (filmStrip)
        {
            setTextBoxStyle(NoTextBox)
        }
    }
};
//==================================================================================
//OLD CODE
class AHCompStripAudioProcessorEditor : public juce::AudioProcessorEditor

{
public:
    AHCompStripAudioProcessorEditor(AHCompStripAudioProcessor&);
    ~AHCompStripAudioProcessorEditor() override;

    //==============================================================================
    void paint(juce::Graphics&) override;
    void resized() override;

    typedef juce::AudioProcessorValueTreeState::SliderAttachment SliderAttachment;


private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AHCompStripAudioProcessor& audioProcessor;
    

    juce::Slider panDial;
    std::unique_ptr<SliderAttachment> panAttachment;

    juce::Slider gainFader;
    std::unique_ptr<SliderAttachment> faderAttachment;

    juce::Slider compInput;
    juce::Label compInputLabel;
    std::unique_ptr<SliderAttachment> inputAttachment;

    juce::Slider compThresh;
    juce::Label compThreshLabel;
    std::unique_ptr<SliderAttachment> threshAttachment;

    juce::Slider compAttack;
    juce::Label compAttackLabel;
    std::unique_ptr<SliderAttachment> attackAttachment;

    juce::Slider compRatio;
    juce::Label compRatioLabel;
    std::unique_ptr<SliderAttachment> ratioAttachment;

    juce::Slider compRelease;
    juce::Label compReleaseLabel;
    std::unique_ptr<SliderAttachment> releaseAttachment;

    juce::Slider compOutput;
    juce::Label compOutputLabel;
    std::unique_ptr<SliderAttachment> outputAttachment;

    juce::Image background = juce::ImageCache::getFromMemory(BinaryData::Press_png, BinaryData::Press_pngSize);
    
    SliderLookAndFeel sliderLookAndFeel;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AHCompStripAudioProcessorEditor)
};

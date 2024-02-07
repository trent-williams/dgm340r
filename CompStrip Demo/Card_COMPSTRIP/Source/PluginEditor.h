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
class AHCompStripAudioProcessorEditor : public juce::AudioProcessorEditor
    //,public juce::Slider::Listener
{
public:
    AHCompStripAudioProcessorEditor(AHCompStripAudioProcessor&);
    ~AHCompStripAudioProcessorEditor() override;

    //==============================================================================
    void paint(juce::Graphics&) override;
    void resized() override;
    //void sliderValueChanged(juce::Slider* slider) override;

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


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AHCompStripAudioProcessorEditor)
};

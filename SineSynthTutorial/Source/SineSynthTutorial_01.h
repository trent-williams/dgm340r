/*
  ==============================================================================

   This file is part of the JUCE tutorials.
   Copyright (c) 2020 - Raw Material Software Limited

   The code included in this file is provided under the terms of the ISC license
   http://www.isc.org/downloads/software-support-policy/isc-license. Permission
   To use, copy, modify, and/or distribute this software for any purpose with or
   without fee is hereby granted provided that the above copyright notice and
   this permission notice appear in all copies.

   THE SOFTWARE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES,
   WHETHER EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR
   PURPOSE, ARE DISCLAIMED.

  ==============================================================================
*/

/*******************************************************************************
 The block below describes the properties of this PIP. A PIP is a short snippet
 of code that can be read by the Projucer and used to generate a JUCE project.

 BEGIN_JUCE_PIP_METADATA

 name:             SineSynthTutorial
 version:          1.0.0
 vendor:           JUCE
 website:          http://juce.com
 description:      Simple sine synthesiser.

 dependencies:     juce_audio_basics, juce_audio_devices, juce_audio_formats,
                   juce_audio_processors, juce_audio_utils, juce_core,
                   juce_data_structures, juce_events, juce_graphics,
                   juce_gui_basics, juce_gui_extra
 exporters:        xcode_mac, vs2019, linux_make

 type:             Component
 mainClass:        MainContentComponent

 useLocalCopy:     1

 END_JUCE_PIP_METADATA

*******************************************************************************/


#pragma once

//==============================================================================
class MainContentComponent   : public juce::AudioAppComponent
{
public:
    MainContentComponent()
    {
        addAndMakeVisible (frequencySlider);
        frequencySlider.setSliderStyle(juce::Slider::Rotary);
        frequencySlider.setRange (50.0, 5000.0);
        frequencySlider.setSkewFactorFromMidPoint (500.0); // [4]
        //frequencySlider.setLookAndFeel(&customLookAndFeel);
        frequencySlider.onValueChange = [this] { targetFrequency = frequencySlider.getValue(); };
        {
            if (currentSampleRate > 0.0)
                updateAngleDelta();
        };

        addAndMakeVisible(levelSlider);
        levelSlider.setRange(0.0, 0.25);

        setSize (600, 100);
        setAudioChannels (0, 2); // no inputs, two outputs
    }

    ~MainContentComponent() override
    {
        shutdownAudio();
    }

    void resized() override
    {
        //frequencySlider.setBounds (10, 10, getWidth() - 20, 20);
        frequencySlider.setBounds(getWidth() / 2 - 100, getHeight() / 2 - 100, 200, 200);
        levelSlider.setBounds(10, 10, getWidth() - 20, 20);
    }

    void updateAngleDelta()
    {
        auto cyclesPerSample = frequencySlider.getValue() / currentSampleRate;         // [2]
        angleDelta = cyclesPerSample * 2.0 * juce::MathConstants<double>::pi;          // [3]
    }

    void prepareToPlay (int, double sampleRate) override
    {
        currentSampleRate = sampleRate;
        updateAngleDelta();
    }

    void releaseResources() override {}

    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override
    {
        /*auto level = levelSlider.getValue();
        auto* leftBuffer  = bufferToFill.buffer->getWritePointer (0, bufferToFill.startSample);
        auto* rightBuffer = bufferToFill.buffer->getWritePointer (1, bufferToFill.startSample);

        for (auto sample = 0; sample < bufferToFill.numSamples; ++sample)
        {
            auto currentSample = (float) std::sin (currentAngle);
            currentAngle += angleDelta;
            leftBuffer[sample]  = currentSample * level;
            rightBuffer[sample] = currentSample * level;
        }*/

        //Updated Synthesis Code
        auto level = levelSlider.getValue();
        auto* leftBuffer = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
        auto* rightBuffer = bufferToFill.buffer->getWritePointer(1, bufferToFill.startSample);

        auto localTargetFrequency = targetFrequency;

        if (! juce::approximatelyEqual (localTargetFrequency, currentFrequency))
        {
            auto frequencyIncrement = (localTargetFrequency - currentFrequency) / bufferToFill.numSamples;
            
            for (auto sample = 0; sample < bufferToFill.numSamples; ++sample)
            {
                auto currentSample = (float)std::sin(currentAngle);
                currentFrequency += frequencyIncrement;
                updateAngleDelta();
                currentAngle += angleDelta;
                leftBuffer[sample] = currentSample * level;
                rightBuffer[sample] = currentSample * level;
            }
            
            currentFrequency = localTargetFrequency;
        }
        else
        {
            for (auto sample = 0; sample < bufferToFill.numSamples; ++sample)
            {
                auto currentSample = (float)std::sin(currentAngle);
                currentAngle += angleDelta;
                leftBuffer[sample] = currentSample * level;
                rightBuffer[sample] = currentSample * level;
            }
        }
    }

private:
    juce::Slider frequencySlider;
    juce::Slider levelSlider;
    double currentSampleRate = 0.0, currentAngle = 0.0, angleDelta = 0.0; // [1]
    float targetFrequency, currentFrequency;
    //int sample;
    

    //CustomLookAndFeel customLookAndFeel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};

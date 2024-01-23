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

 name:             SimpleSynthNoiseTutorial
 version:          1.0.0
 vendor:           JUCE
 website:          http://juce.com
 description:      Simple noise synthesiser.

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
        setSize (800, 600);
        setAudioChannels (0, 2); // no inputs, two outputs

        //GUI
        levelSliderLeft.setRange(0, 0.25);
        levelSliderLeft.setTextBoxStyle(juce::Slider::TextBoxRight, true, 100, 25);
        levelSliderRight.setRange(0, 0.25);
        levelSliderRight.setTextBoxStyle(juce::Slider::TextBoxRight, true, 100, 25);
        levelLabelLeft.setText("Noise Level Left", juce::dontSendNotification);
        levelLabelRight.setText("Noise Level Right", juce::dontSendNotification);

        // Make things visable
        addAndMakeVisible(levelSliderLeft);
        addAndMakeVisible(levelSliderRight);
        addAndMakeVisible(levelLabelLeft);
        addAndMakeVisible(levelLabelRight);
    }

    ~MainContentComponent() override
    {
        shutdownAudio();
    }

    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override
    {
        juce::String message;
        message << "Preparing to play audio...\n";
        message << " samplesPerBlockExpected = " << samplesPerBlockExpected << "\n";
        message << " sampleRate = " << sampleRate;
        juce::Logger::getCurrentLogger()->writeToLog (message);
    }

    void releaseResources() override
    {
        juce::Logger::getCurrentLogger()->writeToLog ("Releasing audio resources");
    }

    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override
    {
        auto levelLeft = (float)levelSliderLeft.getValue();
        auto levelRight = (float)levelSliderRight.getValue();
        auto levelScaleLeft = levelLeft * 2.0f;
        auto levelScaleRight = levelRight * 2.0f;

        for (auto channel = 0; channel < bufferToFill.buffer->getNumChannels(); ++channel)
        {
            // Get a pointer to the start sample in the buffer for this audio output channel
            auto* buffer = bufferToFill.buffer->getWritePointer(channel, bufferToFill.startSample);

            // Fill the required number of samples with noise between -0.125 and +0.125
            if (channel == 0)
            {
                for (auto sample = 0; sample < bufferToFill.numSamples; ++sample)
                    buffer[sample] = random.nextFloat() * levelScaleLeft - levelLeft;
            }
            else
            {
                for (auto sample = 0; sample < bufferToFill.numSamples; ++sample)
                    buffer[sample] = random.nextFloat() * levelScaleRight - levelRight;
            }
            
        }
    }

    void resized() override
    {
        levelLabelLeft.setBounds(10, 10, 90, 20);
        levelLabelRight.setBounds(10, 30, 90, 20);
        levelSliderLeft.setBounds(100, 10, getWidth() - 110, 20);
        levelSliderRight.setBounds(100, 30, getWidth() -110, 20);
    }

private:
    juce::Random random;

    juce::Slider levelSliderLeft;
    juce::Slider levelSliderRight;
    juce::Label levelLabelLeft;
    juce::Label levelLabelRight;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};

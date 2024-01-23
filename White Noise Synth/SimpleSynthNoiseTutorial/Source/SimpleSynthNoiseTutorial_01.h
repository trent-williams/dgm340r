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
class DecibelSlider : public juce::Slider
{
public :
    DecibelSlider() {}

    double getValueFromText(const juce::String& text) override
    {
        auto minusInfinitydB = -100.0;
        auto decibelText = text.upToFirstOccurrenceOf("dB", false, false).trim();

        return decibelText.equalsIgnoreCase("-INF") ? minusInfinitydB : decibelText.getDoubleValue();
    }
    
    juce::String getTextFromValue(double value) override
    {
        return juce::Decibels::toString(value);
    }

private :
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DecibelSlider)
};

class MainContentComponent   : public juce::AudioAppComponent
{
public:
    MainContentComponent()
    {
        setSize (800, 600);
        setAudioChannels (0, 2); // no inputs, two outputs

        /* //Old GUI
        levelSliderLeft.setRange(0, 0.25);
        levelSliderLeft.setTextBoxStyle(juce::Slider::TextBoxRight, true, 100, 25);
        levelSliderRight.setRange(0, 0.25);
        levelSliderRight.setTextBoxStyle(juce::Slider::TextBoxRight, true, 100, 25);
        levelLabelLeft.setText("Noise Level Left", juce::dontSendNotification);
        levelLabelRight.setText("Noise Level Right", juce::dontSendNotification);
        */

        // Note to self. Always check slider ranges. Ranges for linear sliders are
        // incredibly small compared to sliders scaled to decibels.

        // New GUI in dB, hypothetically
        levelSliderLeft.setRange(-100, -12);
        levelSliderLeft.setTextBoxStyle(juce::Slider::TextBoxRight, true, 70, 25);
        levelSliderLeft.onValueChange = [this] { levelLeftIndB = juce::Decibels::decibelsToGain((float)levelSliderLeft.getValue()); };
        levelSliderLeft.setValue(juce::Decibels::gainToDecibels(levelLeftIndB));
        

        levelSliderRight.setRange(-100, -12);
        levelSliderRight.setTextBoxStyle(juce::Slider::TextBoxRight, true, 70, 25);
        levelSliderRight.onValueChange = [this] { levelRightIndB = juce::Decibels::decibelsToGain((float)levelSliderRight.getValue()); };
        levelSliderRight.setValue(juce::Decibels::gainToDecibels(levelRightIndB));
        

        levelLabelLeft.setText("Noise Level Left in dB", juce::dontSendNotification);
        levelLabelRight.setText("Noise Level Right in dB", juce::dontSendNotification);

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
        auto currentLevelLeft = levelLeftIndB; // (float)levelSliderLeft.getValue();
        auto currentLevelRight = levelRightIndB; // (float)levelSliderRight.getValue();
        auto levelScaleLeft = currentLevelLeft * 2.0f;
        auto levelScaleRight = currentLevelRight * 2.0f;

        for (auto channel = 0; channel < bufferToFill.buffer->getNumChannels(); ++channel)
        {
            // Get a pointer to the start sample in the buffer for this audio output channel
            auto* buffer = bufferToFill.buffer->getWritePointer(channel, bufferToFill.startSample);

            // Fill the required number of samples with noise between -0.125 and +0.125
            if (channel == 0)
            {
                for (auto sample = 0; sample < bufferToFill.numSamples; ++sample)
                    buffer[sample] = random.nextFloat() * levelScaleLeft - currentLevelLeft;
            }
            else
            {
                for (auto sample = 0; sample < bufferToFill.numSamples; ++sample)
                    buffer[sample] = random.nextFloat() * levelScaleRight - currentLevelRight;
            }
            
        }
    }

    void resized() override
    {
        levelLabelLeft.setBounds(10, 10, 140, 20);
        levelLabelRight.setBounds(10, 30, 140, 20);
        levelSliderLeft.setBounds(150, 10, getWidth() - 160, 20);
        levelSliderRight.setBounds(150, 30, getWidth() -160, 20);
    }

private:
    juce::Random random;

    //juce::Slider levelSliderLeft;
    //juce::Slider levelSliderRight;
    DecibelSlider levelSliderLeft;
    DecibelSlider levelSliderRight;
    juce::Label levelLabelLeft;
    juce::Label levelLabelRight;

    float levelLeftIndB = 0.0f;
    float levelRightIndB = 0.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};

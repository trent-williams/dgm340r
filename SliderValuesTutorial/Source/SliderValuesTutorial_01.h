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

 name:             SliderValuesTutorial
 version:          1.0.0
 vendor:           JUCE
 website:          http://juce.com
 description:      Explores slider values.

 dependencies:     juce_core, juce_data_structures, juce_events, juce_graphics,
                   juce_gui_basics
 exporters:        xcode_mac, vs2019, linux_make, xcode_iphone, androidstudio

 type:             Component
 mainClass:        MainContentComponent

 useLocalCopy:     1

 END_JUCE_PIP_METADATA

*******************************************************************************/


#pragma once

//==============================================================================
class MainContentComponent   : public juce::Component,
                               public juce::Slider::Listener
{
public:
    //==============================================================================
    MainContentComponent()
    {
        addAndMakeVisible (frequencySlider);
        frequencySlider.setRange (50, 5000.0);          // [1]
        frequencySlider.setTextValueSuffix (" Hz");     // [2]
        frequencySlider.addListener (this);             // [3]
        frequencySlider.setSkewFactor(0.35);            // Skew slider towards lower numbers

        addAndMakeVisible (frequencyLabel);
        frequencyLabel.setText ("Frequency", juce::dontSendNotification);
        frequencyLabel.attachToComponent (&frequencySlider, true); // [4]

        addAndMakeVisible (durationSlider);
        durationSlider.setRange (1.0 / frequencySlider.getMaximum(),
                                 1.0 / frequencySlider.getMinimum());
        durationSlider.setTextValueSuffix (" seconds");
        durationSlider.addListener (this);
        durationSlider.setSkewFactor(0.35);             // Skew slider to be in line with frequencySlider

        addAndMakeVisible (durationLabel);
        durationLabel.setText ("Period", juce::dontSendNotification);
        durationLabel.attachToComponent (&durationSlider, true);

        frequencySlider.setValue (500.0); // [5]

        setSize (600, 90);
    }

    void resized() override
    {
        auto sliderLeft = 120;
        frequencySlider.setBounds (sliderLeft, 20, getWidth() - sliderLeft - 10, 20);
        durationSlider .setBounds (sliderLeft, 50, getWidth() - sliderLeft - 10, 20);
    }

    void sliderValueChanged (juce::Slider* slider) override
    {
        if (slider == &frequencySlider)
            durationSlider.setValue (1.0 / frequencySlider.getValue(), juce::dontSendNotification);
        else if (slider == &durationSlider)
            frequencySlider.setValue (1.0 / durationSlider.getValue(), juce::dontSendNotification);
    }

private:
    juce::Slider frequencySlider;
    juce::Label  frequencyLabel;
    juce::Slider durationSlider;
    juce::Label  durationLabel;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};

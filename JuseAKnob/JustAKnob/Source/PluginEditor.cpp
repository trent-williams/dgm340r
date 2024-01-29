/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
JustAKnobAudioProcessorEditor::JustAKnobAudioProcessorEditor (JustAKnobAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (500, 500);

    addAndMakeVisible(knob);
    knob.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalDrag);
    knob.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    knob.setRange(-1, 1);
    knob.setValue(0);

    knob.setLookAndFeel(&knobLookAndFeel);
    /*
    addAndMakeVisible(secondKnob);
    secondKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalDrag);
    secondKnob.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    secondKnob.setRange(-1, 1);
    secondKnob.setValue(0);*/
}

JustAKnobAudioProcessorEditor::~JustAKnobAudioProcessorEditor()
{
}

//==============================================================================
void JustAKnobAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    //g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    g.fillAll(juce::Colours::white);
    g.setColour (juce::Colours::green);
    g.setFont (15.0f);
   // g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void JustAKnobAudioProcessorEditor::resized()
{
    knob.setBounds(20, 20, 400, 400);
    secondKnob.setBounds(20, 20, 400, 400);
}

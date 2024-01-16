/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
GainAAudioProcessorEditor::GainAAudioProcessorEditor (GainAAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (200, 200);

    //Slider object parameters
    midiVolume.setSliderStyle(juce::Slider::LinearBarVertical);
    midiVolume.setRange(0.0, 127.0, 1.0);
    midiVolume.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    midiVolume.setPopupDisplayEnabled(true, false, this);
    midiVolume.setTextValueSuffix("Volume");
    midiVolume.setValue(1.0);

    // this function add the slider to the GUI
    addAndMakeVisible(&midiVolume);

    // add listener to constructor
    midiVolume.addListener(this);
}

GainAAudioProcessorEditor::~GainAAudioProcessorEditor()
{
}

//==============================================================================
void GainAAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);

    // Plugin Displayed Name
    g.drawFittedText ("Gain", 0,0, getWidth(),30, juce::Justification::centred, 1);
}

void GainAAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    // sets the position and size of th slider with arguments (x, y, width, and height)
    midiVolume.setBounds(40, 30, 20, getHeight() - 60);
}

// listener function
void GainAAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    audioProcessor.noteOnVel = midiVolume.getValue();
}

/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
GainCAudioProcessorEditor::GainCAudioProcessorEditor (GainCAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (500, 200);

    //Slider Style
    gainSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 75, 15);
    gainSlider.setRange(-48.0, 24.0);
    gainSlider.setValue(-1.0);
    gainSlider.addListener(this);

    addAndMakeVisible(gainSlider);

    /*//Lable Style for future use
    gainLabel.setEditable(true);
    gainLabel.addListener(this);
    addAndMakeVisible(gainLabel);*/
}

GainCAudioProcessorEditor::~GainCAudioProcessorEditor()
{
}

//==============================================================================
void GainCAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    //g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId)); 

    //Custom Background
    background = juce::ImageCache::getFromMemory(BinaryData::GainPluginBackground_001_png, BinaryData::GainPluginBackground_001_pngSize);
    g.drawImageWithin(background, 0, 0, getWidth(), getHeight(), juce::RectanglePlacement::stretchToFit);
}

void GainCAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    
    //gainSlider.setBounds(getLocalBounds());
    gainSlider.setBounds(24, 129, 450, 50);

    // for future use in making a label that is manually placeable
    //gainLabel.setBounds(100, 100, 100, 100);
}

void GainCAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &gainSlider)
    {
        audioProcessor.rawVolume = pow(10, gainSlider.getValue() / 20);
    }
}

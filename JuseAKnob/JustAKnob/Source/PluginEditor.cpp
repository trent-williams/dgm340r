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
    knob.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    knob.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 0, 0);
    knob.setRange(0, 100);
    knob.setValue(0);
    
    knobFilmRoll = juce::ImageCache::getFromMemory(BinaryData::Green_Knob_png, BinaryData::Green_Knob_pngSize);
    
}

JustAKnobAudioProcessorEditor::~JustAKnobAudioProcessorEditor()
{
}

//==============================================================================
//FILMSTRIP
FilmStripSlider::FilmStripSlider(juce::Image* _knobStrip) : knobStrip(_knobStrip)
{
    if (knobStrip->getWidth() > knobStrip->getHeight())
    {
        frameCount = knobStrip->getWidth() / knobStrip->getHeight();
        frameSize = knobStrip->getHeight();
        isVerticalStrip = false;
    }
    else
    {
        frameCount = knobStrip->getHeight() / knobStrip->getWidth();
        frameSize = knobStrip->getWidth();
        isVerticalStrip = true;
    }
}

void FilmStripSlider::drawFrame(juce::Graphics& g, int x, int y, int width, int height, juce::Slider& slider, double position)
{
    const double div = slider.getMaximum() / frameCount;
    double pos = (int)(position / div);

    if (pos > 0)
        pos = pos - 1;

    if (width != height)
    {
        x = (width / 2) - (height / 2);
        width = height;
    }

    if (isVerticalStrip)
    {
        g.drawImage(*knobStrip, x, y, width, height, 0, (int)(pos * frameSize), frameSize, frameSize, false);
    }
    else
    {
        g.drawImage(*knobStrip, x, y, width, height, (int)(pos * frameSize), 0, frameSize, frameSize, false);
    }
}
//==============================================================================
void JustAKnobAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::white);
    g.setColour (juce::Colours::green);
    g.setFont (15.0f);

    //FILMROLL STUFF
    FilmStripSlider fssKnob(&knobFilmRoll);
    fssKnob.drawFrame(g, 20, 20, 250, 250, knob, knob.getValue());
}

void JustAKnobAudioProcessorEditor::resized()
{
    knob.setBounds(20, 20, 400, 400);
    secondKnob.setBounds(20, 20, 400, 400);
}

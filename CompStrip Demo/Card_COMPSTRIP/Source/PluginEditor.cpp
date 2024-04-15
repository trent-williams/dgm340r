/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AHCompStripAudioProcessorEditor::AHCompStripAudioProcessorEditor (AHCompStripAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{


    panDial.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    panDial.setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 50);
    panDial.setDoubleClickReturnValue(true, 0.0);
    addAndMakeVisible(panDial);
    panDial.setLookAndFeel(&filmStripLookAndFeel);
    panAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "panner", panDial);



    gainFader.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    gainFader.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 50);
    gainFader.setNumDecimalPlacesToDisplay(2);
    gainFader.setDoubleClickReturnValue(true, 0.0);
    gainFader.setLookAndFeel(&sliderLookAndFeel);
    addAndMakeVisible(gainFader);
    faderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "fader", gainFader);



    compInput.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    compInput.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 75, 25);
    compInput.setDoubleClickReturnValue(true, 0.0);
    compInput.setTextValueSuffix(" dB");
    addAndMakeVisible(compInput);
    compInput.setLookAndFeel(&filmStripLookAndFeel);
    addAndMakeVisible(compInputLabel);
    compInputLabel.setText("Input", juce::dontSendNotification);
    compInputLabel.attachToComponent(&compInput, true);
    inputAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "input", compInput);


    compThresh.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    compThresh.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 75, 25);
    compThresh.setDoubleClickReturnValue(true, 0.0);
    compThresh.setTextValueSuffix(" dB");
    addAndMakeVisible(compThresh);
    compThresh.setLookAndFeel(&filmStripLookAndFeel);
    addAndMakeVisible(compThreshLabel);
    compThreshLabel.setText("Thresh", juce::dontSendNotification);
    compThreshLabel.attachToComponent(&compThresh, true);
    threshAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "thresh", compThresh);



    compRatio.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    compRatio.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 75, 25);
    compRatio.setDoubleClickReturnValue(true, 1.0);
    addAndMakeVisible(compRatio);
    compRatio.setLookAndFeel(&filmStripLookAndFeel);
    addAndMakeVisible(compRatioLabel);
    compRatioLabel.setText("Ratio", juce::dontSendNotification);
    compRatioLabel.attachToComponent(&compRatio, true);
    ratioAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "ratio", compRatio);



    compAttack.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    compAttack.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 75, 25);
    compAttack.setDoubleClickReturnValue(true, 10.0);
    compAttack.setTextValueSuffix(" ms");
    addAndMakeVisible(compAttack);
    compAttack.setLookAndFeel(&filmStripLookAndFeel);
    addAndMakeVisible(compAttackLabel);
    compAttackLabel.setText("Attack", juce::dontSendNotification);
    compAttackLabel.attachToComponent(&compAttack, true);
    attackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "attack", compAttack);




    compRelease.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    compRelease.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 75, 25);
    compRelease.setDoubleClickReturnValue(true, 125.0);
    compRelease.setTextValueSuffix(" ms");
    addAndMakeVisible(compRelease);
    compRelease.setLookAndFeel(&filmStripLookAndFeel);
    addAndMakeVisible(compReleaseLabel);
    compReleaseLabel.setText("Release", juce::dontSendNotification);
    compReleaseLabel.attachToComponent(&compRelease, true);
    releaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "release", compRelease);




    compOutput.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    compOutput.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 75, 25);
    compOutput.setDoubleClickReturnValue(true, 0.0);
    compOutput.setTextValueSuffix(" dB");
    addAndMakeVisible(compOutput);
    compOutput.setLookAndFeel(&filmStripLookAndFeel);
    addAndMakeVisible(compOutputLabel);
    compOutputLabel.setText("Output", juce::dontSendNotification);
    compOutputLabel.attachToComponent(&compOutput, true);
    outputAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "output", compOutput);

    //========================================================================
    //IMAGES AND SUCH
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (800, 600);

    //knobFilmRoll = juce::ImageCache::getFromMemory(BinaryData::Hue_Shift_Orange_Knog_png, BinaryData::Hue_Shift_Orange_Knog_pngSize);
}


AHCompStripAudioProcessorEditor::~AHCompStripAudioProcessorEditor()
{
}

//==============================================================================
// FILMSTRIP CODE

//==============================================================================
void AHCompStripAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::green);

    //g.drawImageWithin(background, 0, 0, getWidth(), getHeight(), juce::RectanglePlacement::stretchToFit);

    //g.drawImageWithin(background, 0, 0, getWidth(), getHeight(), juce::RectanglePlacement::stretchToFit);

    //==============================================================================
    //FILM STRIP PAINT
}

void AHCompStripAudioProcessorEditor::resized()
{
    panDial.setBounds((getWidth() / 2) - 50, (getHeight() / 10) - 25, 100, 100);

    gainFader.setBounds((getWidth() / 2) - 50, (getHeight() / 3) - 50, 100, 400);

    compInput.setBounds((getWidth() / 2) - 250, (getHeight() / 3) - 150, 100, 125);

    compThresh.setBounds((getWidth() / 2) - 250, (getHeight() / 3), 100, 125);

    compRatio.setBounds((getWidth() / 2) - 250, (getHeight() / 3) + 150, 100, 125);

    compAttack.setBounds((getWidth() / 2) + 175, (getHeight() / 3) - 150, 100, 125);

    compRelease.setBounds((getWidth() / 2) + 175, (getHeight() / 3), 100, 125);

    compOutput.setBounds((getWidth() / 2) + 175, (getHeight() / 3) + 150, 100, 125);




    
    
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
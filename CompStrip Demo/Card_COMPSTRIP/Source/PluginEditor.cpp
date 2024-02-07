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
    //panDial.setRange(0.0, (M_PI / 2));
    //panDial.setValue(M_PI / 4);
    panDial.setDoubleClickReturnValue(true, 0.0);
    //panDial.setPopupDisplayEnabled(true, true, this, 2000);
    //panDial.addListener(this);
    addAndMakeVisible(panDial);
    panAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "panner", panDial);



    gainFader.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    gainFader.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 50);
    //gainFader.setRange(-60.0, 10.0);
    // the two setValue's below are to fix a glitch where the audio wouldn't pass through until a value was changed
    //gainFader.setValue(-0.1);
    //gainFader.setValue(0.0);
    gainFader.setNumDecimalPlacesToDisplay(2);
    gainFader.setDoubleClickReturnValue(true, 0.0);
    //gainFader.addListener(this);
    addAndMakeVisible(gainFader);
    faderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "fader", gainFader);



    compInput.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    compInput.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 75, 25);
    //compInput.setRange(-60.0, 10.0);
    //compInput.setValue(0.0);
    compInput.setDoubleClickReturnValue(true, 0.0);
    compInput.setTextValueSuffix(" dB");
    //compInput.addListener(this);
    addAndMakeVisible(compInput);
    addAndMakeVisible(compInputLabel);
    compInputLabel.setText("Input", juce::dontSendNotification);
    compInputLabel.attachToComponent(&compInput, true);
    inputAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "input", compInput);


    compThresh.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    compThresh.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 75, 25);
    //compThresh.setRange(-60.0, 10.0);
    //compThresh.setValue(0.0);
    compThresh.setDoubleClickReturnValue(true, 0.0);
    compThresh.setTextValueSuffix(" dB");
    //compThresh.addListener(this);
    addAndMakeVisible(compThresh);
    addAndMakeVisible(compThreshLabel);
    compThreshLabel.setText("Thresh", juce::dontSendNotification);
    compThreshLabel.attachToComponent(&compThresh, true);
    threshAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "thresh", compThresh);



    compRatio.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    compRatio.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 75, 25);
    //compRatio.setRange(1.0, 20.0);
    //compRatio.setValue(1.0);
    compRatio.setDoubleClickReturnValue(true, 1.0);
    //compRatio.addListener(this);
    addAndMakeVisible(compRatio);
    addAndMakeVisible(compRatioLabel);
    compRatioLabel.setText("Ratio", juce::dontSendNotification);
    compRatioLabel.attachToComponent(&compRatio, true);
    ratioAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "ratio", compRatio);



    compAttack.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    compAttack.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 75, 25);
    //compAttack.setRange(0.01, 100.0);
    //compAttack.setValue(10.0);
    compAttack.setDoubleClickReturnValue(true, 10.0);
    compAttack.setTextValueSuffix(" ms");
    //compAttack.addListener(this);
    addAndMakeVisible(compAttack);
    addAndMakeVisible(compAttackLabel);
    compAttackLabel.setText("Attack", juce::dontSendNotification);
    compAttackLabel.attachToComponent(&compAttack, true);
    attackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "attack", compAttack);




    compRelease.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    compRelease.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 75, 25);
    //compRelease.setRange(5.0, 2000.0);
    //compRelease.setValue(125.0);
    compRelease.setDoubleClickReturnValue(true, 125.0);
    compRelease.setTextValueSuffix(" ms");
    //compRelease.addListener(this);
    addAndMakeVisible(compRelease);
    addAndMakeVisible(compReleaseLabel);
    compReleaseLabel.setText("Release", juce::dontSendNotification);
    compReleaseLabel.attachToComponent(&compRelease, true);
    releaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "release", compRelease);




    compOutput.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    compOutput.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 75, 25);
    //compOutput.setRange(0.0, 60.0);
    //setValue(0.0);
    compOutput.setDoubleClickReturnValue(true, 0.0);
    compOutput.setTextValueSuffix(" dB");
    //compOutput.addListener(this);
    addAndMakeVisible(compOutput);
    addAndMakeVisible(compOutputLabel);
    compOutputLabel.setText("Output", juce::dontSendNotification);
    compOutputLabel.attachToComponent(&compOutput, true);
    outputAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "output", compOutput);



    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (800, 600);
}


AHCompStripAudioProcessorEditor::~AHCompStripAudioProcessorEditor()
{
}

//==============================================================================
void AHCompStripAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    //g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(juce::Colours::purple);

    //g.setColour (juce::Colours::white);
    //g.setFont (15.0f);
    //g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void AHCompStripAudioProcessorEditor::resized()
{
    panDial.setBounds((getWidth() / 2) - 50, (getHeight() / 10) - 25, 100, 100);

    gainFader.setBounds((getWidth() / 2) - 50, (getHeight() / 3) - 50, 100, 400);

    compInput.setBounds((getWidth() / 2) - 250, (getHeight() / 3) - 150, 100, 100);

    compThresh.setBounds((getWidth() / 2) - 250, (getHeight() / 3) - 50, 100, 100);

    compRatio.setBounds((getWidth() / 2) - 250, (getHeight() / 3) + 50, 100, 100);

    compAttack.setBounds((getWidth() / 2) + 175, (getHeight() / 3) - 150, 100, 100);

    compRelease.setBounds((getWidth() / 2) + 175, (getHeight() / 3) - 50, 100, 100);

    compOutput.setBounds((getWidth() / 2) + 175, (getHeight() / 3) + 50, 100, 100);




    
    
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}


/*
void AHCompStripAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{

    // Here we attach the GUI value to the processor, so that a value change in the GUI results in a value change in the audio
    if (slider == &panDial)
    {
        //audioProcessor.rawVolume = pow(10, gainSlider.getValue() / 20.0);
        //audioProcessor.panValue = panDial.getValue();
        //audioProcessor.panPercent = panSlider.getValue()/((M_PI/2) / 100);
    }

    else if (slider == &gainFader)
    {
        // this math makes the fader work in deciBels
        //audioProcessor.volumeValue = pow(10, gainFader.getValue() / 20.0);
    }

    else if (slider == &compInput)
    {
        audioProcessor.cInput = compInput.getValue();
    }
}
*/

/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/

class PANAudioProcessor : public juce::AudioProcessor
{
public: 
    PANAudioProcessor()
    {
        juce::AudioParameterFloat* gain = new juce::AudioParameterFloat("gain", "Gain", 0.0f, 1.0f, 0.5f);
    }
};
class PANAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    PANAudioProcessor();
    ~PANAudioProcessor() override {}

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:
    //==============================================================================
    //juce::AudioParameterFloat* gain;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PANAudioProcessor)
};

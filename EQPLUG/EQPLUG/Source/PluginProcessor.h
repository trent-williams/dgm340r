/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

enum Slope
{
    Slope_12,
    Slope_24,
    Slope_36,
    Slope_48
};

struct ChainSettings
{
    float peakFreq{ 0 }, peakGainInDecibels{ 0 }, peakQ{ 1.f };
    float lowCutFreq{ 0 }, highCutFreq{ 0 };
    int lowCutSlope{ Slope::Slope_12 }, highCutSlope{ Slope::Slope_12 };
};

ChainSettings getChainSettings(juce::AudioProcessorValueTreeState& treeState);

//==============================================================================
/**
*/
class EQPLUGAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    EQPLUGAudioProcessor();
    ~EQPLUGAudioProcessor() override;

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

    //NEW STUFF=====================================================================
    static juce::AudioProcessorValueTreeState::ParameterLayout creatParameterLayout();
    juce::AudioProcessorValueTreeState treeState{ *this, nullptr, "Parameter", creatParameterLayout() };

private:
    //Setup EQ Filter Chains and Aliases for easy reference
    using Filter = juce::dsp::IIR::Filter<float>;

    using CutFilter = juce::dsp::ProcessorChain<Filter, Filter, Filter, Filter>;

    using MonoChain = juce::dsp::ProcessorChain<CutFilter, Filter, CutFilter>;

    MonoChain leftChain, rightChain;

    enum ChainPositions
    {
        LowCut,
        Peak,
        HighCut
    };

    //Custom Funcitons to save space, times, and trees
    void updatePeakFilter(const ChainSettings& chainSettings);
    using Coefficients = Filter::CoefficientsPtr;
    static void updateCoefficients(Coefficients& old, const Coefficients& replacements);

    template<typename ChainType, typename CoefficientType>
   
    void updateCutFilter(ChainType& leftLowCut, 
        const CoefficientType& cutCoefficients,
        //const ChainSettings& chainSettings,
        const Slope& lowCutSlope)
    {
        leftLowCut.template setBypassed<0>(true);
        leftLowCut.template setBypassed<1>(true);
        leftLowCut.template setBypassed<2>(true);
        leftLowCut.template setBypassed<3>(true);

        switch ( lowCutSlope )
        {
        case Slope_12:
        {
            *leftLowCut.template get<0>().coefficients = *cutCoefficients[0];
            leftLowCut.template setBypassed<0>(false);
            break;
        }

        case Slope_24:
        {
            *leftLowCut.template get<0>().coefficients = *cutCoefficients[0];
            leftLowCut.template setBypassed<0>(false);
            *leftLowCut.template get<1>().coefficients = *cutCoefficients[1];
            leftLowCut.template setBypassed<1>(false);
            break;
        }
        case Slope_36:
        {
            *leftLowCut.template get<0>().coefficients = *cutCoefficients[0];
            leftLowCut.template setBypassed<0>(false);
            *leftLowCut.template get<1>().coefficients = *cutCoefficients[1];
            leftLowCut.template setBypassed<1>(false);
            *leftLowCut.template get<2>().coefficients = *cutCoefficients[2];
            leftLowCut.template setBypassed<2>(false);
            break;
        }
        case Slope_48:
        {
            *leftLowCut.template get<0>().coefficients = *cutCoefficients[0];
            leftLowCut.template setBypassed<0>(false);
            *leftLowCut.template get<1>().coefficients = *cutCoefficients[1];
            leftLowCut.template setBypassed<1>(false);
            *leftLowCut.template get<2>().coefficients = *cutCoefficients[2];
            leftLowCut.template setBypassed<2>(false);
            *leftLowCut.template get<3>().coefficients = *cutCoefficients[3];
            leftLowCut.template setBypassed<3>(false);
            break;
        }
        }
    }

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EQPLUGAudioProcessor)
};

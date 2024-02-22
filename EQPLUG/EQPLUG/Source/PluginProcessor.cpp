/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
EQPLUGAudioProcessor::EQPLUGAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

EQPLUGAudioProcessor::~EQPLUGAudioProcessor()
{
}

//==============================================================================
const juce::String EQPLUGAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool EQPLUGAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool EQPLUGAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool EQPLUGAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double EQPLUGAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int EQPLUGAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int EQPLUGAudioProcessor::getCurrentProgram()
{
    return 0;
}

void EQPLUGAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String EQPLUGAudioProcessor::getProgramName (int index)
{
    return {};
}

void EQPLUGAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void EQPLUGAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = 1;
    spec.sampleRate = sampleRate;

    leftChain.prepare(spec);
    rightChain.prepare(spec);

    auto chainSettings = getChainSettings(treeState);

    updatePeakFilter(chainSettings);
    
    //You may be asking what the heck is this crap. First of all, dito. Sedcond of all, the coefficient
    //is the thing that does the math, so we need to feed it all the info and it will spit out the
    //adjusted audio. The last bit is taking the index of the choices that we defined elsewhere, adding
    //1 and then multiplying by 2. This is because of weirdness in the backend coding for the
    //coefficients.
    auto cutCoefficients = juce::dsp::FilterDesign<float>::designIIRHighpassHighOrderButterworthMethod(chainSettings.lowCutFreq,
        sampleRate, 
        2 * (chainSettings.lowCutSlope + 1));

    auto& leftLowCut = leftChain.get<ChainPositions::LowCut>();

    leftLowCut.setBypassed<0>(true);
    leftLowCut.setBypassed<1>(true);
    leftLowCut.setBypassed<2>(true);
    leftLowCut.setBypassed<3>(true);

    switch (chainSettings.lowCutSlope)
    {
    case Slope_12:
    {
        *leftLowCut.get<0>().coefficients = *cutCoefficients[0];
        leftLowCut.setBypassed<0>(false);
        break;
    }
        
    case Slope_24:
    {
        *leftLowCut.get<0>().coefficients = *cutCoefficients[0];
        leftLowCut.setBypassed<0>(false);
        *leftLowCut.get<1>().coefficients = *cutCoefficients[0];
        leftLowCut.setBypassed<1>(false);
        break;
    }
    case Slope_36:
    {
        *leftLowCut.get<0>().coefficients = *cutCoefficients[0];
        leftLowCut.setBypassed<0>(false);
        *leftLowCut.get<1>().coefficients = *cutCoefficients[0];
        leftLowCut.setBypassed<1>(false);
        *leftLowCut.get<2>().coefficients = *cutCoefficients[0];
        leftLowCut.setBypassed<2>(false);
        break;
    }
    case Slope_48:
    {
        *leftLowCut.get<0>().coefficients = *cutCoefficients[0];
        leftLowCut.setBypassed<0>(false);
        *leftLowCut.get<1>().coefficients = *cutCoefficients[0];
        leftLowCut.setBypassed<1>(false);
        *leftLowCut.get<2>().coefficients = *cutCoefficients[0];
        leftLowCut.setBypassed<2>(false);
        *leftLowCut.get<3>().coefficients = *cutCoefficients[0];
        leftLowCut.setBypassed<3>(false);
        break;
    }
    }

    auto& rightLowCut = rightChain.get<ChainPositions::LowCut>();

    rightLowCut.setBypassed<0>(true);
    rightLowCut.setBypassed<1>(true);
    rightLowCut.setBypassed<2>(true);
    rightLowCut.setBypassed<3>(true);

    switch (chainSettings.lowCutSlope)
    {
    case Slope_12:
    {
        *rightLowCut.get<0>().coefficients = *cutCoefficients[0];
        rightLowCut.setBypassed<0>(false);
        break;
    }

    case Slope_24:
    {
        *rightLowCut.get<0>().coefficients = *cutCoefficients[0];
        rightLowCut.setBypassed<0>(false);
        *rightLowCut.get<1>().coefficients = *cutCoefficients[0];
        rightLowCut.setBypassed<1>(false);
        break;
    }
    case Slope_36:
    {
        *rightLowCut.get<0>().coefficients = *cutCoefficients[0];
        rightLowCut.setBypassed<0>(false);
        *rightLowCut.get<1>().coefficients = *cutCoefficients[0];
        rightLowCut.setBypassed<1>(false);
        *rightLowCut.get<2>().coefficients = *cutCoefficients[0];
        rightLowCut.setBypassed<2>(false);
        break;
    }
    case Slope_48:
    {
        *rightLowCut.get<0>().coefficients = *cutCoefficients[0];
        rightLowCut.setBypassed<0>(false);
        *rightLowCut.get<1>().coefficients = *cutCoefficients[0];
        rightLowCut.setBypassed<1>(false);
        *rightLowCut.get<2>().coefficients = *cutCoefficients[0];
        rightLowCut.setBypassed<2>(false);
        *rightLowCut.get<3>().coefficients = *cutCoefficients[0];
        rightLowCut.setBypassed<3>(false);
        break;
    }
    }
}

void EQPLUGAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool EQPLUGAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void EQPLUGAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    //Updatign Parameters
    auto chainSettings = getChainSettings(treeState);

    updatePeakFilter(chainSettings);

    

    //A lot of pourly written garbage, but hey it works.
    auto cutCoefficients = juce::dsp::FilterDesign<float>::designIIRHighpassHighOrderButterworthMethod(chainSettings.lowCutFreq,
        getSampleRate(),
        2 * (chainSettings.lowCutSlope + 1));

    auto& leftLowCut = leftChain.get<ChainPositions::LowCut>();

    leftLowCut.setBypassed<0>(true);
    leftLowCut.setBypassed<1>(true);
    leftLowCut.setBypassed<2>(true);
    leftLowCut.setBypassed<3>(true);

    switch (chainSettings.lowCutSlope)
    {
    case Slope_12:
    {
        *leftLowCut.get<0>().coefficients = *cutCoefficients[0];
        leftLowCut.setBypassed<0>(false);
        break;
    }

    case Slope_24:
    {
        *leftLowCut.get<0>().coefficients = *cutCoefficients[0];
        leftLowCut.setBypassed<0>(false);
        *leftLowCut.get<1>().coefficients = *cutCoefficients[0];
        leftLowCut.setBypassed<1>(false);
        break;
    }
    case Slope_36:
    {
        *leftLowCut.get<0>().coefficients = *cutCoefficients[0];
        leftLowCut.setBypassed<0>(false);
        *leftLowCut.get<1>().coefficients = *cutCoefficients[0];
        leftLowCut.setBypassed<1>(false);
        *leftLowCut.get<2>().coefficients = *cutCoefficients[0];
        leftLowCut.setBypassed<2>(false);
        break;
    }
    case Slope_48:
    {
        *leftLowCut.get<0>().coefficients = *cutCoefficients[0];
        leftLowCut.setBypassed<0>(false);
        *leftLowCut.get<1>().coefficients = *cutCoefficients[0];
        leftLowCut.setBypassed<1>(false);
        *leftLowCut.get<2>().coefficients = *cutCoefficients[0];
        leftLowCut.setBypassed<2>(false);
        *leftLowCut.get<3>().coefficients = *cutCoefficients[0];
        leftLowCut.setBypassed<3>(false);
        break;
    }
    }

    auto& rightLowCut = rightChain.get<ChainPositions::LowCut>();

    rightLowCut.setBypassed<0>(true);
    rightLowCut.setBypassed<1>(true);
    rightLowCut.setBypassed<2>(true);
    rightLowCut.setBypassed<3>(true);

    switch (chainSettings.lowCutSlope)
    {
    case Slope_12:
    {
        *rightLowCut.get<0>().coefficients = *cutCoefficients[0];
        rightLowCut.setBypassed<0>(false);
        break;
    }

    case Slope_24:
    {
        *rightLowCut.get<0>().coefficients = *cutCoefficients[0];
        rightLowCut.setBypassed<0>(false);
        *rightLowCut.get<1>().coefficients = *cutCoefficients[0];
        rightLowCut.setBypassed<1>(false);
        break;
    }
    case Slope_36:
    {
        *rightLowCut.get<0>().coefficients = *cutCoefficients[0];
        rightLowCut.setBypassed<0>(false);
        *rightLowCut.get<1>().coefficients = *cutCoefficients[0];
        rightLowCut.setBypassed<1>(false);
        *rightLowCut.get<2>().coefficients = *cutCoefficients[0];
        rightLowCut.setBypassed<2>(false);
        break;
    }
    case Slope_48:
    {
        *rightLowCut.get<0>().coefficients = *cutCoefficients[0];
        rightLowCut.setBypassed<0>(false);
        *rightLowCut.get<1>().coefficients = *cutCoefficients[0];
        rightLowCut.setBypassed<1>(false);
        *rightLowCut.get<2>().coefficients = *cutCoefficients[0];
        rightLowCut.setBypassed<2>(false);
        *rightLowCut.get<3>().coefficients = *cutCoefficients[0];
        rightLowCut.setBypassed<3>(false);
        break;
    }
    }
    
    //Hardcoded processing for each of the left and right channels
    juce::dsp::AudioBlock<float> block(buffer);

    auto leftBlock = block.getSingleChannelBlock(0);
    auto rightBlock = block.getSingleChannelBlock(1);

    juce::dsp::ProcessContextReplacing<float> leftContext(leftBlock);
    juce::dsp::ProcessContextReplacing<float> rightContext(rightBlock);

    leftChain.process(leftContext);
    rightChain.process(rightContext);
}

//==============================================================================
bool EQPLUGAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* EQPLUGAudioProcessor::createEditor()
{
    //return new EQPLUGAudioProcessorEditor (*this);
    return new juce::GenericAudioProcessorEditor(*this);
}

//==============================================================================
void EQPLUGAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void EQPLUGAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//NEW STUFF====================================================================
ChainSettings getChainSettings(juce::AudioProcessorValueTreeState& treeState)
{
    ChainSettings settings;

    settings.lowCutFreq = treeState.getRawParameterValue("lowcutfreq")->load();
    settings.highCutFreq = treeState.getRawParameterValue("highcutfreq")->load();
    settings.peakFreq = treeState.getRawParameterValue("peakfreq")->load();
    settings.peakGainInDecibels = treeState.getRawParameterValue("peakgain")->load();
    settings.peakQ = treeState.getRawParameterValue("peakq")->load();
    settings.lowCutSlope = static_cast<Slope>(treeState.getRawParameterValue("lowcutslope")->load());
    settings.highCutSlope = static_cast<Slope>(treeState.getRawParameterValue("highcutslope")->load());

    return settings;
}

void EQPLUGAudioProcessor::updatePeakFilter(const ChainSettings& chainSettings)
{
    auto peakCoefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(getSampleRate(),
        chainSettings.peakFreq,
        chainSettings.peakQ,
        juce::Decibels::decibelsToGain(chainSettings.peakGainInDecibels));

    //*leftChain.get<ChainPositions::Peak>().coefficients = *peakCoefficients;
    //*rightChain.get<ChainPositions::Peak>().coefficients = *peakCoefficients;
    updateCoefficients(leftChain.get<ChainPositions::Peak>().coefficients, peakCoefficients);
    updateCoefficients(rightChain.get<ChainPositions::Peak>().coefficients, peakCoefficients);
}

void EQPLUGAudioProcessor::updateCoefficients(Coefficients &old, const Coefficients &replacements)
{
    *old = *replacements;
}

juce::AudioProcessorValueTreeState::ParameterLayout EQPLUGAudioProcessor::creatParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add(std::make_unique<juce::AudioParameterFloat>("lowcutfreq",
        "Low Cut Freq",
        juce::NormalisableRange<float>(20.f, 20000.f, 1.f, 1.f),
        20.f));

    layout.add(std::make_unique<juce::AudioParameterFloat>("highcutfreq",
        "High Cut Freq",
        juce::NormalisableRange<float>(20.f, 20000.f, 1.f, 1.f),
        20.f));

    layout.add(std::make_unique<juce::AudioParameterFloat>("peakfreq",
        "Peak Freq",
        juce::NormalisableRange<float>(20.f, 20000.f, 1.f, 1.f),
        750.f));

    layout.add(std::make_unique<juce::AudioParameterFloat>("peakgain",
        "Peak Gain",
        juce::NormalisableRange<float>(-24.f, 24.f, 0.5f, 1.f),
        0.f));

    layout.add(std::make_unique<juce::AudioParameterFloat>("peakq",
        "Q",
        juce::NormalisableRange<float>(0.1f, 10.f, 0.05f, 1.f),
        0.7f));

    juce::StringArray stringArray;
    for (int i = 0; i < 4; ++i)
    {
        juce::String str;
        str << (12 + i * 12);
        str << " db/Oct";
        stringArray.add(str);
    }

    layout.add(std::make_unique<juce::AudioParameterChoice>("lowcutslope",
        "Low Cut Slope",
        stringArray,
        0));

    layout.add(std::make_unique<juce::AudioParameterChoice>("highcutslope",
        "High Cut Slope",
        stringArray,
        0));

    return layout;
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new EQPLUGAudioProcessor();
}

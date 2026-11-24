#include "PluginProcessor.h"
#include "PluginEditor.h"

namespace {
class WingSound final : public juce::SynthesiserSound
{
public:
    bool appliesToNote(int) override { return true; }
    bool appliesToChannel(int) override { return true; }
};
juce::AudioProcessorValueTreeState::ParameterLayout makeLayout()
{
    using P = juce::AudioProcessorValueTreeState;
    P::ParameterLayout p;
    p.add(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID {"vowel", 1}, "Vowel", juce::StringArray {"A", "E", "I", "O", "U"}, 0));
    p.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID {"formant", 1}, "Formant", juce::NormalisableRange<float>(0.0f, 1.0f), 0.48f));
    p.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID {"breath", 1}, "Breath", juce::NormalisableRange<float>(0.0f, 1.0f), 0.12f));
    p.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID {"warmth", 1}, "Warmth", juce::NormalisableRange<float>(0.0f, 1.0f), 0.58f));
    p.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID {"vibrato", 1}, "Vibrato", juce::NormalisableRange<float>(0.0f, 1.0f), 0.22f));
    p.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID {"attack", 1}, "Attack", juce::NormalisableRange<float>(0.005f, 1.5f, 0.001f), 0.08f));
    p.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID {"release", 1}, "Release", juce::NormalisableRange<float>(0.03f, 2.0f, 0.001f), 0.32f));
    p.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID {"output", 1}, "Output", juce::NormalisableRange<float>(0.0f, 1.0f), 0.78f));
    return p;
}
}

WingVoice::WingVoice(juce::AudioProcessorValueTreeState& s) : state(s) {}
float WingVoice::value(const char* id) const { if (auto* v = state.getRawParameterValue(id)) return v->load(); return 0.0f; }
void WingVoice::prepare(double rate, int, int) { sampleRate = rate; envelope.setSampleRate(rate); }
void WingVoice::startNote(int midiNote, float velocity, juce::SynthesiserSound*, int)
{
    noteHz = juce::MidiMessage::getMidiNoteInHertz(midiNote); velocityLevel = velocity; phase = 0.0; releasing = false;
    envParams.attack = value("attack"); envParams.decay = 0.18f; envParams.sustain = 0.88f; envParams.release = value("release");
    envelope.setParameters(envParams); envelope.noteOn();
}
void WingVoice::stopNote(float, bool tail)
{
    if (tail) { envParams.release = value("release"); envelope.setParameters(envParams); envelope.noteOff(); releasing = true; }
    else { envelope.reset(); clearCurrentNote(); releasing = false; }
}
void WingVoice::renderNextBlock(juce::AudioBuffer<float>& buffer, int start, int count)
{
    const auto vowel = juce::jlimit(0, 4, (int) value("vowel"));
    const float form = value("formant"), breath = value("breath"), warmth = value("warmth"), vib = value("vibrato");
    const float out = value("output") * 0.24f;
    const float centers[5][4] = {{700,1100,2450,3300},{500,1900,2600,3400},{300,2200,3000,3600},{500,900,2400,3300},{400,800,2200,3000}};
    const float shifts[4] = {0.78f + form * 0.52f, 0.82f + form * 0.44f, 0.92f + form * 0.3f, 1.0f + form * 0.18f};
    const float f0 = noteHz * (1.0f + form * 0.06f);
    for (int i = 0; i < count; ++i)
    {
        if (!envelope.isActive()) { clearCurrentNote(); break; }
        const float env = envelope.getNextSample();
        vibratoPhase += 5.2 / sampleRate;
        const float hz = f0 * (1.0f + std::sin(vibratoPhase * juce::MathConstants<double>::twoPi) * vib * 0.012f);
        const int harmonics = juce::jlimit(8, 48, (int)(sampleRate / (2.2 * hz)));
        float sample = 0.0f;
        for (int h = 1; h <= harmonics; ++h)
        {
            const float freq = hz * (float) h;
            float weight = 0.0f;
            for (int j = 0; j < 4; ++j)
            {
                const float center = centers[vowel][j] * shifts[j];
                const float width = center * (0.20f + 0.13f * warmth);
                const float d = (freq - center) / width;
                weight += (j == 0 ? 1.0f : 0.74f) * std::exp(-0.5f * d * d);
            }
            const float tilt = std::pow((float) h, -(1.08f + warmth * 0.65f));
            sample += std::sin(phase * h) * weight * tilt;
        }
        phase += juce::MathConstants<double>::twoPi * hz / sampleRate;
        if (phase > juce::MathConstants<double>::twoPi) phase = std::fmod(phase, juce::MathConstants<double>::twoPi);
        noiseState = noiseState * 1664525u + 1013904223u;
        const float airy = ((float)(noiseState >> 8) / 8388607.5f - 1.0f) * breath * 0.06f;
        sample = std::tanh(sample * (1.45f + warmth) * 0.12f + airy) * env * velocityLevel * out;
        for (int ch = 0; ch < buffer.getNumChannels(); ++ch) buffer.addSample(ch, start + i, sample);
    }
    juce::ignoreUnused(releasing);
}

WingSynthAudioProcessor::WingSynthAudioProcessor()
 : AudioProcessor(BusesProperties().withOutput("Output", juce::AudioChannelSet::stereo(), true)), parameters(*this, nullptr, "PARAMETERS", makeLayout())
{
    for (int i = 0; i < 12; ++i) synth.addVoice(new WingVoice(parameters));
    synth.addSound(new WingSound());
}
juce::AudioProcessorValueTreeState::ParameterLayout WingSynthAudioProcessor::createParameters() { return makeLayout(); }
void WingSynthAudioProcessor::prepareToPlay(double rate, int block) { synth.setCurrentPlaybackSampleRate(rate); for (int i=0;i<synth.getNumVoices();++i) if (auto* v=dynamic_cast<WingVoice*>(synth.getVoice(i))) v->prepare(rate,block,getTotalNumOutputChannels()); }
void WingSynthAudioProcessor::releaseResources() {}
bool WingSynthAudioProcessor::isBusesLayoutSupported(const BusesLayout& l) const { return l.getMainOutputChannelSet() == juce::AudioChannelSet::mono() || l.getMainOutputChannelSet() == juce::AudioChannelSet::stereo(); }
void WingSynthAudioProcessor::processBlock(juce::AudioBuffer<float>& b, juce::MidiBuffer& midi) { juce::ScopedNoDenormals noDenormals; b.clear(); synth.renderNextBlock(b,midi,0,b.getNumSamples()); }
juce::AudioProcessorEditor* WingSynthAudioProcessor::createEditor() { return new WingSynthAudioProcessorEditor(*this); }
void WingSynthAudioProcessor::getStateInformation(juce::MemoryBlock& d) { auto s=parameters.copyState(); std::unique_ptr<juce::XmlElement> x(s.createXml()); copyXmlToBinary(*x,d); }
void WingSynthAudioProcessor::setStateInformation(const void* data,int size) { std::unique_ptr<juce::XmlElement> x(getXmlFromBinary(data,size)); if(x && x->hasTagName(parameters.state.getType())) parameters.replaceState(juce::ValueTree::fromXml(*x)); }
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() { return new WingSynthAudioProcessor(); }

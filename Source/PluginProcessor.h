#pragma once
#include <JuceHeader.h>

class WingVoice final : public juce::SynthesiserVoice
{
public:
    explicit WingVoice(juce::AudioProcessorValueTreeState& state);
    bool canPlaySound(juce::SynthesiserSound*) override { return true; }
    void startNote(int midiNote, float velocity, juce::SynthesiserSound*, int) override;
    void stopNote(float velocity, bool allowTailOff) override;
    void pitchWheelMoved(int) override {}
    void controllerMoved(int, int) override {}
    void renderNextBlock(juce::AudioBuffer<float>&, int start, int count) override;
    void prepare(double sampleRate, int samplesPerBlock, int outputChannels);
    void setCurrentPlaybackSampleRate(double rate) override { sampleRate = rate; }
private:
    float value(const char* id) const;
    juce::AudioProcessorValueTreeState& state;
    double sampleRate = 44100.0;
    double phase = 0.0, vibratoPhase = 0.0;
    float noteHz = 440.0f, amp = 0.0f, velocityLevel = 0.0f;
    bool releasing = false;
    uint32_t noiseState = 0x1234567u;
    juce::ADSR envelope;
    juce::ADSR::Parameters envParams;
};

class WingSynthAudioProcessor final : public juce::AudioProcessor
{
public:
    WingSynthAudioProcessor();
    ~WingSynthAudioProcessor() override = default;
    void prepareToPlay(double, int) override;
    void releaseResources() override;
    bool isBusesLayoutSupported(const BusesLayout&) const override;
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }
    const juce::String getName() const override { return "Wing Synth"; }
    bool acceptsMidi() const override { return true; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 2.0; }
    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int) override {}
    const juce::String getProgramName(int) override { return {}; }
    void changeProgramName(int, const juce::String&) override {}
    void getStateInformation(juce::MemoryBlock&) override;
    void setStateInformation(const void*, int) override;
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameters();
    juce::AudioProcessorValueTreeState parameters;
private:
    juce::Synthesiser synth;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WingSynthAudioProcessor)
};

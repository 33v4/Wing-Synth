#pragma once
#include "PluginProcessor.h"
#include <array>

class WingSynthAudioProcessorEditor final : public juce::AudioProcessorEditor
{
public:
    explicit WingSynthAudioProcessorEditor(WingSynthAudioProcessor&);
    ~WingSynthAudioProcessorEditor() override;
    void paint(juce::Graphics&) override;
    void resized() override;
private:
    WingSynthAudioProcessor& processor;
    juce::ComboBox vowel;
    juce::Label vowelLabel;
    juce::Slider formant, breath, warmth, vibrato, attack, release, output;
    juce::Label title, subtitle, footer;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> vowelAttach;
    std::array<std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>, 7> attachments;
    void setupSlider(juce::Slider&, juce::Label&, const juce::String&);
    std::array<juce::Label,7> labels;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WingSynthAudioProcessorEditor)
};

cmake_minimum_required(VERSION 3.22)
project(WingSynth VERSION 1.0.0 LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(JUCE
  GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
  GIT_TAG 8.0.3
  GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(JUCE)

juce_add_plugin(WingSynth
  COMPANY_NAME "Wing Synth"
  BUNDLE_ID "com.wingsynth.plugin"
  PLUGIN_MANUFACTURER_CODE WngS
  PLUGIN_CODE Wng1
  FORMATS VST3
  PRODUCT_NAME "Wing Synth"
  DESCRIPTION "Expressive vowel synthesizer"
  IS_SYNTH TRUE
  NEEDS_MIDI_INPUT TRUE
  NEEDS_MIDI_OUTPUT FALSE
  EDITOR_WANTS_KEYBOARD_FOCUS FALSE
  COPY_PLUGIN_AFTER_BUILD FALSE
  VST3_CATEGORIES Instrument Synth)

target_sources(WingSynth PRIVATE Source/PluginProcessor.cpp Source/PluginEditor.cpp)
target_compile_features(WingSynth PRIVATE cxx_std_17)
target_compile_definitions(WingSynth PUBLIC JUCE_WEB_BROWSER=0 JUCE_USE_CURL=0)
target_link_libraries(WingSynth PRIVATE juce::juce_audio_utils juce::juce_dsp PUBLIC juce::juce_recommended_config_flags juce::juce_recommended_lto_flags juce::juce_recommended_warning_flags)

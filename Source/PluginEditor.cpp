#include "PluginEditor.h"

namespace { const juce::Colour ink {0xff291c27}, wine {0xff901f39}, rose {0xffd64f65}, cream {0xfffff4e7}, gold {0xffe5bb79}; }
WingSynthAudioProcessorEditor::WingSynthAudioProcessorEditor(WingSynthAudioProcessor& p) : AudioProcessorEditor(&p), processor(p)
{
    setSize(860, 540);
    title.setText("WING", juce::dontSendNotification); title.setFont(juce::Font(32.0f, juce::Font::bold)); title.setColour(juce::Label::textColourId, cream); addAndMakeVisible(title);
    subtitle.setText("S Y N T H   /   VOCAL INSTRUMENT", juce::dontSendNotification); subtitle.setFont(juce::Font(11.0f)); subtitle.setColour(juce::Label::textColourId, gold); addAndMakeVisible(subtitle);
    vowel.addItemList({"A  ·  OPEN","E  ·  BRIGHT","I  ·  PURE","O  ·  ROUND","U  ·  DEEP"},1); vowel.setColour(juce::ComboBox::backgroundColourId,juce::Colour(0xff422831)); vowel.setColour(juce::ComboBox::textColourId,cream); vowel.setColour(juce::ComboBox::outlineColourId,juce::Colour(0xffb57677)); addAndMakeVisible(vowel);
    vowelLabel.setText("SINGING VOWEL",juce::dontSendNotification); vowelLabel.setFont(juce::Font(10.0f,juce::Font::bold)); vowelLabel.setColour(juce::Label::textColourId,gold); addAndMakeVisible(vowelLabel);
    vowelAttach=std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(processor.parameters,"vowel",vowel);
    setupSlider(formant,labels[0],"FORMANT"); setupSlider(breath,labels[1],"BREATH"); setupSlider(warmth,labels[2],"WARMTH"); setupSlider(vibrato,labels[3],"VIBRATO"); setupSlider(attack,labels[4],"ATTACK"); setupSlider(release,labels[5],"RELEASE"); setupSlider(output,labels[6],"OUTPUT");
    const char* ids[] = {"formant","breath","warmth","vibrato","attack","release","output"}; juce::Slider* sliders[]={&formant,&breath,&warmth,&vibrato,&attack,&release,&output};
    for(size_t i=0;i<attachments.size();++i) attachments[i]=std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor.parameters,ids[i],*sliders[i]);
    footer.setText("MIDI IN   •   12 VOICES   •   PLAY A CHORD OR MELODY",juce::dontSendNotification); footer.setFont(juce::Font(10.0f,juce::Font::bold)); footer.setColour(juce::Label::textColourId,juce::Colour(0xffbb9694)); addAndMakeVisible(footer);
}
WingSynthAudioProcessorEditor::~WingSynthAudioProcessorEditor() = default;
void WingSynthAudioProcessorEditor::setupSlider(juce::Slider& s,juce::Label& l,const juce::String& name)
{
    s.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag); s.setTextBoxStyle(juce::Slider::TextBoxBelow,false,66,19); s.setColour(juce::Slider::rotarySliderFillColourId,rose); s.setColour(juce::Slider::rotarySliderOutlineColourId,juce::Colour(0xff63434d)); s.setColour(juce::Slider::thumbColourId,gold); s.setColour(juce::Slider::textBoxTextColourId,cream); s.setColour(juce::Slider::textBoxBackgroundColourId,juce::Colour(0xff39252e)); s.setColour(juce::Slider::textBoxOutlineColourId,juce::Colours::transparentBlack); addAndMakeVisible(s);
    l.setText(name,juce::dontSendNotification); l.setJustificationType(juce::Justification::centred); l.setFont(juce::Font(10.0f,juce::Font::bold)); l.setColour(juce::Label::textColourId,cream); addAndMakeVisible(l);
}
void WingSynthAudioProcessorEditor::resized()
{
    auto r=getLocalBounds(); title.setBounds(40,24,240,42); subtitle.setBounds(42,66,290,22); vowelLabel.setBounds(42,122,210,22); vowel.setBounds(42,148,224,42);
    const int x0=300,y0=118,w=126,h=142; juce::Slider* s[]={&formant,&breath,&warmth,&vibrato,&attack,&release,&output};
    for(int i=0;i<7;++i){int col=i%4,row=i/4;int x=x0+col*w,y=y0+row*168;labels[(size_t)i].setBounds(x,y,w,20);s[i]->setBounds(x+8,y+24,110,110);}
    footer.setBounds(42,496,480,20);
}
void WingSynthAudioProcessorEditor::paint(juce::Graphics& g)
{
    auto b=getLocalBounds().toFloat(); g.fillAll(ink); g.setColour(juce::Colour(0xff38252e)); g.fillRoundedRectangle(b.reduced(14),20);
    g.setColour(wine); g.fillRoundedRectangle(juce::Rectangle<float>(18,18,260,504),18);
    g.setColour(juce::Colour(0xff5c2636)); g.fillRoundedRectangle(juce::Rectangle<float>(286,96,550,392),18);
    g.setColour(gold.withAlpha(.55f)); g.drawRoundedRectangle(juce::Rectangle<float>(18,18,824,504),18,1.2f);
    // Minimal stage portrait: dark curls, warm face, crimson floral dress.
    g.setColour(juce::Colour(0xff211a24)); g.fillEllipse(69,190,162,168); g.fillRoundedRectangle(juce::Rectangle<float>(63,236,175,151),48);
    g.setColour(juce::Colour(0xffffd9b2)); g.fillEllipse(86,205,129,152);
    g.setColour(juce::Colour(0xff33222b)); g.fillEllipse(79,190,145,42); g.fillEllipse(58,214,54,108); g.fillEllipse(185,214,48,108);
    g.setColour(juce::Colour(0xff64434a)); g.drawLine(114,276,134,279,2); g.drawLine(164,279,184,276,2); g.drawLine(147,283,143,311,2);
    g.setColour(juce::Colour(0xffb84c70)); g.drawLine(130,323,163,323,3);
    juce::Path dress; dress.startNewSubPath(84,352); dress.lineTo(58,387); dress.lineTo(48,474); dress.lineTo(224,474); dress.lineTo(213,387); dress.lineTo(188,352); dress.closeSubPath(); g.setColour(wine); g.fillPath(dress);
    g.setColour(juce::Colour(0xffffd9b2)); g.fillRoundedRectangle(juce::Rectangle<float>(48,374,67,24),11); g.fillRoundedRectangle(juce::Rectangle<float>(166,374,59,24),11);
    for(int i=0;i<11;++i){float x=65+(i*47)%145,y=392+(i*31)%72; g.setColour(cream);g.fillEllipse(x,y,10,10);g.setColour(gold);g.fillEllipse(x+3,y+3,4,4);}
    g.setColour(gold); g.setFont(12.0f); g.drawText("THE VOICE OF",54,162,190,20,juce::Justification::centred);
    g.setColour(cream); g.setFont(juce::Font(15.0f,juce::Font::bold)); g.drawText("W I N G",54,470,190,24,juce::Justification::centred);
    g.setColour(gold.withAlpha(.45f)); for(int i=0;i<5;++i) g.drawEllipse(726+i*18,44,5,5,1);
    g.setColour(cream); g.setFont(11); g.drawText("SHAPE YOUR SONG",306,64,240,20,juce::Justification::left);
}

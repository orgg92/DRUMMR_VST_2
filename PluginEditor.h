/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Drummr.h"

//==============================================================================
/**
*/
class DRUMMR_VST_2AudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::Timer
{
public:

    juce::Image step_inactive = juce::ImageCache::getFromMemory(BinaryData::step_inactive_png, BinaryData::step_inactive_pngSize);
    juce::Image step_low = juce::ImageCache::getFromMemory(BinaryData::step_low_png, BinaryData::step_low_pngSize);
    juce::Image step_medium = juce::ImageCache::getFromMemory(BinaryData::step_medium_png, BinaryData::step_medium_pngSize);
    juce::Image step_high = juce::ImageCache::getFromMemory(BinaryData::step_high_png, BinaryData::step_high_pngSize);

    juce::Image select_inactive = juce::ImageCache::getFromMemory(BinaryData::SELECT_BTN_png, BinaryData::SELECT_BTN_pngSize);
    juce::Image select_active = juce::ImageCache::getFromMemory(BinaryData::SELECT_BTN_ACTIVE_png, BinaryData::SELECT_BTN_ACTIVE_pngSize);

    juce::Image one = juce::ImageCache::getFromMemory(BinaryData::_1_png, BinaryData::_1_pngSize);
    juce::Image two = juce::ImageCache::getFromMemory(BinaryData::_2_png, BinaryData::_2_pngSize);
    juce::Image three = juce::ImageCache::getFromMemory(BinaryData::_3_png, BinaryData::_3_pngSize);
    juce::Image four = juce::ImageCache::getFromMemory(BinaryData::_4_png, BinaryData::_4_pngSize);
    juce::Image five = juce::ImageCache::getFromMemory(BinaryData::_5_png, BinaryData::_5_pngSize);
    juce::Image six = juce::ImageCache::getFromMemory(BinaryData::_6_png, BinaryData::_6_pngSize);
    juce::Image seven = juce::ImageCache::getFromMemory(BinaryData::_7_png, BinaryData::_7_pngSize);
    juce::Image eight = juce::ImageCache::getFromMemory(BinaryData::_8_png, BinaryData::_8_pngSize);

    juce::Image one_active = juce::ImageCache::getFromMemory(BinaryData::_1_ACTIVE_png, BinaryData::_1_ACTIVE_pngSize);
    juce::Image two_active = juce::ImageCache::getFromMemory(BinaryData::_2_ACTIVE_png, BinaryData::_2_ACTIVE_pngSize);
    juce::Image three_active = juce::ImageCache::getFromMemory(BinaryData::_3_ACTIVE_png, BinaryData::_3_ACTIVE_pngSize);
    juce::Image four_active = juce::ImageCache::getFromMemory(BinaryData::_4_ACTIVE_png, BinaryData::_4_ACTIVE_pngSize);
    juce::Image five_active = juce::ImageCache::getFromMemory(BinaryData::_5_ACTIVE_png, BinaryData::_5_ACTIVE_pngSize);
    juce::Image six_active = juce::ImageCache::getFromMemory(BinaryData::_6_ACTIVE_png, BinaryData::_6_ACTIVE_pngSize);
    juce::Image seven_active = juce::ImageCache::getFromMemory(BinaryData::_7_ACTIVE_png, BinaryData::_7_ACTIVE_pngSize);
    juce::Image eight_active = juce::ImageCache::getFromMemory(BinaryData::_8_ACTIVE_png, BinaryData::_8_ACTIVE_pngSize);

    juce::Image step_indicator_active = juce::ImageCache::getFromMemory(BinaryData::step_indicator_active_png, BinaryData::step_indicator_active_pngSize);
    juce::Image step_indicator_inactive = juce::ImageCache::getFromMemory(BinaryData::step_indicator_inactive_png, BinaryData::step_indicator_inactive_pngSize);

    juce::Image randomize = juce::ImageCache::getFromMemory(BinaryData::RANDOMIZE_png, BinaryData::RANDOMIZE_pngSize);
    juce::Image alter = juce::ImageCache::getFromMemory(BinaryData::ALTER_png, BinaryData::ALTER_pngSize);
    juce::Image clear = juce::ImageCache::getFromMemory(BinaryData::CLEAR_png, BinaryData::CLEAR_pngSize);

    juce::Image velocity_mode_low = juce::ImageCache::getFromMemory(BinaryData::velocity_mode_low_png, BinaryData::velocity_mode_low_pngSize);
    juce::Image velocity_mode_medium = juce::ImageCache::getFromMemory(BinaryData::velocity_mode_medium_png, BinaryData::velocity_mode_medium_pngSize);
    juce::Image velocity_mode_high = juce::ImageCache::getFromMemory(BinaryData::velocity_mode_high_png, BinaryData::velocity_mode_high_pngSize);

    juce::Image run_active = juce::ImageCache::getFromMemory(BinaryData::RUN_BTN_ACTIVE_png, BinaryData::RUN_BTN_ACTIVE_pngSize);
    juce::Image run_inactive = juce::ImageCache::getFromMemory(BinaryData::RUN_BTN_INACTIVE_png, BinaryData::RUN_BTN_INACTIVE_pngSize);

    juce::Colour clrWhite = juce::Colour(0xffffff);

    std::stringstream ss;

    juce::Label patternLabel;
    juce::Label drumLabels[10];

    juce::ImageButton selectButtonArray[10];
    juce::ImageButton stepButtonArray[16];
    juce::ImageButton runButton;
    juce::ImageButton velocityButton[3];
    juce::ImageButton patternButton[8];
    juce::ImageButton stepIndicatorArray[16];

    StepSequencer stepSequencer;

    juce::Label stepsLabel;
    juce::Slider stepsSlider;

    juce::Slider quantizeSlider;

    juce::ImageButton sceneButtonArray[4];
    juce::Label sceneLabel;

    juce::TextEditor midiMessagesBox;

    int midiChannel = 10;
    double startTime;

    DRUMMR_VST_2AudioProcessorEditor (DRUMMR_VST_2AudioProcessor&);
    ~DRUMMR_VST_2AudioProcessorEditor() override;

    //==============================================================================
    //void paint (juce::Graphics&) override;
    void resized() override;
    void timerCallback() override;
    void start();
    void stop();
    void setNoteNumber(int noteNumber);
    void changeStepLength(int stepLength);
    void activateStep(int stepNumber);
    void setDrumNumber(int drumNumber);
    void setRunMode();
    void clearPattern();
    void resetPattern();
    void loadPattern();
    void loadDrumPattern();
    void changeQuantizeLength(int value);
    juce::Image getVelocityImage();
    juce::Image getVelocityModeImage(int mode);
    juce::Image getVelocityImageWithParam(Drum drum);
    void setVelocityMode(int mode);
    void logMessage(const juce::String& m);
    void addMessageToList(const juce::MidiMessage& message);
    static juce::String getMidiMessageDescription(const juce::MidiMessage& m);
    void UpdateUIElements(int stepNumber);



private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    DRUMMR_VST_2AudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DRUMMR_VST_2AudioProcessorEditor)
};


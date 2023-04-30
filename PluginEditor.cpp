/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"



//==============================================================================
DRUMMR_VST_2AudioProcessorEditor::DRUMMR_VST_2AudioProcessorEditor (DRUMMR_VST_2AudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    


    addAndMakeVisible(runButton);
    runButton.onClick = [this] { setRunMode(); };
    runButton.setImages(false, true, true, run_inactive, 1.0f, clrWhite, run_inactive, 1.0f, clrWhite, run_inactive, 1.0f, clrWhite);

    //stepSequencer.MidiClock.run();

    // velocity selector buttons

    for (int i = 0; i < 3; i++)
    {
        addAndMakeVisible(velocityButton[i]);
    }

    // undo to here

    velocityButton[0].onClick = [this] { setVelocityMode(0); };
    velocityButton[0].setImages(false, true, true,
        step_low,
        1.0f, clrWhite,
        step_low,
        1.0f, clrWhite,
        step_low,
        1.0f, clrWhite);

    velocityButton[1].onClick = [this] { setVelocityMode(1); };
    velocityButton[1].setImages(false, true, true,
        step_medium,
        1.0f, clrWhite,
        step_medium,
        1.0f, clrWhite,
        step_medium,
        1.0f, clrWhite);

    velocityButton[2].onClick = [this] { setVelocityMode(2); };
    velocityButton[2].setImages(false, true, true,
        step_high,
        1.0f, clrWhite,
        step_high,
        1.0f, clrWhite,
        step_high,
        1.0f, clrWhite);


    // drum selector buttons

    for (int i = 0; i < 10; i++) {
        addAndMakeVisible(selectButtonArray[i]);
        selectButtonArray[i].setButtonText("Drum1 (36)");
        selectButtonArray[i].onClick = [this, i] { setDrumNumber(i); };
        selectButtonArray[i].setImages(false, true, true,
            select_inactive,
            1.0f, juce::Colour(0xffffff),
            select_inactive,
            1.0f, juce::Colour(0xffffff),
            select_inactive,
            1.0f, juce::Colour(0xffffff));
    }

    selectButtonArray[0].setImages(false, true, true, select_active, 1.0f, juce::Colour(0x880808), select_active,
        1.0f, juce::Colour(0xffffff),
        select_active,
        1.0f, juce::Colour(0xffffff));

    //addAndMakeVisible (drum2Button);
    //drum2Button.setButtonText ("Drum2 (37)");
    //drum2Button.onClick = [this] { setDrumNumber (2); };

    // EO drum selectors

    // Stepper buttons

    for (int i = 0; i < 16; i++) {
        addAndMakeVisible(stepButtonArray[i]);
        stepButtonArray[i].setImages(false, true, true,
            step_inactive,
            1.0f, clrWhite,
            step_inactive,
            1.0f, clrWhite,
            step_inactive,
            1.0f, clrWhite);

        stepButtonArray[i].onClick = [this, i] { activateStep(i); };
    }

    // EO stepper buttons


    for (int i = 0; i < 4; i++) {
        addAndMakeVisible(stepButtonArray[i]);
        sceneButtonArray[i].setImages(false, true, true,
            step_inactive,
            1.0f, clrWhite,
            step_inactive,
            1.0f, clrWhite,
            step_inactive,
            1.0f, clrWhite);

        stepButtonArray[i].onClick = [this, i] { activateStep(i); };
    }

    // Scene buttons

    addAndMakeVisible(sceneButtonArray[0]);
    sceneButtonArray[0].setImages(false, true, true,
        one_active,
        1.0f, clrWhite,
        one_active,
        1.0f, clrWhite,
        one_active,
        1.0f, clrWhite);

    addAndMakeVisible(sceneButtonArray[1]);
    sceneButtonArray[1].setImages(false, true, true,
        two,
        1.0f, clrWhite,
        two,
        1.0f, clrWhite,
        two,
        1.0f, clrWhite);

    addAndMakeVisible(sceneButtonArray[2]);
    sceneButtonArray[2].setImages(false, true, true,
        three,
        1.0f, clrWhite,
        three,
        1.0f, clrWhite,
        three,
        1.0f, clrWhite);

    addAndMakeVisible(sceneButtonArray[3]);
    sceneButtonArray[3].setImages(false, true, true,
        four,
        1.0f, clrWhite,
        four,
        1.0f, clrWhite,
        four,
        1.0f, clrWhite);


    // Step indicator buttons


    for (int i = 0; i < 16; i++) {
        addAndMakeVisible(stepIndicatorArray[i]);
        stepIndicatorArray[i].setImages(false, true, true,
            step_indicator_inactive,
            1.0f, clrWhite,
            step_indicator_inactive,
            1.0f, clrWhite,
            step_indicator_inactive,
            1.0f, clrWhite);

    }

    stepIndicatorArray[0].setImages(false, true, true,
        step_indicator_active,
        1.0f, clrWhite,
        step_indicator_active,
        1.0f, clrWhite,
        step_indicator_active,
        1.0f, clrWhite);

    for (int i = 0; i < 10; i++)
    {
        addAndMakeVisible(drumLabels[i]);
        ss << "Drum " << (i + 1);
        drumLabels[i].setText(ss.str(), juce::dontSendNotification);
        ss.str("");

    }



    addAndMakeVisible(stepsLabel);
    stepsLabel.setText("Steps: ", juce::dontSendNotification);

    stepsSlider.setSliderStyle(juce::Slider::IncDecButtons);
    addAndMakeVisible(stepsSlider);
    stepsSlider.setRange(0, 128, 1);
    stepsSlider.onValueChange = [this]
    {
        auto message = juce::MidiMessage::controllerEvent (midiChannel, 7, (int) stepsSlider.getValue());
        message.setTimeStamp (juce::Time::getMillisecondCounterHiRes() * 0.001 - startTime);
        addMessageToList (message);
        changeStepLength(stepsSlider.getValue());
    };

    addAndMakeVisible(sceneLabel);
    sceneLabel.setText("Scene:", juce::dontSendNotification);

    //addAndMakeVisible(quantizeSlider);
    ////quantizeSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 10, 10);
    //quantizeSlider.setSliderStyle(juce::Slider::Rotary);
    //quantizeSlider.setRange(0, 6, 1);
    //quantizeSlider.setValue(stepSequencer.QuantizeMode);
    //quantizeSlider.onValueChange = [this]
    //{
    //    changeQuantizeLength(quantizeSlider.getValue());

    //};


    addAndMakeVisible(midiMessagesBox);
    midiMessagesBox.setMultiLine(true);
    midiMessagesBox.setReturnKeyStartsNewLine(true);
    midiMessagesBox.setReadOnly(true);
    midiMessagesBox.setScrollbarsShown(true);
    midiMessagesBox.setCaretVisible(false);
    midiMessagesBox.setPopupMenuEnabled(true);
    midiMessagesBox.setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x32ffffff));
    midiMessagesBox.setColour(juce::TextEditor::outlineColourId, juce::Colour(0x1c000000));
    midiMessagesBox.setColour(juce::TextEditor::shadowColourId, juce::Colour(0x16000000));

    setSize(800, 300);
}

DRUMMR_VST_2AudioProcessorEditor::~DRUMMR_VST_2AudioProcessorEditor()
{
}

//==============================================================================


void DRUMMR_VST_2AudioProcessorEditor::resized()
{
        auto halfWidth = getWidth() / 2;

        auto buttonsBounds = getLocalBounds().withWidth (halfWidth).reduced (10);
        int buttonWidth = 50;
        int buttonHeight = 30;
        int xpos = 100;

        runButton.setBounds(15, 250, 75, 50);

        //drum1Button   .setBounds (buttonWidth, 10,  buttonWidth, buttonHeight);

        for (int i = 0; i < 10; i++) {
            selectButtonArray[i].setBounds(buttonWidth * (i + 1), 30, buttonWidth, buttonHeight);
        }

        for (int i = 0; i < 16; i++) {
            stepIndicatorArray[i].setBounds((30 * (i + 1)) + 36, 68, 8, 8);
        }

        for (int i = 0; i < 16; i++) {
            stepButtonArray[i].setBounds((30 * (i + 1)) + 30, 80, 20, 20);
        }

        for (int i = 0; i < 10; i++) {
            drumLabels[i].setBounds((buttonWidth * (i + 1)), 13, buttonWidth, buttonHeight);
        }

        // velocity buttons


        velocityButton[2].setBounds(550, 68, 12, 12);
        velocityButton[1].setBounds(550, 80, 12, 12);
        velocityButton[0].setBounds(550, 92, 12, 12);

        setVelocityMode(stepSequencer.VelocityMode);

        stepsLabel      .setBounds (xpos, 247, buttonsBounds.getWidth(), 20);
        stepsSlider     .setBounds (xpos, 265, 200, 20);

        stepsSlider.setValue(stepSequencer.StepLength);

        // scene buttons
        int sceneXPos = xpos + 220;
        for (int i = 0; i < 4; i++)
        {
            sceneButtonArray[i].setBounds(sceneXPos + (20 * i), 266, 20, 20);
        };

        sceneLabel.setBounds(sceneXPos + 20, 247, buttonsBounds.getWidth(), 20);

        quantizeSlider.setBounds(xpos, 100, 150, 150);

        midiMessagesBox.setBounds(20, 150, 800, 100);
}

void DRUMMR_VST_2AudioProcessorEditor::start() {
    auto tickTime = 60 / stepSequencer.Tempo;
    DRUMMR_VST_2AudioProcessorEditor::startTimer(500 / stepSequencer.QuantizeMode);
    //updateCurrentTimeInfoFromHost();
};

void DRUMMR_VST_2AudioProcessorEditor::stop() {
    DRUMMR_VST_2AudioProcessorEditor::stopTimer();
}

void DRUMMR_VST_2AudioProcessorEditor::timerCallback()
{
    stepSequencer.IncrementStep();
    UpdateUIElements(stepSequencer.ActiveStep);
    stepSequencer.TriggerStepHits(stepSequencer.ActiveStep);
};

void DRUMMR_VST_2AudioProcessorEditor::UpdateUIElements(int stepNumber)
{
    long test = sizeof(stepSequencer.Steps);
    long test2 = sizeof(stepSequencer.Steps[0]);
    auto test3 = test / test2;

    if (stepNumber == 0)
    {
        stepIndicatorArray[0].setImages(false, true, true, step_indicator_inactive, 1.0f, juce::Colour(0x880808), step_indicator_inactive,
            1.0f, juce::Colour(0xffffff),
            step_indicator_active,
            1.0f, juce::Colour(0xffffff));

        stepIndicatorArray[test3 - 1].setImages(false, true, true, step_indicator_inactive, 1.0f, juce::Colour(0x880808), step_indicator_inactive,
            1.0f, juce::Colour(0xffffff),
            step_indicator_inactive,
            1.0f, juce::Colour(0xffffff));
    }
    else {
        stepIndicatorArray[stepNumber - 1].setImages(false, true, true, step_indicator_inactive, 1.0f, juce::Colour(0x880808), step_indicator_inactive,
            1.0f, juce::Colour(0xffffff),
            step_indicator_inactive,
            1.0f, juce::Colour(0xffffff));
    }



    if (stepNumber < 16)
    {
        stepIndicatorArray[stepNumber].setImages(false, true, true, step_indicator_active, 1.0f, juce::Colour(0x880808), step_indicator_active,
            1.0f, juce::Colour(0xffffff),
            step_indicator_active,
            1.0f, juce::Colour(0xffffff));
    }
    else {
        //stepIndicatorArray[stepNumber - 1].setImages(false, true, true, step_indicator_inactive, 1.0f, juce::Colour(0x880808), step_indicator_inactive,
        //    1.0f, juce::Colour(0xffffff),
        //    step_indicator_inactive,
        //    1.0f, juce::Colour(0xffffff));

        stepIndicatorArray[0].setImages(false, true, true, step_indicator_inactive, 1.0f, juce::Colour(0x880808), step_indicator_inactive,
            1.0f, juce::Colour(0xffffff),
            step_indicator_active,
            1.0f, juce::Colour(0xffffff));
    }

}

void DRUMMR_VST_2AudioProcessorEditor::setNoteNumber(int noteNumber)
{
    auto message = juce::MidiMessage::noteOn(DRUMMR_VST_2AudioProcessorEditor::midiChannel, noteNumber, (juce::uint8)100);
    message.setTimeStamp(juce::Time::getMillisecondCounterHiRes() * 0.001 - startTime);
    addMessageToList(message);
}

void DRUMMR_VST_2AudioProcessorEditor::changeStepLength(int stepLength) {
    stepSequencer.StepLength = stepLength;
}

void DRUMMR_VST_2AudioProcessorEditor::activateStep(int stepNumber)
{
    int velocity = DRUMMR_VST_2AudioProcessorEditor::stepSequencer.getVelocityMode();

    auto currentStep = stepSequencer.getStep(stepNumber);

    if (currentStep.Drums[stepSequencer.ActiveDrum].On == false) {
        stepButtonArray[stepNumber].setImages(false, true, true, getVelocityImage(), 1.0f, juce::Colour(0x880808), getVelocityImage(),
            1.0f, juce::Colour(0xffffff),
            getVelocityImage(),
            1.0f, juce::Colour(0xffffff));

        stepButtonArray[stepNumber].setButtonText("test");
    }
    else {
        stepButtonArray[stepNumber].setImages(false, true, true, step_inactive, 1.0f, juce::Colour(0xffffff), step_inactive,
            1.0f, juce::Colour(0xffffff),
            step_inactive,
            1.0f, juce::Colour(0xffffff));
    }

    stepSequencer.stepClickEvent(stepNumber);
}

void DRUMMR_VST_2AudioProcessorEditor::setDrumNumber(int drumNumber) {
    if (drumNumber != stepSequencer.ActiveDrum) {

        selectButtonArray[stepSequencer.ActiveDrum].setImages(false, true, true, select_inactive, 1.0f, juce::Colour(0x880808), select_inactive,
            1.0f, juce::Colour(0xffffff),
            select_inactive,
            1.0f, juce::Colour(0xffffff));

        stepSequencer.ActiveDrum = drumNumber;

        selectButtonArray[drumNumber].setImages(false, true, true, select_active, 1.0f, juce::Colour(0x880808), select_active,
            1.0f, juce::Colour(0xffffff),
            select_active,
            1.0f, juce::Colour(0xffffff));


        loadDrumPattern();
    }
}

void DRUMMR_VST_2AudioProcessorEditor::setRunMode() {

    if (stepSequencer.Running == true) {
        runButton.setImages(false, true, true, run_inactive, 1.0f, clrWhite, run_inactive, 1.0f, clrWhite, run_inactive, 1.0f, clrWhite);
        stop();
        stepIndicatorArray[stepSequencer.ActiveStep].setImages(false, true, true, step_indicator_inactive, 1.0f, juce::Colour(0x880808), step_indicator_inactive,
            1.0f, juce::Colour(0xffffff),
            step_indicator_active,
            1.0f, juce::Colour(0xffffff));
        stepSequencer.ActiveStep = 0;
    }
    else {
        runButton.setImages(false, true, true, run_active, 1.0f, clrWhite, run_active, 1.0f, clrWhite, run_active, 1.0f, clrWhite);
        start();
    }

    stepSequencer.Running = !stepSequencer.Running;
}

void DRUMMR_VST_2AudioProcessorEditor::clearPattern() {
    for (int i = 0; i < 16; i++) {
        stepButtonArray[i].setImages(false, true, true, step_inactive, 1.0f, juce::Colour(0xffffff), step_inactive,
            1.0f, juce::Colour(0xffffff),
            step_inactive,
            1.0f, juce::Colour(0xffffff));
    }
}

void DRUMMR_VST_2AudioProcessorEditor::resetPattern() {

}

void DRUMMR_VST_2AudioProcessorEditor::loadDrumPattern() {

    clearPattern();

    for (int i = 0; i < 16; i++) {
        Drum drum = stepSequencer.Steps[i].Drums[stepSequencer.ActiveDrum];
        if (drum.On == true)
        {
            stepButtonArray[i].setImages(false, true, true, getVelocityImageWithParam(drum), 1.0f, juce::Colour(0x880808), getVelocityImageWithParam(drum),
                1.0f, juce::Colour(0xffffff),
                getVelocityImageWithParam(drum),
                1.0f, juce::Colour(0xffffff));
        }
    }

}

void DRUMMR_VST_2AudioProcessorEditor::changeQuantizeLength(int value) {
    stepSequencer.QuantizeMode = value;
}

juce::Image DRUMMR_VST_2AudioProcessorEditor::getVelocityImageWithParam(Drum drum) {
    switch (drum.Velocity) {
    case 45:
        return step_low;
        break;
    case 90:
        return step_medium;
        break;
    case 125:
        return step_high;
        break;
    }
}

juce::Image DRUMMR_VST_2AudioProcessorEditor::getVelocityImage() {
    int velocityMode = stepSequencer.getVelocityMode();
    switch (velocityMode) {
    case 0:
        return step_low;
        break;
    case 1:
        return step_medium;
        break;
    case 2:
        return step_high;
        break;
    }
}

juce::Image DRUMMR_VST_2AudioProcessorEditor::getVelocityModeImage(int mode) {
    switch (mode) {
    case 0:
        return velocity_mode_low;
        break;
    case 1:
        return velocity_mode_medium;
        break;
    case 2:
        return velocity_mode_high;
        break;
    }
}

void DRUMMR_VST_2AudioProcessorEditor::setVelocityMode(int mode) {
    juce::Image img = getVelocityImage();

    velocityButton[stepSequencer.VelocityMode].setImages(false, true, true,
        img,
        1.0f, clrWhite,
        img,
        1.0f, clrWhite,
        img,
        1.0f, clrWhite);

    stepSequencer.VelocityMode = mode;
    img = getVelocityModeImage(mode);

    velocityButton[mode].setImages(false, true, true,
        img,
        1.0f, clrWhite,
        img,
        1.0f, clrWhite,
        img,
        1.0f, clrWhite);

}

void DRUMMR_VST_2AudioProcessorEditor::logMessage(const juce::String& m)
{
    midiMessagesBox.moveCaretToEnd();
    midiMessagesBox.insertTextAtCaret(m + juce::newLine);
}

void DRUMMR_VST_2AudioProcessorEditor::addMessageToList(const juce::MidiMessage& message)
{
    auto time = message.getTimeStamp();

    auto hours = ((int)(time / 3600.0)) % 24;
    auto minutes = ((int)(time / 60.0)) % 60;
    auto seconds = ((int)time) % 60;
    auto millis = ((int)(time * 1000.0)) % 1000;

    auto timecode = juce::String::formatted("%02d:%02d:%02d.%03d",
        hours,
        minutes,
        seconds,
        millis);

    logMessage(timecode + "  -  " + getMidiMessageDescription(message));
}

juce::String DRUMMR_VST_2AudioProcessorEditor::getMidiMessageDescription(const juce::MidiMessage& m)
{
    if (m.isNoteOn())           return "Note on " + juce::MidiMessage::getMidiNoteName(m.getNoteNumber(), true, true, 3);
    if (m.isNoteOff())          return "Note off " + juce::MidiMessage::getMidiNoteName(m.getNoteNumber(), true, true, 3);
    if (m.isProgramChange())    return "Program change " + juce::String(m.getProgramChangeNumber());
    if (m.isPitchWheel())       return "Pitch wheel " + juce::String(m.getPitchWheelValue());
    if (m.isAftertouch())       return "After touch " + juce::MidiMessage::getMidiNoteName(m.getNoteNumber(), true, true, 3) + ": " + juce::String(m.getAfterTouchValue());
    if (m.isChannelPressure())  return "Channel pressure " + juce::String(m.getChannelPressureValue());
    if (m.isAllNotesOff())      return "All notes off";
    if (m.isAllSoundOff())      return "All sound off";
    if (m.isMetaEvent())        return "Meta event";

    if (m.isController())
    {
        juce::String name(juce::MidiMessage::getControllerName(m.getControllerNumber()));

        if (name.isEmpty())
            name = "[" + juce::String(m.getControllerNumber()) + "]";

        return "Controller " + name + ": " + juce::String(m.getControllerValue());
    }

    return juce::String::toHexString(m.getRawData(), m.getRawDataSize());
}

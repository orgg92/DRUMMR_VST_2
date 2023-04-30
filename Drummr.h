#pragma once
#include <stdio.h>
#include <windows.h>
#include "JuceHeader.h"

enum QuantizeLengths {
    Quarters = 1,
    Eighths = 2,
    Sixteenths = 3,
    Thirtyseconds = 4
};


class Drum {
public:
    int Velocity;
    bool On = false;
    bool Mute = false;
    bool Solo = false;
    int MIDI_Note;
};

class Step {
public:
    Drum Drums[10];
};

class StepPattern {
public:
    Step Pattern[16];
};

class SequencerPatterns {
public:
    StepPattern StepPatterns[8];
};

class StepSequencer : public juce::Component {
public:

    StepSequencer() {
        SetupMIDINotes();
    };


    Step Steps[16];
    int VelocityMode = 1;
    int Scene = 0;
    bool Running = false;
    int Tempo = 120;
    int ActiveStep = 0;
    int ActiveDrum = 0;
    int ActivePattern = 1;
    int QuantizeMode = 4;
    int StepLength = 16;

    SequencerPatterns SeqeuencerPatterns;

    void RunSequencer() {
        Running = !Running;
    }

    void Tick() {

        double tickTime = (Tempo / 60) * 1000;

    }

    void SetupMIDINotes() {
        int root = 67;
        for (int a = 0; a <= (sizeof(Steps) / sizeof(Steps[0])) - 1; a++) {
            for (int i = 0; i < 10; i++) {
                Steps[a].Drums[i].MIDI_Note = root;
                root++;
            }
            root = 67;
        }
    }

    void IncrementStep() {
        long test = sizeof(Steps);
        long test2 = sizeof(Steps[0]);
        auto test3 = test / test2;

        if (ActiveStep < (sizeof(Steps) / sizeof Steps[0]) - 1)
        {
            ActiveStep++;
        }
        else {
            ActiveStep = 0;
        }

    }

    void TriggerStepHits(int stepNumber) {
        for (int i = 0; i < 10; i++)
        {
            std::stringstream ss;

            if (Steps[stepNumber].Drums[i].On) {
                ss << "Triggering MIDI drum: " << std::to_string(Steps[stepNumber].Drums[i].MIDI_Note) << " with velocity: " << std::to_string(getVelocityValue(Steps[stepNumber].Drums[i].Velocity)) << " for drum: " << i << " on step number: " << stepNumber << " \r\n";
                std::string str = ss.str();
                LPCSTR cstr = str.c_str();
                OutputDebugString(cstr);
                juce::MidiMessage::noteOn(1, Steps[stepNumber].Drums[i].MIDI_Note, (juce::uint8)getVelocityValue(Steps[stepNumber].Drums[i].Velocity));
            }
        }
    }

    void TriggerDrum(int midiChannel, int noteNumber) {
        OutputDebugString("Triggering drum(s) for step \r\n");
        //juce::MidiMessage::noteOn(midiChannel, noteNumber, (juce::uint8)100);
    }

    int getActiveDrum() {
        return ActiveDrum;
    }

    void setActiveDrum(int drumNumber) {
        ActiveDrum = drumNumber;
    }

    int getVelocityMode() {
        return VelocityMode;
    }

    int getVelocityValue(int velocity) {
        int velocityValue = 0;

        switch (velocity) {
        case 0:
            return 45;
            break;
        case 1:
            return 90;
            break;
        case 2:
            return 125;
            break;
        }
    }

    Step getStep(int stepNumber) {
        return Steps[stepNumber];
    }

    void calculateStepLength() {

    }

    void stepClickEvent(int stepNumber) {
        if (Steps[stepNumber].Drums[ActiveDrum].On == false) {
            Steps[stepNumber].Drums[ActiveDrum].On = true;
            Steps[stepNumber].Drums[ActiveDrum].Velocity = getVelocityValue(getVelocityMode());
        }
        else {
            Steps[stepNumber].Drums[ActiveDrum].On = false;
            Steps[stepNumber].Drums[ActiveDrum].Velocity = getVelocityValue(getVelocityMode());
        }
    }

    void setScene(int sceneNo) {
        Scene = sceneNo;
    }

    void resetPattern() {
        Steps->Drums->On = false;
    }

    void setPattern(int patternNo) {

    }
};



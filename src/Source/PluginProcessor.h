/**
 * @file PluginProcessor.h
 * @brief DonAudioProcessor�N���X�p�̃w�b�_�t�@�C���B����JUCE�ɂ�鎩�������B
 * @author h.wakimoto
 * @date 2017/05/01
 */


/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../extern/LUFSMeter/Ebu128LoudnessMeter.h"



//==============================================================================
/**
*/
class DonAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
	DonAudioProcessor();
    ~DonAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioSampleBuffer&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

	//LUFS

	float getShortTermLoudness();
	float getMomentaryLoudness();
	float getIntegratedLoudness();
	float getLoudnessRangeStart();
	float getLoudnessRangeEnd();
	float getLoudnessRange();
	Ebu128LoudnessMeter ebu128LoudnessMeter;

	//Mute
	bool getIsMute() {
		return isMute;
	}
	void setIsMute(bool value) {
		isMute = value;
	}

private:
	/**
	@brief �~���[�g�{�^��
	*/
	bool isMute = false;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DonAudioProcessor)
};

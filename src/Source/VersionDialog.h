/**
 * @file VersionDialog.h
 * @brief �o�[�W�����_�C�A���O�i���g�p�j
 * @author h.wakimoto
 * @date 2017/05/01
 */

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"


//���܂������Ȃ��̂Ŗ��g�p
class VersionDialog : public DialogWindow {
public:
	VersionDialog(const String &name, Colour backgroundColour, bool escapeKeyTriggersCloseButton = true, bool addToDesktop = true);
	~VersionDialog();
	//void resized() override;

private:
	ScopedPointer<Label> lbl;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VersionDialog)
};

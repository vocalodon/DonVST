/**
 * @file DonWebBrowser.h
 * @brief DonWebBrowser�N���X�p�̃w�b�_�t�@�C��
 * @author h.wakimoto
 * @date 2017/05/01
 */


#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

/***
DonWebBrowser
WebBrowser�ŃX�N���[�������������Ƃ��Ƀ����[�h���~�߂鏈�����������߂�
���s���낵�������̂Ƃ��됬�ʂȂ��B���̂Ƃ���WebBrowserComponent�𒼐ڎg���Ă�����
*/
class DonWebBrowser : public WebBrowserComponent {
public:

	/**
	* @brief �C�x���g�n���h��
	* @todo ���̂Ƃ�����Ȃ��i�v�����j
	*/
	void mouseEnter(const MouseEvent &event) {
		if (mouseMoveEvent != nullptr) {
			mouseMoveEvent(false);
		}
	}

	/**
	* @brief �C�x���g�n���h��
	* @todo ���̂Ƃ�����Ȃ��i�v�����j
	*/
	void mouseDown(const MouseEvent &event) { //���Ȃ�
		if (mouseMoveEvent != nullptr) {
			mouseMoveEvent(false);
		}
	}

	/**
	* @brief �C�x���g�n���h��
	* @todo �^�C�~���O���s���R�ŗ��p���Â炢
	*/
	void mouseWheelMove(const MouseEvent &event, const MouseWheelDetails &wheel) { 
		if (mouseMoveEvent != nullptr) {
			mouseMoveEvent(false);
		}
	}

	/**
	* @brief �Ȃ񂩎��s���낵�Ă��r���̃S�~
	*/
	std::function<void(bool)> mouseMoveEvent = nullptr;
};
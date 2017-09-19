/**
* @file ApiAccount.h
* @brief API�A�N�Z�X�ɕK�v�Ȋe��R���e�i�N���X�p�̃w�b�_�t�@�C��
* @author h.wakimoto
* @date 2017/05/01
*/

#pragma once

#include "TootModel.h"

#include <string>

using namespace std;

/***
LoginInfo�N���X�B
AccessToken���擾����܂łɕK�v�ȏ����i�[���邽�߂̃G���e�B�e�B�B
*/
class LoginInfo {
public:

	/**
	* @brief �R���X�g���N�^
	*/	LoginInfo() {

	}

    /**
    * @brief �f�X�g���N�^
    */
	~LoginInfo() {

	}

	/**
	* @brief ��{�������Z�b�g����
	* @detail AuthCode�擾�̂��߂̃T�[�o�ւ̏���A�N�Z�X�p�B�u���E�U�ł̃R�[�h�擾���s��������z��B
	* �p�X���[�h�����̏ꍇ�̓��[�U���ƃp�X���[�h���Z�b�g����K�v�����邪
	* ����Ă݂��Ƃ���Ȃ�������͂��܂��s���Ȃ��������A�Z�L�����e�B�I�ɂ��̂��܂����Ȃ��B
	* @todo HostUrl�ɂ͍Ō�̃X���b�V���������Ă͂Ȃ�Ȃ��̂ł���΍폜����B
	*/
	void setInfo(string pHosturl) {
		hostUrl = pHosturl;
	}

	/**
	* @brief �N���C�A���g�����Z�b�g����
	* @detail AuthCode�擾�̂��߂̃T�[�o�ւ�2��߂̃A�N�Z�X�p�B
	*/
	void setClientInfo(string pClientId, string pClientSecuret) {
		clientId = pClientId;
		clientSecret = pClientSecuret;
	};

	/**
	* @brief authCode��Setter
	*/
	void setAuthCode(string pAuthCode) {
		authCode = pAuthCode;
	}

	/**
	* @brief hostUrl��Getter
	*/
	string getHostUrl() {
		return hostUrl;
	}

	/**
	* @brief clientId��Getter
	*/
	string getClientId() {
		return clientId;
	}

	/**
	* @brief clientSecret��Getter
	*/
	string getClientSecret() {
		return clientSecret;
	}

	/**
	* @brief authCode��Getter
	*/
	string getAuthCode() {
		return authCode;
	}


private:
	//!API�A�N�Z�X����C���X�^���X��URL
	string hostUrl;

	//!�N���C�A���gID(AuthCode���擾���邽�߂ɕK�v)
	string clientId;

	//!�N���C�A���g�V�[�N���b�g�L�[(AuthCode���擾���邽�߂ɕK�v)
	string clientSecret;

	//!Auth Code�B������擾����̂��ړI
	string authCode;
};


/***
AccessInfo�N���X�B
AccessToken���擾���Ă���̏����i�[���邽�߂̃G���e�B�e�B�B
*/
class AccessInfo {

public:

	/**
	* @brief �R���X�g���N�^
	*/
	AccessInfo() {
		account = new DonAccountModel();
	}

	/**
	* @brief �f�X�g���N�^
	*/
	~AccessInfo() {
		delete account;
	}

	/**
	* @brief ��{�����Z�b�g����
	* @detail API�ɃA�N�Z�X����̂ɕK�v�Ȃ̂�URL��AccessToken�B
	* �A�J�E���g����AccessToken�ɕR�t���ăT�[�o�����ێ����Ă���B
	* account�ɂ̓Z�b�^�[�͂Ȃ����A�R���X�g���N�^�Ő����ς݂Ȃ̂ŁAgetAccount()->set???()�Ŋe�p�����[�^���Z�b�g����B
	* @todo HostUrl�ɂ͍Ō�̃X���b�V���������Ă͂Ȃ�Ȃ��̂ł���΍폜����B
	*/
	void setInfo(string pHostUrl, string pAccessToken) {
		this->hostUrl = pHostUrl;
		this->accessToken = pAccessToken;
	}

	/**
    * @brief hostUrl��Getter
	*/
	string getHostUrl() {
		return hostUrl;
	}

	/**
	* @brief accessToken��Getter
	*/
	string getAccessToken() {
		return accessToken;
	}

	/**
	* @brief account��Getter
	*/
	DonAccountModel* getAccount() {
		return account;
	}

private:

	//!API�A�N�Z�X����C���X�^���X��URL
	string hostUrl;

	//!API�A�N�Z�X�ɕK�v�ȃA�N�Z�X�g�[�N��
	string accessToken;

	//!�A�J�E���g���
	DonAccountModel* account;
};



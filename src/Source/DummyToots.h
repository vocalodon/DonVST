/**
 * @file DummyToots.h
 * @brief DummyTootFactory�N���X�p�̃w�b�_�t�@�C��
 * @author h.wakimoto
 * @date 2017/05/01
 */


#pragma once

#include "TootModel.h"
#include "Utils.h"

/***
DummyTootFactory
�������b�Z�[�W�𐶐�����N���X
*/
class DummyTootFactory {
public:
	/**
	* @brief �R���X�g���N�^
	* @param pTimeline �_�~�[�g�D�[�g�𗬂��^�C�����C��
	* @param pAcounts �L���b�V���ς݃A�J�E���g���X�g
	* @param pSelfAccount ���v���C��i��VST���[�U�j�A�J�E���g
	*/
	DummyTootFactory(DonTimeLineModel* pTimeline, DonAccountListModel* pAccounts, DonAccountModel* pSelfAccount, string pLanguage) {
		this->timeline = pTimeline;
		this->accounts = pAccounts;
		this->self_account = pSelfAccount;
		this->language = pLanguage;
	}

	/**
	* @brief �f�X�g���N�^
	*/
	~DummyTootFactory() {
		//
	}

	void setLanguage(string value) {
		this->language = value;
	}

	/**
	* @brief �_�~�[�g�D�[�g�𐶐�
	* @return �_�~�[�g�D�[�g
	* @detail �_�~�[�g�D�[�g��1����������B�������ꂽ�g�D�[�g�̓^�C�����C���ɒǉ������B
	* �O�̂��ߒǉ����ꂽ�g�D�[�g���߂�l�ɂȂ��Ă��邪�A��{�I�ɂ̓^�C�����C�����ĕ`�悷�邾���ŗǂ��B
	* @todo �_�~�[���[�U��id���ǂ����邩�����Bid�͂���قǏd�v�ȏ��ł͂Ȃ����A�L���b�V����ł�
	* �d���`�F�b�N�ɗp�����邽�߁A�������[�U�Əd�����Ă��܂��Ƃ��̃��[�U�ɒu��������Ă��܂��B
	*/
	DonTootModel* makeDummyToot() {

		//!�_�~�[���[�U�̃A�J�E���g��
		string acct;

		//!�_�~�[���[�U�̃A�o�^�[URL
		string avatar_url;

		//!�_�~�[�g�D�[�g�̃|�X�g����
		string created_at = getCurrentTimeJP();

		//!�_�~�[�g�D�[�g�{��
		string content;

		//!�_�~�[���[�U�̕\����
		string display_name;

		//!����������
		Random random = Random();

	//�_�~�[���[�U������
		
		//!�_�~�[���[�U�t�@�C��
		File usersfile(DUMMY_DATA_DIR + "cheerup.users."+language);

		//!�_�~�[���[�U�ꗗ
		StringArray* users = new StringArray();
		usersfile.readLines(*users);

		try {
			const char* delim = ",";

			//!�����_���ɔ����o����1���[�U��CSV���
			string selected_user = users->getReference(random.nextInt(users->size() - 1)).toStdString();

			//!���[�U���B0���A�J�E���g���A1���\����
			vector<string> user = split(selected_user, *delim);
			acct = user[0];
			display_name = user[1];

		}
		catch (const exception& e) {
			showMessageDialog("�G���[�FDT001");
		}

		delete(users);

		if (acct!=""){

			avatar_url = "file://" + DUMMY_ICON_DIR + acct + ".png";

			//!�_�~�[�A�J�E���g�C���X�^���X
			DonAccountModel* account = new DonAccountModel();

			account->setId(random.nextInt(10000)+1000000);//�b��B100�����[�U�����̃C���X�^���X�̏ꍇ�A�������[�U�Əd������\�������邪
														  //�G���[�ɂ͂Ȃ炸���̐l�̃_�~�[�g�D�[�g�Ƃ������ƂɂȂ�B�L���b�V���ς݃��[�U�Ƃ�
														  //�d���̏ꍇ�����Ȃ̂ŉ\���͒Ⴂ�B
			account->setUserName(acct);
			account->setAcct("."+acct+ ".");//�}�X�g�h���̎d�l��h�b�g�t���̃A�J�E���g�͎��݂��Ȃ�
			account->setDisplayName(display_name);
			account->setAvatar(avatar_url);
			account->setURL("");
			account = this->accounts->addAccount(account); //�L���b�V���ɒǉ����A�L���b�V����Ńq�b�g�����炻��ɒu�������B

		//�g�D�[�g�̌���

			//!�_�~�[�g�D�[�g�t�@�C��
			File wordsfile(DUMMY_DATA_DIR + "cheerup.messages."+language);

			//!�_�~�[�g�D�[�g�ꗗ
			StringArray* words = new StringArray();
			wordsfile.readLines(*words);

			try {
				//�����_���ɔ����o����1�_�~�[�g�D�[�g
				content = words->getReference(random.nextInt(words->size() - 1)).toStdString();
				content = Replace(content,"@", this->self_account->getDisplayName());
				content = '@' + this->self_account->getAcct() + " " + content;

			}
			catch (const exception& e) {
				showMessageDialog("�G���[�FDT002");
			}
			delete(words);

			//!�_�~�[�g�D�[�g�C���X�^���X
			DonTootModel* toot = new DonTootModel();
			toot->setAccount(account);
			toot->setId(random.nextInt(1000));//�b��B1000�g�D�[�g�ɖ����Ȃ��C���X�^���X�̏ꍇ�A�ǂݍ��݃^�C�����C������ID�Ƃ��Ԃ�\��������
											 //���̏ꍇ�g�D�[�g���ǉ�����Ȃ��B�܂�1000�g�D�[�g�ȏ�̃_�~�[�ǉ��͂ł��Ȃ��B�i�G���[�ɂ͂Ȃ�Ȃ��j
			toot->setCreatedAt(created_at);
			toot->setVisibility("public");
			toot->setContent(content);

			toot = this->timeline->addToot(toot); //���ꂪ�d�v
			return toot; //������͂��܂�
		}
		else {
			return nullptr;
		}

	}
private:

	//!�_�~�[�g�D�[�g�𗬂��^�C�����C��
	DonTimeLineModel* timeline;

	//!�L���b�V���ς݃A�J�E���g���X�g
	DonAccountListModel* accounts;

	//!�A�C�R���t�@�C���̏ꏊ
	string DUMMY_ICON_DIR = (File::getSpecialLocation(File::currentExecutableFile).getParentDirectory().getFullPathName() + File::separator + "VocalodonVST_files" + File::separator + "images" + File::separator + "icons" + File::separator).toStdString();

	//!���[�U���t�@�C������у_�~�[�g�D�[�g���t�@�C���̒u����
	string DUMMY_DATA_DIR = (File::getSpecialLocation(File::currentExecutableFile).getParentDirectory().getFullPathName() + File::separator + "VocalodonVST_files" + File::separator + "data" + File::separator).toStdString();

	//!���v���C��i��VST���[�U�j�A�J�E���g
	DonAccountModel* self_account;

	//!����
	string language;
};



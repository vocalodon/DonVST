/**
 * @file TootModel.h
 * @brief �}�X�g�h���̃^�C�����C���Ɋւ���e�탂�f���N���X�p�̃w�b�_�t�@�C��
 * @author h.wakimoto
 * @date 2017/05/01
 */


#pragma once

#include <string>
#include <vector>
#include <picojson.h>
#include <functional>
#include <algorithm>

#include "Utils.h"

/*
Toot�֌W�̃��f��

�ŏI�I�ɂ̓t�@�C���������H
���̂Ƃ���w�b�_�ɂ��ׂċL�q���Ă���B

�EDonAccountModel�N���X
�EDonTootModel�N���X
�EDonTimeLineModel�N���X
�EDonAccountListModel�N���X

*/


using namespace std;


typedef enum {
	tl_mode_home,
	tl_mode_local,
	tl_mode_super_local,
	tl_mode_public
}DonTimeLineMode;


/****
DonInstanceModel �N���X
�E�C���X�^���X���̃G���e�B�e�B
*/
//�b��B�ŏI�I�ɂ͑S���̃t�B�[���h������
class DonInstanceModel {
public:
	DonInstanceModel() {

	}
	~DonInstanceModel() {
		//
	}

	string getUri() {
		return uri;
	}
	string getTitle() {
		return title;
	}
	string getDescription(){
		return description;
	}
	string getEmail() {
		return email;
	}
	string getVersion() {
		return version;
	}

	void setUri(string value) {
		uri = value;
	}
	void setTitle(string value) {
		title = value;
	}
	void setDescription(string value) {
		description = value;
	}
	void setEmail(string value) {
		email = value;
	}
	void setVersion(string value) {
		version = value;
	}

private:
	string uri;
	string title;
	string description;
	string email;
	string version;
};

/****
DonAccountModel�N���X

�E���[�U���̃G���e�B�e�B
�E���̃C���X�^���X�̃|�C���^��DonAccountListModel���̃��X�g�ɒǉ������B
�E��L���X�g�ɒǉ�����Ȃ����́i�d���Ȃǁj�͑���delete�����B�������Ǘ��̓��X�g�ōs���B

*/
//�b��B�ŏI�I�ɂ͑S���̃t�B�[���h������
class DonAccountModel {
public:
	DonAccountModel() {
		//
	}
	DonAccountModel(picojson::object obj) {
		id       = stoi(obj["id"].to_str());
		username = obj["username"].to_str();
		acct     = obj["acct"].to_str();
		display_name = obj["display_name"].to_str();
		avatar   = obj["avatar"].to_str();
		url      = obj["url"].to_str();
	}
	~DonAccountModel() {
		//
	}
	unsigned __int64 getId() {
		return id;
	}
	string getUserName() {
		return username;
	}
	string getAcct() {
		return acct;
	}
	string getDisplayName() {
		return display_name;
	}
	string getAvatar() {
		return avatar;
	}
	string getURL() {
		return url;
	}
	void setId(unsigned __int64 value) {
		id = value;
	}
	void setUserName(string value) {
		username = value;
	}
	void setAcct(string value) {
		acct = value;
	}
	void setDisplayName(string value) {
		display_name = value;
	}
	void setAvatar(string value) {
		avatar = value;
	}
	void setURL(string value) {
		url = value;
	}
private:
	unsigned __int64 id;
	string username;
	string acct;
	string display_name;//
	string avatar;//
	string url;//
};

/***
DonMediaAttachmentModel�N���X

�EToot�̓Y�t�t�@�C���B
�EToot1�ɑ΂��đ��ő��݂�����B
�E���X�g��Toot���ێ��B
*/
class DonMediaAttachmentModel {
public:

	DonMediaAttachmentModel(unsigned __int64 pId, string pType, string pPreviewUrl, string pSize) {
		this->id = pId;
		this->type = pType;
		this->preview_url = pPreviewUrl;
		this->size = pSize;
	}
	~DonMediaAttachmentModel() {
		//
	}

	unsigned __int64 getId() {
		return id;
	}
	string getType() {
		return type;
	}
	string getPreviewURL() {
		return preview_url;
	}
	string getSize() {
		return size;
	}
	void setId(unsigned __int64 value) {
		id = value;
	}
	void setType(string value) {
		type = value;
	}
	void setPreviewURL(string value) {
		preview_url = value;
	}
	void setSize(string value) {
		size = value;
	}
private:
	unsigned __int64 id;
	string type;
	string preview_url;
	string size;
};


/****
DonTootModel�N���X

�E�g�D�[�g���̃G���e�B�e�B
�E���̃C���X�^���X�̃|�C���^��DonTimeLineModel���̃��X�g�ɒǉ������B
�E��L���X�g�ɒǉ�����Ȃ����́i�d���Ȃǁj�͑���delete�����B
�E�A�J�E���g�������Ă��邪�A�����DonAccountListModel�ɓ����Ă�����̂�
�@�������̂��Q�Ƃ��Ă���B�������Ǘ��̓��X�g���ōs���B
�EDonMediaAttachment�̃C���X�^���X�̃��X�g��ێ�����B�������Ǘ��͂����ōs���B
�E�u�[�X�g�͖{����DonTootModel�̃l�X�g�\������2�K�w����1���Ɍ��肳��邽�߁A����K�w��ɓW�J���Ă���B
*/
//�b��B�ŏI�I�ɂ͑S���̃t�B�[���h������
class DonTootModel {
public:
	DonTootModel() {
		//
	}
	DonTootModel(picojson::object obj, DonAccountModel* acnt) {
		//���C��Toot
		id = stoull(obj["id"].to_str());
		created_at = time8601toJP(obj["created_at"].to_str());
		visibility = obj["visibility"].to_str();
		account = acnt;
		content = obj["content"].to_str();
		//�u�[�X�g
		if (obj["reblog"].to_str() != "null") {
			is_boost = true;
			picojson::object boost_obj = obj["reblog"].get<picojson::object>();
			picojson::object boost_acnt_obj = boost_obj["account"].get<picojson::object>();
			reblog_acct = boost_acnt_obj["acct"].to_str();
			reblog_name = boost_acnt_obj["display_name"].to_str();
			reblog_avatar = boost_acnt_obj["avatar"].to_str();
			reblog_created_at = time8601toJP(boost_obj["created_at"].to_str());
			reblog_content = boost_obj["content"].to_str();

			picojson::array boost_medias = boost_obj["media_attachments"].get<picojson::array>();
			picojson::array::iterator itr = boost_medias.begin();
			while (itr != boost_medias.end()) {
				picojson::object boost_media = itr->get<picojson::object>();
				unsigned __int64 m_id = stoi(boost_media["id"].to_str());
				string m_type = boost_media["type"].to_str();
				string m_url = boost_media["preview_url"].to_str();
				string m_size = boost_media["size"].to_str();
				DonMediaAttachmentModel* attach = new DonMediaAttachmentModel(m_id, m_type, m_url, m_size);
				reblog_media_attachments.push_back(attach);
				++itr;
			}

		}
	}
	~DonTootModel() {
		vector<DonMediaAttachmentModel*>::iterator itr = media_attachments.begin();
		while (itr != media_attachments.end()) {
			delete *itr;
			++itr;
		}
		vector<DonMediaAttachmentModel*>::iterator itr2 = reblog_media_attachments.begin();
		while (itr2 != reblog_media_attachments.end()) {
			delete *itr2;
			++itr2;
		}
	}
	unsigned __int64 getId() {
		return id;
	}
	string getCreatedAt() {
		return created_at;
	}
	string getVisibility() {
		return visibility;
	}
	DonAccountModel* getAccount() { //���X�g��̃A�J�E���g�ւ̃|�C���^
		return account;
	}
	string getContent() {
		return content;
	}
	vector<DonMediaAttachmentModel*> getMediaAttachments() {
		return media_attachments;
	}

	bool isBoost() {
		return is_boost;
	}
	string getReblogContent() {
		return reblog_content;
	}
	string getReblogAcct() {
		return reblog_acct;
	}
	string getReblogName() {
		return reblog_name;
	}
	string getReblogAvatar() {
		return reblog_avatar;
	}
	string getReblogCreatedAt() {
		return reblog_created_at;
	}
	vector<DonMediaAttachmentModel*> getReblogMediaAttachments() {
		return reblog_media_attachments;
	}


	void setId(unsigned __int64 value) {
		id = value;
	}

	void setCreatedAt(string value) {
		created_at = value;
	}
	void setVisibility(string value) {
		visibility = value;
	}
	void setAccount(DonAccountModel* value) {
		account = value;
	}
	void setContent(string value) {
		content = value;
	}
	void addMediaAttachments(unsigned __int64 pId,string pType,string pPreviewUrl,string pSize) {
		DonMediaAttachmentModel* attach = new DonMediaAttachmentModel(pId, pType, pPreviewUrl, pSize);
		media_attachments.push_back(attach);
	}
	

private:
	unsigned __int64 id;
	string created_at;//�ŏI�I�ɂ͓��t�^
	string visibility;
	DonAccountModel* account;
	string content;
	vector<DonMediaAttachmentModel*> media_attachments;
	bool is_boost = false;

	//�u�[�X�g���
	string reblog_avatar;
	string reblog_content;
	string reblog_acct;
	string reblog_name;
	string reblog_created_at;
	vector<DonMediaAttachmentModel*> reblog_media_attachments;
};



/***
DonTimeLineModel�N���X

�EDonTootModel�N���X�i�G���e�B�e�B�j�̃��X�g��ێ�����B
�E��L�̃������Ǘ����s���B
�E�d��������̂͑���delete�����B

*/
class DonTimeLineModel {
public:
	DonTimeLineModel() {
	//
	}
	~DonTimeLineModel() {
		vector<DonTootModel*>::iterator itr = list.begin();
		while (itr!=list.end()) {
			delete *itr;
			++itr;
		}
	}
	//�擾�����g�D�[�g�����łɓo�^����Ă��Ȃ��ꍇ�̂ݓo�^
	//�߂�l�͒ǉ������g�D�[�g�������͂��łɂ������g�D�[�g�̃C���X�^���X�i�̃|�C���^�j
	DonTootModel* addToot(DonTootModel* toot) {
		vector<DonTootModel*>::iterator itr;
		for (itr = list.begin(); itr != list.end(); ++itr) {
			if ((*itr)->getId() == toot->getId()) {
				delete toot;
				return *itr;
			}
		}
		list.push_back(toot);
		last_at = toot->getCreatedAt();
		return toot;
	}
	vector<DonTootModel*> getList() {
		return list;
	}
	string getLastAt() {
		return last_at;
	}
	void sort() {
		std::sort(list.begin(), list.end(), [](DonTootModel* left, DonTootModel* right) {
			return left->getCreatedAt() < right->getCreatedAt();
		});
	}
	void clear() {
		vector<DonTootModel*>::iterator itr = list.begin();
		while (itr != list.end()) {
			delete (*itr);
			++itr;
		}
		list.clear();
	}
private:
	vector<DonTootModel*> list;
	string last_at;
};

/***
DonAccountListModel�N���X

�EDonAccountModel�N���X�i�G���e�B�e�B�j�̃��X�g��ێ�����B
�E��L�̃������Ǘ����s���B
�E�d��������̂͑���delete�����B

*/
class DonAccountListModel {
public:
	DonAccountListModel() {
		//
	}
	~DonAccountListModel() {
		vector<DonAccountModel*>::iterator itr = list.begin();
		while (itr != list.end()) {
			delete *itr;
			++itr;
		}
	}
	//�擾�����A�J�E���g�����łɓo�^����Ă��Ȃ��ꍇ�̂ݓo�^
	//�߂�l�͒ǉ������A�J�E���g�������͂��łɂ������A�J�E���g�̃C���X�^���X�i�̃|�C���^�j
	DonAccountModel* addAccount(DonAccountModel* pAccount) {
		vector<DonAccountModel*>::iterator itr;
		for (itr = list.begin(); itr != list.end(); ++itr) {
			if ((*itr)->getId() == pAccount->getId()) {
				delete pAccount;
				return *itr;
			}
		}
		list.push_back(pAccount);
		return pAccount;
	}
	vector<DonAccountModel*> getList() {
		return list;
	}
private:
	vector<DonAccountModel*> list;

};
/**
 * @file WebAccess.h
 * @brief WebAccess�N���X�p�̃w�b�_�t�@�C��
 * @author h.wakimoto
 * @date 2017/05/01
 */


#pragma once

#include "ApiAccount.h"
#include "TootModel.h"

#include <string>

/*****

API�����֌W(�̃w�b�_)
curl�̏����͂����ɏW��B

�EWebAccess�N���X
�E���̑��֘A

*/

using namespace std;

class WebAccess {

public:

	WebAccess();
	~WebAccess();
	string getCodeURL(LoginInfo* logininfo);
	string getAccessToken(LoginInfo* logininfo);
	void getAccessInfoAdditional(AccessInfo* accessinfo);
	void getInstanceInfo(AccessInfo* accessinfo, DonInstanceModel* instanceinfo);

	void getTimeline(
		AccessInfo* accessinfo, DonTimeLineModel* timeline, DonAccountListModel* accounts,
		string path);
	void getCurrentHomeTimeline(AccessInfo* accessinfo, DonTimeLineModel* timeline, DonAccountListModel* accounts);
	void getCurrentPublicTimeline(AccessInfo* accessinfo, DonTimeLineModel* timeline, DonAccountListModel* accounts);
	void getCurrentLocalTimeline(AccessInfo* accessinfo, DonTimeLineModel* timeline, DonAccountListModel* accounts);
	void getCurrentSuperLocalTimeline(AccessInfo* accessinfo, DonTimeLineModel* timeline, DonAccountListModel* accounts);
	
private:

};

/**
 * @file MainWindow.cpp
 * @brief DonMainWindow�N���X�p�̃\�[�X�t�@�C��
 * @author h.wakimoto
 * @date 2017/05/01
 */

#include "PluginProcessor.h"
#include "MainWindow.h"
#include "WebAccess.h"
#include "TimeUtil.h"

#include "Utils.h"

/**
@brief API�A�N�Z�X�����݂̃^�C�����C���f�[�^���擾(�ʃX���b�h�p)
*/
void getData(DonTimeLineMode tl_mode, AccessInfo* accessinfo, DonTimeLineModel* timeline, DonAccountListModel* accounts) {

	WebAccess* wa = new WebAccess();

	//�z�[���^�C�����C����ǂݍ���
	if (tl_mode == tl_mode_home) {
		wa->getCurrentHomeTimeline(accessinfo, timeline, accounts);
		//���[�J���^�C�����C����ǂݍ���
	}
	else if (tl_mode == tl_mode_local) {
		wa->getCurrentLocalTimeline(accessinfo, timeline, accounts);
		//�X�[�p�[LTL
	}
	else if (tl_mode == tl_mode_super_local) {
		wa->getCurrentSuperLocalTimeline(accessinfo, timeline, accounts);
		//�A���^�C�����C����ǂݍ���
	}
	else if (tl_mode == tl_mode_public) {
		wa->getCurrentPublicTimeline(accessinfo, timeline, accounts);
	}
	timeline->sort();

	delete wa;
}


/**
@brief �R���X�g���N�^
*/
DonMainWindow::DonMainWindow(DonAudioProcessor& p)
	: AudioProcessorEditor(&p), processor(p)
{

#if JUCE_MAC || JUCE_WINDOWS
	getLookAndFeel().setDefaultSansSerifTypefaceName("Arial Unicode MS");
#endif

	//���{�ꃊ�e���������鎞�̃T���v��
	//String textToDisplay = CharPointer_UTF8("\xe4\xb8\x80\xe4\xba\x9b\xe6\x96\x87\xe5\xad\x97");

	/**
	@brief �^�C�}�[���X���[���邩�ǂ�����ݒ肷��B
	*/
	stop_timer = true;


	/**
	@brief ���[�U��/�C���X�^���X��
	@detail �ݒ�ς݂ł���΋N�����A���ݒ�ł���ΐݒ莞�ɓǂݍ��܂��
	*/

	addAndMakeVisible(userTitleLabel = new Label(String(), TRANS("-- Instance / User --")));
	userTitleLabel->setBounds(450, 42, 215, 24);
	userTitleLabel->setColour(Label::textColourId, Colours::grey);
	userTitleLabel->setJustificationType(Justification::centred);

	addAndMakeVisible(instanceLabel = new Label(String(), TRANS("------")));
	instanceLabel->setEditable(false, false, false);
	instanceLabel->setColour(Label::textColourId, Colours::silver);
	instanceLabel->setBounds(450, 66, 215, 24);
	instanceLabel->setJustificationType(Justification::centred);

	addAndMakeVisible(userLabel = new Label(String(), TRANS("------")));
	userLabel->setEditable(false, false, false);
	userLabel->setFont(Font("Meiryo UI", 16.00f, 0));
	userLabel->setColour(Label::textColourId, Colours::silver);
	userLabel->setBounds(450, 86, 215, 24);
	userLabel->setJustificationType(Justification::centred);

	/**
	@brief ���݂̃X�e�[�^�X��\������
	@detail ���̂Ƃ���Stop��Playing�Ȃǂ�\�������Ă��邪�قڃe�X�g�p�B
	*/
	//addAndMakeVisible(infoLabel = new Label(String(), TRANS("INFO")));
	//infoLabel->setJustificationType(Justification::right);
	//infoLabel->setEditable(false, false, false);
	//infoLabel->setColour(Label::textColourId, Colours::silver);


	/**
	@brief �ݒ�{�^��
	@detail ���̂Ƃ��댾��ݒ�̂݁B
	*/
	addAndMakeVisible(preferenceButton = new TextButton(String()));
	preferenceButton->setButtonText(TRANS("Preference"));
	preferenceButton->setColour(TextButton::buttonColourId, Colour::Colour(48, 48, 48));
	preferenceButton->setConnectedEdges(Button::ConnectedOnRight);
	preferenceButton->addListener(this);
	preferenceButton->setBounds(450, 8, 69, 24);


	/**
	@brief �A�J�E���g�ݒ�{�^��
	@detail �C���X�^���X��A�J�E���g��ς��鎞�Ɏg���B
	*/
	addAndMakeVisible(accountSettingButton = new TextButton(String()));
	accountSettingButton->setButtonText(TRANS("Account"));
	accountSettingButton->setColour(TextButton::buttonColourId, Colour::Colour(48, 48, 48));
	accountSettingButton->setConnectedEdges(Button::ConnectedOnRight | Button::ConnectedOnLeft);
	accountSettingButton->addListener(this);
	accountSettingButton->setBounds(524, 8, 68, 24);

	/**
	@brief �o�[�W�������ȂǁBGPL�̕\�L�Ȃǂ�����̂ŏd�v�B
	@todo �{���͕ʂ�Windows�ɂ��������b��B
	*/
	addAndMakeVisible(aboutButton = new TextButton(String()));
	aboutButton->setButtonText(TRANS("About"));
	aboutButton->setColour(TextButton::buttonColourId, Colour::Colour(48, 48, 48));
	aboutButton->setConnectedEdges(Button::ConnectedOnLeft);
	aboutButton->addListener(this);
	aboutButton->setBounds(597, 8, 68, 24);

	/**
	@brief �e�X�g�{�^���i�e�X�g�p�j
	@detail �{�Ԃł͎g��Ȃ��B�V�����@�\�������Ƃ��ɂƂ肠����
	���̃{�^���̃C�x���g�ɏ����ăe�X�g����B
	*/
	addAndMakeVisible(testButton = new TextButton(String()));
	testButton->setButtonText(TRANS("Dummy Toot"));
	testButton->addListener(this);


	/**
	@brief �e�L�X�g�G�f�B�^�i�e�X�g�p�j
	@detail �{�Ԃł͎g��Ȃ��B�V�����@�\�����ĂƂ肠�����f�o�b�O�\�����������ȂǂɁB
	*/
	addAndMakeVisible(textEditor = new TextEditor(String()));
	textEditor->setFont(Font("Meiryo UI", 20.00f, 0));
	textEditor->setColour(TextEditor::highlightedTextColourId, Colours::red);
	textEditor->setColour(TextEditor::highlightColourId, Colours::darkgrey);
	textEditor->setColour(TextEditor::textColourId, Colours::greenyellow);
	textEditor->setColour(TextEditor::backgroundColourId, Colours::black);
	textEditor->setMultiLine(true);


	/**
	@brief �^�C�����C���Z���N�^
	*/
	addAndMakeVisible(selectTLButton_Home        = new TextButton("Home"     ));
	addAndMakeVisible(selectTLButton_Local       = new TextButton("Local"    ));
	addAndMakeVisible(selectTLButton_Super_Local = new TextButton("Super LTL"));
	addAndMakeVisible(selectTLButton_Public      = new TextButton("Public"   ));
	selectTLButton_Home       ->setClickingTogglesState(true);
	selectTLButton_Local      ->setClickingTogglesState(true);
	selectTLButton_Super_Local->setClickingTogglesState(true);
	selectTLButton_Public     ->setClickingTogglesState(true);
	selectTLButton_Home       ->setRadioGroupId(1);
	selectTLButton_Local      ->setRadioGroupId(1);
	selectTLButton_Super_Local->setRadioGroupId(1);
	selectTLButton_Public     ->setRadioGroupId(1);
	selectTLButton_Home       ->setColour(TextButton::textColourOffId, Colours::black);
	selectTLButton_Local      ->setColour(TextButton::textColourOffId, Colours::black);
	selectTLButton_Super_Local->setColour(TextButton::textColourOffId, Colours::black);
	selectTLButton_Public     ->setColour(TextButton::textColourOffId, Colours::black);
	selectTLButton_Home       ->setColour(TextButton::textColourOnId, Colours::black);
	selectTLButton_Local      ->setColour(TextButton::textColourOnId, Colours::black);
	selectTLButton_Super_Local->setColour(TextButton::textColourOnId, Colours::black);
	selectTLButton_Public     ->setColour(TextButton::textColourOnId, Colours::black);
	selectTLButton_Home       ->setColour(TextButton::buttonColourId, Colours::white);
	selectTLButton_Local      ->setColour(TextButton::buttonColourId, Colours::white);
	selectTLButton_Super_Local->setColour(TextButton::buttonColourId, Colours::white);
	selectTLButton_Public     ->setColour(TextButton::buttonColourId, Colours::white);
	selectTLButton_Home       ->setColour(TextButton::buttonOnColourId, Colour::Colour(60,245,215));
	selectTLButton_Local      ->setColour(TextButton::buttonOnColourId, Colour::Colour(60,245,215));
	selectTLButton_Super_Local->setColour(TextButton::buttonOnColourId, Colour::Colour(60,245,215));
	selectTLButton_Public     ->setColour(TextButton::buttonOnColourId, Colour::Colour(60,245,215));
	int TL_SELECT_X = 10;
	selectTLButton_Home       ->setBounds(TL_SELECT_X, 8, 100, 24);
	selectTLButton_Local      ->setBounds(TL_SELECT_X+100, 8, 100, 24);
	selectTLButton_Super_Local->setBounds(TL_SELECT_X+200, 8, 100, 24);
	selectTLButton_Public     ->setBounds(TL_SELECT_X+300, 8, 100, 24);
	selectTLButton_Home       ->setConnectedEdges(Button::ConnectedOnRight);
	selectTLButton_Local	  ->setConnectedEdges(Button::ConnectedOnLeft | Button::ConnectedOnRight);
	selectTLButton_Super_Local->setConnectedEdges(Button::ConnectedOnLeft | Button::ConnectedOnRight);
	selectTLButton_Public     ->setConnectedEdges(Button::ConnectedOnLeft);
	selectTLButton_Super_Local->setToggleState(true, dontSendNotification);
	selectTLButton_Home->addListener(this);
	selectTLButton_Local->addListener(this);
	selectTLButton_Super_Local->addListener(this);
	selectTLButton_Public->addListener(this);


	/**
	@brief �A�N�e�B�u�{�^��
	*/

	addAndMakeVisible(activeTitleLabel = new Label(String(), TRANS("-- Active status --")));
	activeTitleLabel->setEditable(false, false, false);
	activeTitleLabel->setColour(Label::textColourId, Colours::grey);

	addAndMakeVisible(activeTLButton = new TextButton("Timeline"));
	addAndMakeVisible(activeLMButton = new TextButton("Loudness Meter"));
	addAndMakeVisible(activeDTButton = new TextButton("Cheerup message"));
	addAndMakeVisible(activeSoundButton = new TextButton("Mute"));

	int ACTIVE_BTN_POS_X = 450;
	int ACTIVE_BTN_POS_Y = 120;

	activeTitleLabel->setBounds(450, ACTIVE_BTN_POS_Y, 215, 24);
	activeTitleLabel->setJustificationType(Justification::centred);

	activeTLButton->setBounds(ACTIVE_BTN_POS_X,       ACTIVE_BTN_POS_Y + 30     , 105, 24);
	activeLMButton->setBounds(ACTIVE_BTN_POS_X + 110, ACTIVE_BTN_POS_Y + 30     , 105, 24);
	activeDTButton->setBounds(ACTIVE_BTN_POS_X,       ACTIVE_BTN_POS_Y + 24 + 35, 105, 24);
	activeSoundButton->setBounds(ACTIVE_BTN_POS_X + 110,     ACTIVE_BTN_POS_Y + 24 + 35, 105, 24);


	activeTLButton->setClickingTogglesState(true);
	activeLMButton->setClickingTogglesState(true);
	activeDTButton->setClickingTogglesState(true);
	activeSoundButton    ->setClickingTogglesState(true); 

	activeTLButton->setToggleState(true, dontSendNotification);
	activeLMButton->setToggleState(true, dontSendNotification);
	activeDTButton->setToggleState(true, dontSendNotification);
	activeSoundButton    ->setToggleState(true, dontSendNotification); //! @todo �X�^���h�A���[���������珉����ԂŃ~���[�g

	activeTLButton->setColour(TextButton::textColourOffId, Colours::grey);
	activeLMButton->setColour(TextButton::textColourOffId, Colours::grey);
	activeDTButton->setColour(TextButton::textColourOffId, Colours::grey);
	activeSoundButton    ->setColour(TextButton::textColourOffId, Colours::grey);

	activeTLButton->setColour(TextButton::textColourOnId, Colours::black);
	activeLMButton->setColour(TextButton::textColourOnId, Colours::black);
	activeDTButton->setColour(TextButton::textColourOnId, Colours::black);
	activeSoundButton    ->setColour(TextButton::textColourOnId, Colours::black);
	activeSoundButton->setColour(TextButton::textColourOffId, Colours::black);

	activeTLButton->setColour(TextButton::buttonOnColourId, Colour::Colour(102, 204, 255));
	activeLMButton->setColour(TextButton::buttonOnColourId, Colour::Colour(102, 204, 255));
	activeDTButton->setColour(TextButton::buttonOnColourId, Colour::Colour(102, 204, 255));
	activeSoundButton->setColour(TextButton::buttonOnColourId, Colour::Colour(102, 204, 255));
	activeSoundButton->setColour(TextButton::buttonColourId, Colour::Colour(250, 164, 128));

	activeTLButton->setConnectedEdges(Button::ConnectedOnRight);
	activeLMButton->setConnectedEdges(Button::ConnectedOnLeft);
	activeDTButton->setConnectedEdges(Button::ConnectedOnRight);
	activeSoundButton    ->setConnectedEdges(Button::ConnectedOnLeft);

	activeTLButton->addListener(this);
	activeLMButton->addListener(this);
	activeDTButton->addListener(this);
	activeSoundButton    ->addListener(this);


	/**
	@brief ���E�h�l�X���[�^�[
	*/
	addAndMakeVisible(lmTitleLabel = new Label(String(), TRANS("-- Loudness meter (LUFS) --")));
	lmTitleLabel->setEditable(false, false, false);
	lmTitleLabel->setColour(Label::textColourId, Colours::grey);

	addAndMakeVisible(momentaryLoudnessLabel = new Label(String(), TRANS("Momentary")));
	addAndMakeVisible(shortTermLoudnessLabel  = new Label(String(), TRANS("Short Term")));
	addAndMakeVisible(integratedLoudnessLabel = new Label(String(), TRANS("Integrated")));

	addAndMakeVisible(momentaryLoudnessValue = new Label(String(), TRANS("---")));
	momentaryLoudnessValue->setJustificationType(Justification::right);
	momentaryLoudnessValue->setFont(Font("Arial", 24.00f, Font::bold));

	addAndMakeVisible(shortTermLoudnessValue = new Label(String(), TRANS("---")));
	shortTermLoudnessValue->setJustificationType(Justification::right);
	shortTermLoudnessValue->setFont(Font("Arial", 24.00f, Font::bold));

	addAndMakeVisible(integratedLoudnessValue = new Label(String(), TRANS("---")));
	integratedLoudnessValue->setJustificationType(Justification::right);
	integratedLoudnessValue->setFont(Font("Arial", 24.00f, Font::bold));

	int LM_START_POS_X = 450;
	int LM_START_POS_Y = 220;

	lmTitleLabel->setBounds(450, LM_START_POS_Y, 215, 24);
	lmTitleLabel->setJustificationType(Justification::centred);

	momentaryLoudnessLabel->setBounds(450, LM_START_POS_Y + 30, 150, 26);
	shortTermLoudnessLabel->setBounds(450, LM_START_POS_Y + 60, 150, 26);
	integratedLoudnessLabel->setBounds(450, LM_START_POS_Y + 90, 150, 26);
	momentaryLoudnessValue->setBounds(400, LM_START_POS_Y + 30, 270, 26);
	shortTermLoudnessValue->setBounds(400, LM_START_POS_Y + 60, 270, 26);
	integratedLoudnessValue->setBounds(400, LM_START_POS_Y + 90, 270, 26);

	/**
	@brief �u���E�U�R���|�[�l���g�B
	@detail ���̂Ƃ���WebBrowserComponent��P�Ɍp�����Ă��邾���Ȃ̂�WebBrowserComponent�ł��\��Ȃ��B
	*/
	webBrowser = new DonWebBrowser();
	addAndMakeVisible(webBrowser);
	webBrowser->setVisible(false);



	//�v���O�C�����C���E�B���h�E�̃T�C�Y
	setSize(700, 600);

	//�C���X�g�[��������Ɋ������Ă��邩�ǂ���
	if (setupCheck()) {

		//�V���O���g������
		this->accessinfo = new AccessInfo();
		this->instanceinfo = new DonInstanceModel();
		this->accounts = new DonAccountListModel();
		this->timeline  = new DonTimeLineModel();
		this->applicaitonProperties = new ApplicationProperties();

		/**
		@brief �e��ݒ�
		*/
		prop_options = new PropertiesFile::Options();
		prop_options->applicationName = "Vocalodon VST";
		prop_options->commonToAllUsers = true;
		prop_options->filenameSuffix = "xml";
		prop_options->folderName = "VocalodonVST";
		prop_options->storageFormat = PropertiesFile::storeAsXML;
		prop_options->ignoreCaseOfKeyNames = true;
		applicaitonProperties->setStorageParameters(*prop_options);

		lastLoadedAt = "";

		//�ݒ�m�F
		if (loadAccessInfo()) {
			showUserInformations();
		}
		else {
			//�����ݒ�
			if (initialSetting()) {
				if (loadAccessInfo()) {
					showUserInformations();
				}
				else {
					showMessageDialog("Wrong Setting. Please try again. ");
				}
			}
		}

		loadSettings();

		//	if (this->accessinfo->getAccessToken() != "") {
		stop_timer = false;
		//		activeTLButton->setToggleState(true, dontSendNotification);
		startTimer(TIMER_INTERVAL);
		//	}

			//�_�~�[Toot�p�^�C�}�[
		dummyTootTimer = new DummyTootTimer(this->timeline, this->accounts, this->accessinfo->getAccount(), this, dummyTootLang);

		//DAW��~��
		setIsPlaying(false);
	}
	else {
		showMessageDialog("Install is not complete. ");
		this->setEnabled(false);
	}

	repaint();
}

/**
@brief �C���X�g�[�������튮�����Ă��邩�m�F�B
@todo �������B
@detail �t�@�C���Ȃǂ����ׂđ����Ă��邩�m�F�B����t�@�C��������Ȃ���DAW��������̂Ŏ��O�`�F�b�N�B
*/
bool DonMainWindow::setupCheck() {
	return true;
}


/**
@brief ���[�U���A�C���X�^���X����\������
@detail �R���X�g���N�^�A�ݒ�{�^���N���b�N����Ăяo�����
*/
void DonMainWindow::showUserInformations() {
	
	//���[�U���
	string username = this->accessinfo->getAccount()->getDisplayName(); //�\�����̂�
	//string username = this->accessinfo->getAccount()->getDisplayName() + " (@" + this->accessinfo->getAccount()->getAcct() + ")";//�t��
	userLabel->setText(username, NotificationType::dontSendNotification);
	
	//�C���X�^���X���
	string instancename = this->instanceinfo->getTitle(); //�\�����̂�
	//string instancename = this->instanceinfo->getTitle() + " (" + this->instanceinfo->getUri() + ")"; //�t��
	instanceLabel->setText(instancename, NotificationType::dontSendNotification);
}



/**
@brief �f�X�g���N�^
*/
DonMainWindow::~DonMainWindow()
{
	if (this->isEnabled()) {
		saveSettings();
		try {

			if (threadactive) {
				makeListThread.join();
				threadactive = false;
			}

			stop_timer = true;//����͂��̈ʒu�Ōザ��Ȃ��ƃG���[
			delete this->timeline;
			delete this->accounts;
			delete this->instanceinfo;
			delete this->accessinfo;
		}
		catch (exception e) {
			//
		}
	}
}


/**
@brief �^�C�}�[�R�[���o�b�N
@detail
�^�C�}�[�Ń^�C�����C�����ĕ\���B
�֋X�㓯���^�C�}�[�Ńg���K���Ă��邪�AgetCurrentTimeline()�ɂ��^�C�����C���f�[�^�擾�͕ʃX���b�h�ōs���Ă���
reloadTimeline���Ă΂ꂽ���_�ł͂܂��O�s�̏����͍s���Ă��Ȃ��B
�ŏI�I�ɂ͌ʂ̃^�C�}�[���Ăяo�����ق����ǂ��Ǝv���邪���̕ӂ͂܂��������B

�^�C�}�[��0.2�b���Ƃɔ����B
�E���E�h�l�X���[�^�[�͖���ĕ`��
�E�^�C�����C�������[�h��1�b����
�EAPI�f�[�^�擾��3�b����
*/
void DonMainWindow::timerCallback() {

	File htmlfile(TEMP_DIR + "index.html");
	
	if (htmlfile.exists() && !webBrowser->isVisible()) {
		webBrowser->setVisible(true);
	}

	if (!stop_timer) {

		//TL�擾
		if (activeTLButton->getToggleState() && counter_get_toots <= 0) {
			getCurrentTimeline();
			reloadTimeline();
			counter_get_toots = INTERVAL_GET_TOOTS;
		}

		if (counter_tl_reload <= 0) {
			counter_tl_reload = INTERVAL_TL_RELOAD;
		}

		//���E�h�l�X
		if (activeLMButton->getToggleState()) {
			updateLoudness();
		}
		else {
			resetLoudness();
		}

		counter_tl_reload -= TIMER_INTERVAL;
		counter_get_toots -= TIMER_INTERVAL;
	}
}


/**
@brief �ĕ`��
*/
void DonMainWindow::paint(Graphics& g)
{
	g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));
	g.setColour(Colours::white);
	g.setFont(15.0f);
	g.drawImageAt(bgImage, 0, 0);
}

/**
@brief ���T�C�Y
@detail �E�C���h�E�T�C�Y����t�Z����ꍇ�ɂ͂����ŃZ�b�g�B
*/
void DonMainWindow::resized()
{
	//infoLabel->setBounds(400, 58, 295, 26);
	//textEditor->setBounds(450, 10, 300, 300);//�f�o�b�O�p

	//testButton->setBounds(getWidth() - 100, 108, 90, 26);//�f�o�b�O�p

	webBrowser->setBounds(10, 40, 400, 550);
	webBrowser->goToURL(LOCAL_URL + "index.html");

}


/**
@brief �ݒ�t�@�C����������擾�B
@detail �����API�Œǉ����i���O�C�����[�U�̏��,�C���X�^���X���j���擾�B
*/
bool DonMainWindow::loadAccessInfo() {

	PropertiesFile* prop = applicaitonProperties->getUserSettings();
	string host_url     = prop->getValue("host_url", "").toStdString();
	string access_token = prop->getValue("access_token", "").toStdString();
	applicaitonProperties->closeFiles();
	if (access_token == "") {
		return false;
	}
	else {
		
		this->accessinfo->setInfo(host_url,access_token);
		WebAccess* wa = new WebAccess();

		//���̎��_�ł�host_url��access_token������񂪂Ȃ��̂ŁA�T�[�o���烆�[�U�����擾����B
		wa->getAccessInfoAdditional(this->accessinfo);

		//�C���X�^���X���
		wa->getInstanceInfo(this->accessinfo, this->instanceinfo);

		delete wa;

		string bg_filename;
		if (host_url == "https://vocalodon.net") {
			bg_filename = "bg.png";
		}
		else {
			bg_filename = "bg.png";
		}
		File bgImageFile(IMAGE_DIR + bg_filename);
		bgImage = ImageFileFormat::loadFrom(bgImageFile);

		return true;
	}
}

/**
@brief �ݒ���t�@�C���ɕۑ��B
*/
void DonMainWindow::saveAccessInfo() {

	PropertiesFile* prop = applicaitonProperties->getUserSettings();
	prop->setValue("host_url", this->accessinfo->getHostUrl().c_str());
	prop->setValue("access_token", this->accessinfo->getAccessToken().c_str());

	applicaitonProperties->setStorageParameters(*prop_options);
	applicaitonProperties->saveIfNeeded();
	applicaitonProperties->closeFiles();

}

/**
@brief �����ݒ胋�[�`��
@detail �N�����ɃC���X�^���X��A�J�E���g�̏�񂪂Ȃ��ꍇ��A�A�J�E���g�ݒ��ύX�������ꍇ�ɌĂяo�����B
*/
bool DonMainWindow::initialSetting() {
	stop_timer = true;
	LoginInfo* logininfo = new LoginInfo();
	bool changed = false;

	//�C���X�^���X�I����ʂ��Ăяo��
	if (showLoginDialog(logininfo))
	{
		WebAccess* wa = new WebAccess();
		string url = wa->getCodeURL(logininfo);
		textEditor->setText(url);
		delete wa;
		ChildProcess process;
		process.start("start " + url);
		//�ݒ��ʂ��Ăяo��
		if (showCodeEntryDialog(logininfo, url))
		{
			wa = new WebAccess();
			string access_token = wa->getAccessToken(logininfo);
			if (access_token=="null") { //null�Ƃ��������񂪓����Ă���
				//�ݒ�Ɏ��s
				showMessageDialog("Setting FAILED. Try again.");
			}
			else {
				//�ݒ�ɐ���
				this->accessinfo->setInfo(logininfo->getHostUrl(), access_token);
				textEditor->setText(access_token);
				saveAccessInfo();
				changed = true;
				showMessageDialog("Success!");
				reloadTimeline();
			}
			delete wa;
		}
	}
	delete logininfo;
	stop_timer = false;
	return changed;
}


/**
@brief �C�x���g�n���h��
*/
void DonMainWindow::buttonClicked(Button* button) {
	//�A�J�E���g�ݒ�
	if (button == accountSettingButton) {
		if (initialSetting()) {
			if (loadAccessInfo()) {
				showUserInformations();
			}
			else {
				showMessageDialog("Wrong Setting. Please try again. ");
			}
		}
	}
	//�ݒ�
	else if (button == preferenceButton) {
		if (showPreferenceDialog()) {
			loadSettings();
		}
	}

	//�z�[���{�^��
	else if (button == selectTLButton_Home) {
		clearTimeline();
		this->tl_mode = tl_mode_home;
		reloadTimeline();
	}
	//���[�J���{�^��
	else if (button == selectTLButton_Local) {
		clearTimeline();
		this->tl_mode = tl_mode_local;
		reloadTimeline();
	}
	//�X�[�p�[LTL�{�^��
	else if (button == selectTLButton_Super_Local) {
		clearTimeline();
		this->tl_mode = tl_mode_super_local;
		reloadTimeline();
	}
	//�A���{�^��
	else if (button == selectTLButton_Public) {
		clearTimeline();
		this->tl_mode = tl_mode_public;
		reloadTimeline();
	}
	//TL�A�N�e�B�u�{�^��
	else if (button == activeTLButton){
		if (activeTLButton->getToggleState()) {
			getCurrentTimeline();
		}
	}
	//�������b�Z�[�W�A�N�e�B�u�{�^��
	else if (button == activeDTButton) {
		if (activeDTButton->getToggleState()) {
			dummyTootTimer->start(500);
		}
		else {
			dummyTootTimer->stop();
		}
	}
	//���E�h�l�X�A�N�e�B�u�{�^��
	else if (button == activeLMButton) {
		//onTimer�̕���Ńt���O�Ƃ��ė��p�B�C�x���g�͔������Ȃ�
	}
	//AudioOutput�{�^��
	else if (button == activeSoundButton) {
		this->processor.setIsMute(!activeSoundButton->getToggleState());
	}
	//About�{�^��
	else if (button == aboutButton) {
		showAboutDialog();
	}
	//�e�X�g�{�^��
	else if (button == testButton) {
		DummyTootFactory* dummy = new DummyTootFactory(this->timeline,this->accounts,this->accessinfo->getAccount(),dummyTootLang);
		dummy->makeDummyToot();
	}
	
}


/**
@brief �C���X�^���X���̓_�C�A���O��\��
@return OK�{�^���������ꂽ��
*/
bool DonMainWindow::showLoginDialog(LoginInfo* logininfo) {
	LookAndFeel_V4 myLookandFeel;
	myLookandFeel.setColour(TextEditor::textColourId, Colours::yellow);
	myLookandFeel.setColour(AlertWindow::backgroundColourId, Colour::Colour(20, 20, 20));
	myLookandFeel.setColour(TextButton::buttonColourId, Colour::Colour(0, 128, 128));

	AlertWindow w("Accounts", "Enter your instance's URL.                                               ", AlertWindow::NoIcon);
	w.setLookAndFeel(&myLookandFeel);

	w.setSize(580, 300);
	w.addTextEditor("hosturl", "https://vocalodon.net", "Instance URL: (don't include last slash)");
	w.addTextBlock("");

	w.addButton("OK", 1, KeyPress(KeyPress::returnKey, 0, 0));
	w.addButton("Cancel", 0, KeyPress(KeyPress::escapeKey, 0, 0));

	if (w.runModalLoop() != 0) //OK
	{
		logininfo->setInfo(
			w.getTextEditorContents("hosturl").toStdString()
		);
		return true;
	}
	return false;
}

/**
@brief �R�[�h���̓_�C�A���O��\��
@return OK�{�^���������ꂽ��
*/
bool DonMainWindow::showCodeEntryDialog(LoginInfo* logininfo, string url) {
	LookAndFeel_V4 myLookandFeel;
	myLookandFeel.setColour(TextEditor::highlightColourId, Colours::yellow);
	myLookandFeel.setColour(TextEditor::highlightedTextColourId, Colours::black);
	myLookandFeel.setColour(AlertWindow::backgroundColourId, Colour::Colour(20, 20, 20));
	myLookandFeel.setColour(TextButton::buttonColourId, Colour::Colour(0, 128, 128));

	AlertWindow w("InputCode", "Copy and Paste this URL.                                                          ", AlertWindow::NoIcon);
	w.setLookAndFeel(&myLookandFeel);

	w.setColour(TextEditor::textColourId, Colours::yellow);
	w.addTextBlock("1. After open your browser and login, Please copy this URL all (triple click and right click and select `copy`) and paste to your browser's URL bar.");
	w.addTextBlock(url);
	w.addTextBlock("");
	w.addTextBlock("2. On your web browser, Click Green button and then you get Auth-Code. Please copy it and paste here.");
	w.addTextEditor("authcode", "", "Code: (paste from web browser)");
	w.addTextBlock("");
	w.addButton("OK", 1, KeyPress(KeyPress::returnKey, 0, 0));
	w.addButton("Cancel", 0, KeyPress(KeyPress::escapeKey, 0, 0));

	if (w.runModalLoop() != 0) //OK
	{
		logininfo->setAuthCode(
			w.getTextEditorContents("authcode").toStdString()
		);

		return true;
	}
	return false;
}


/**
@brief �o�[�W�����_�C�A���O��\��
@todo ��Window�ł����ƍ�肽��
*/
bool DonMainWindow::showAboutDialog() {
	LookAndFeel_V4 myLookandFeel;
	myLookandFeel.setColour(AlertWindow::backgroundColourId, Colour::Colour(20, 20, 20));
	myLookandFeel.setColour(TextButton::buttonColourId, Colour::Colour(0, 128, 128));
	AlertWindow w("Vocalodon VST", "                                                                                                    ", AlertWindow::NoIcon);
	w.setLookAndFeel(&myLookandFeel);

	w.addTextBlock("ver 0.9.0");
	w.addTextBlock("Copyright (c) TOMOKI++ / vocalodon.net ");
	w.addTextBlock("");
	w.addTextBlock("License : GPLv3");
	w.addTextBlock("Using\n\n");
	w.addTextBlock("JUCE : https://www.juce.com/\nlibcurl : https://curl.haxx.se/libcurl/\nThe LUFS Meter : https://www.klangfreund.com/lufsmeter/\npicojson : https://github.com/kazuho/picojson");
	w.addTextBlock("Official site : https://vst.vocalodon.net/");
	w.addTextBlock("Technology supported by SGN.Inc https://sgnx.co.jp/");
	w.addTextBlock("VST is a plugin technology by Steinberg Media Technologies GmbH.\nVOCALOID and VOCALO are trademarks of Yamaha Corporation.");
	w.addButton("OK", 1, KeyPress(KeyPress::returnKey, 0, 0));
	if (w.runModalLoop() != 0) //OK
	{
		return true;
	}
	return true;
}


/**
@brief �ݒ�_�C�A���O
@detail ���̂Ƃ���͉������b�Z�[�W�̌���I���̂�
@return OK�{�^���������ꂽ��
*/
bool DonMainWindow::showPreferenceDialog() {
	LookAndFeel_V4 myLookandFeel;
	myLookandFeel.setColour(TextEditor::highlightColourId, Colours::yellow);
	myLookandFeel.setColour(TextEditor::highlightedTextColourId, Colours::black);
	myLookandFeel.setColour(AlertWindow::backgroundColourId, Colour::Colour(20, 20, 20));
	myLookandFeel.setColour(TextButton::buttonColourId, Colour::Colour(0, 128, 128));

	AlertWindow w("Preferences", "                                                          ", AlertWindow::NoIcon);
	w.setLookAndFeel(&myLookandFeel);

	StringArray* langArray = new StringArray();
	langArray->add("Japanese (default)");
	langArray->add("English");
	
	w.addComboBox("language", *langArray);
	w.addTextBlock("");
	w.addButton("OK", 1, KeyPress(KeyPress::returnKey, 0, 0));
	w.addButton("Cancel", 0, KeyPress(KeyPress::escapeKey, 0, 0));

	if (dummyTootLang == "en") {
		w.getComboBoxComponent("language")->setSelectedItemIndex(1);
	}
	else {
		w.getComboBoxComponent("language")->setSelectedItemIndex(0);
	}

	if (w.runModalLoop() != 0) //OK
	{
		switch (w.getComboBoxComponent("language")->getSelectedItemIndex()) {
			case 0: 
				dummyTootLang = "ja";
				break;
			case 1: 
				dummyTootLang = "en";
				break;
			default:
				dummyTootLang = "ja";

		}
		saveSettings();
		return true;
	}
	return false;
}


/**
@brief �e��ݒ�ۑ�
@detail �A�J�E���g�ݒ�ȊO�͂��ׂĂ����ŕۑ�
*/
void DonMainWindow::saveSettings() {

	PropertiesFile* prop = applicaitonProperties->getUserSettings();
	
	//�������b�Z�[�W�̌���
	prop->setValue("cheerup_lang", this->dummyTootLang.c_str());
	dummyTootTimer->setLanguage(this->dummyTootLang.c_str());

	//TL�{�^���̏��
	string active_timeline;

	if (selectTLButton_Home->getToggleState()) {
		active_timeline = "home";
	}
	else if (selectTLButton_Local->getToggleState()) {
		active_timeline = "local";
	}
	else if (selectTLButton_Super_Local->getToggleState()) {
		active_timeline = "super_ltl";
	}
	else if (selectTLButton_Public->getToggleState()) {
		active_timeline = "public";
	}
	prop->setValue("active_timeline", active_timeline.c_str());


	//�A�N�e�B�u�{�^���̏��
	if (activeTLButton->getToggleState()) {
		prop->setValue("active_button_timeline", "1");
	}
	else {
		prop->setValue("active_button_timeline", "0");
	}
	if (activeLMButton->getToggleState()) {
		prop->setValue("active_button_loudnessmeter", "1");
	}
	else {
		prop->setValue("active_button_loudnessmeter", "0");
	}
	if (activeDTButton->getToggleState()) {
		prop->setValue("active_button_cheerupmessage", "1");
	}
	else {
		prop->setValue("active_button_cheerupmessage", "0");
	}
	if (activeSoundButton->getToggleState()) {
		prop->setValue("active_button_sound", "1");
	}
	else {
		prop->setValue("active_button_sound", "0");
	}

	applicaitonProperties->setStorageParameters(*prop_options);
	applicaitonProperties->saveIfNeeded();
	applicaitonProperties->closeFiles();

}

/**
@brief �ݒ�ǂݍ���
@detail �A�J�E���g�ݒ�ȊO�͂��ׂĂ����œǂݍ���
*/
bool DonMainWindow::loadSettings() {

	PropertiesFile* prop = applicaitonProperties->getUserSettings();

	//�������b�Z�[�W�̌���
	this->dummyTootLang = prop->getValue("cheerup_lang","ja").toStdString();

	//Timeline
	string active_timeline = prop->getValue("active_timeline", "super_ltl").toStdString();
	if (active_timeline=="home") {
		selectTLButton_Home->setToggleState(true, dontSendNotification);
	}
	else if (active_timeline == "local") {
		selectTLButton_Local->setToggleState(true, dontSendNotification);
	}
	else if (active_timeline == "super_ltl") {
		selectTLButton_Super_Local->setToggleState(true, dontSendNotification);
	}
	else if (active_timeline == "public") {
		selectTLButton_Public->setToggleState(true, dontSendNotification);
	}

	//�A�N�e�B�u�{�^��
	activeTLButton->setToggleState(prop->getValue("active_button_timeline", "1").toStdString()       == "1", dontSendNotification);
	activeLMButton->setToggleState(prop->getValue("active_button_loudnessmeter", "1").toStdString()  == "1", dontSendNotification);
	activeDTButton->setToggleState(prop->getValue("active_button_cheerupmessage", "1").toStdString() == "1", dontSendNotification);
	activeSoundButton->setToggleState(prop->getValue("active_button_sound", "1").toStdString()          == "1", dontSendNotification);

	applicaitonProperties->closeFiles();

	return true;
}


/**
@brief DAW�̍Đ�/��~�̃X�e�[�^�X���ς�����Ƃ��̊e�폈��
@todo DAW�̍Đ�/��~���m���Ɏ擾������@���s���Ȃ��߁A���̂Ƃ���
AudioPlayHead::CurrentPositionInfok����擾���Ă��邪�APremiere�Ȃǂł�
��~�̔��肪����Ȃ��͗l�B
*/
void DonMainWindow::setIsPlaying(bool value) {
	string infotxt;
	if (this->isPlaying != value) {
		this->isPlaying = value;
		if (value && activeDTButton->getToggleState()) {
			infotxt = "playing";
			dummyTootTimer->start(500);
			this->processor.ebu128LoudnessMeter.reset();
			momentaryLoudnessValue->setText("---",NotificationType::dontSendNotification);
			shortTermLoudnessValue->setText("---", NotificationType::dontSendNotification);
			integratedLoudnessValue->setText("---", NotificationType::dontSendNotification);

		} else {
			infotxt = "stopped";
			momentaryLoudnessValue->setColour(Label::textColourId, Colours::white);
			shortTermLoudnessValue->setColour(Label::textColourId, Colours::white);
			integratedLoudnessValue->setColour(Label::textColourId, Colours::white);
			dummyTootTimer->stop();
		}
	}
	//this->infoLabel->setText(infotxt,NotificationType::dontSendNotification);//�f�o�b�O�p
}

/**
@brief �Đ�/��~��Ԃ�Ԃ��B
@return �Đ�/��~���
*/
bool DonMainWindow::getIsPlaying() {
	return this->isPlaying;
}

/**
@brief �^�C�����C����������
@todo ���ꂪ�Ă΂��^�C�~���O�Ȃǂ͍��̂Ƃ���悭�l���ĂȂ��B
*/
void DonMainWindow::clearTimeline() {
	timeline->clear();
	renderHTML();
}


/**
@brief HTML�𐶐�
@detail �^�C�����C����HTML�����ĕۑ��B
@todo ���̕ӂ͕ʃN���X�ɂ������B
*/
void DonMainWindow::renderHTML() {

	vector<DonTootModel*> list = timeline->getList();
	vector<DonTootModel*>::reverse_iterator itr;
	File headerfile(TEMPLATE_DIR + "header.html");
	string header = headerfile.loadFileAsString().toStdString();

	File footerfile(TEMPLATE_DIR + "footer.html");
	string footer = footerfile.loadFileAsString().toStdString();

	string body;
	for (itr = list.rbegin(); itr != list.rend(); ++itr) {
		DonTootModel* toot = *itr;
		DonAccountModel* account = toot->getAccount();

		//�A�J�E���g���
		body += "<div class=\"toot_outer\">\n";

		//�ʏ�toot
		if (!toot->isBoost()) {
			body += "<div class=\"avatar\">\n";
			body += "  <img src=\"" + account->getAvatar() + "\">\n";
			body += "</div>\n";

			body += "<div class=\"toot_inner\">\n";
			body += "  <div class=\"head\">\n";
			body += "    <div class=\"account_detail\">\n";
			body += "      <p><span class=\"account\">" + account->getDisplayName() + "</span><span class=\"acct\">(@" + account->getAcct() + ")</span></p>\n";
			body += "      <p><span class=\"timestamp\">" + toot->getCreatedAt() + "</span></p>\n";
			body += "    </div>\n";
			body += "  </div>\n";

			//�R���e���c
			body += "  <div class=\"content\">\n";
			body += toot->getContent() + "\n";
			body += "  </div>\n";

			//�Y�t
			body += "<div class=\"media_attachments\">\n";
			vector<DonMediaAttachmentModel*> attachments = toot->getMediaAttachments();
			vector<DonMediaAttachmentModel*>::iterator itera = attachments.begin();
			while (itera != attachments.end()) {
				DonMediaAttachmentModel* attachment = *itera;
				if (attachment->getType() == "image") {
					body += "<img src=\"" + attachment->getPreviewURL() + "\">\n";
				}
				++itera;
			}
			body += "</div>\n";//media_attachment

			body += "</div>\n";//toot_inner
		}

		//�u�[�X�g
		else {
			body += "<div class=\"avatar\">\n";
			body += "  <img src=\"" + account->getAvatar() + "\" class=\"boost\"><span class=\"boost\">Bst!</span><br>\n";
			body += "  <img src=\"" + toot->getReblogAvatar() + "\">\n";
			body += "</div>\n";

			body += "<div class=\"toot_inner\">\n";
			body += "  <div class=\"head\">\n";
			body += "    <div class=\"account_detail\">\n";
			body += "      <p><span class=\"account\">" + toot->getReblogName() + "</span><span class=\"acct\">(@" + toot->getReblogAcct() + ")</span></p>\n";
			body += "      <p><span class=\"timestamp\">" + toot->getReblogCreatedAt() + "</span></p>\n";
			body += "    </div>\n";
			body += "  </div>\n";

			//�R���e���c
			body += "  <div class=\"content\">\n";
			body += toot->getReblogContent() + "\n";
			body += "  </div>\n";

			//�Y�t
			body += "<div class=\"media_attachments\">\n";
			vector<DonMediaAttachmentModel*> attachments = toot->getReblogMediaAttachments();
			vector<DonMediaAttachmentModel*>::iterator itera = attachments.begin();
			while (itera != attachments.end()) {
				DonMediaAttachmentModel* attachment = *itera;
				if (attachment->getType() == "image") {
					body += "<img src=\"" + attachment->getPreviewURL() + "\">\n";
				}
				++itera;
			}
			body += "</div>\n";//media_attachment

			body += "</div>\n";//toot_inner
		}

		body += "</div>\n";//toot_outer
	}

	File htmlfile(TEMP_DIR + "index.html");
	htmlfile.create();
	htmlfile.replaceWithText(header, false, false);
	htmlfile.appendText(body, false, false);
	htmlfile.appendText(footer, false, false);

}

/**
@brief ���݂̃^�C�����C������API�Ŏ擾�i�ʃX���b�h�j
*/
void DonMainWindow::getCurrentTimeline() {

	if (threadactive) {
		makeListThread.join();
	}

	//�f�[�^�擾�i�ʃX���b�h�j
	DonTimeLineMode      _tl_mode = this->tl_mode;
	AccessInfo*          _accessinfo = this->accessinfo;
	DonTimeLineModel*    _timeline = this->timeline;
	DonAccountListModel* _accounts = this->accounts;

	makeListThread = thread([_tl_mode, _accessinfo, _timeline, _accounts] { getData(_tl_mode, _accessinfo, _timeline, _accounts); });

	threadactive = true;

}


/**
@brief ���݂̃^�C�����C������`��
@detail �����ł̓f�[�^�擾�͍s�킸�A�L���b�V����̃^�C�����C����\������B
*/
void DonMainWindow::reloadTimeline() {
	//�^�C�����C���̍ŏI�X�V�����ς���Ă���ꍇ�̂ݍĕ`��
	if (lastLoadedAt != timeline->getLastAt()) {
		renderHTML();
		this->webBrowser->goToURL(LOCAL_URL + "index.html");
		lastLoadedAt = timeline->getLastAt();
	}
}


/**
@brief ���E�h�l�X���̐F�ݒ�
*/
void DonMainWindow::setLabelColor(Label* lbl, float val) {
	if (val > -8) {
		lbl->setColour(Label::textColourId, Colours::red);
	}
	else if (val > -13) {
		lbl->setColour(Label::textColourId, Colours::yellow);
	}
	else {
		lbl->setColour(Label::textColourId, Colours::white);
	}
}


/**
@breif ���E�h�l�X���̍X�V
*/
void DonMainWindow::updateLoudness() {

	string momentary;
	string shotTerm;
	string integrated;

	if (this->processor.getMomentaryLoudness() < -250) {
		momentary = "---";
	}
	else {
		momentary = Float2String(processor.getMomentaryLoudness(), 2) + " LUFS";
		setLabelColor(momentaryLoudnessValue, processor.getMomentaryLoudness());
	}

	if (this->processor.getShortTermLoudness() < -250) {
		shotTerm = "---";
	}
	else {
		shotTerm = Float2String(processor.getShortTermLoudness(),2) + " LUFS";
		setLabelColor(shortTermLoudnessValue, processor.getShortTermLoudness());
	}

	if (this->processor.getIntegratedLoudness() < -250) {
		integrated = "---";
	}
	else {
		integrated = Float2String(processor.getIntegratedLoudness(),2) + " LUFS";
		setLabelColor(integratedLoudnessValue, processor.getIntegratedLoudness());
	}

	momentaryLoudnessValue->setText(momentary.c_str(), NotificationType::dontSendNotification);
	shortTermLoudnessValue->setText(shotTerm.c_str(), NotificationType::dontSendNotification);
	integratedLoudnessValue->setText(integrated.c_str(), NotificationType::dontSendNotification);

}


/**
@breif ���E�h�l�X���̍X�V
*/
void DonMainWindow::resetLoudness() {
	momentaryLoudnessValue->setText("---", NotificationType::dontSendNotification);
	shortTermLoudnessValue->setText("---", NotificationType::dontSendNotification);
}
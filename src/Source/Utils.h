/**
* @file Utils.h
* @brief �ėp�֐��p�w�b�_�t�@�C��
* @author h.wakimoto
* @date 2017/05/01
*/

#pragma once

#include <string>
#include <sstream>

#include "../JuceLibraryCode/JuceHeader.h"

using namespace std;

//JUCE�ˑ��͔����ȋC������B
string time8601toJP(string time8601);

string getCurrentTime8601();

string getCurrentTimeJP();

vector<string> split(const string &s, char delim);

bool showMessageDialog(string msg);

string Replace(string srcStr, string findStr, string replaceStr);

string Float2String(float f, int digits);

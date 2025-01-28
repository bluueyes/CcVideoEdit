#include "CcAudio.h"
#include <iostream>
#include <mutex>
using namespace std;

struct CcTime{

	CcTime(int tms) {
		h = (tms / 1000) / 3600;
		m = ((tms / 1000) % 3600) / 60;
		s = (tms / 1000) % (60);
		ms = tms % 1000;
	}
	std::string ToString() {
		char buf[16] = { 0 };
		sprintf_s(buf, "%d:%d:%d.%d", h, m, s, ms);
		return buf;
	}
	int h = 0;
	int m = 0;
	int s = 0;
	int ms = 0;

};

class CcxAudio :public CcAudio {
public:
	bool ExPortA(std::string src, std::string out,int beginMs,int endMs) {
		std::lock_guard<std::mutex> lock(_mutex);
		string cmd = "ffmpeg ";
		cmd += "-i " + src;//
		if (beginMs > 0) {
			cmd += " -ss " + CcTime(beginMs).ToString();
		}
		if (endMs > 0) {
			cmd += " -t " + CcTime(endMs).ToString();
		}
		cmd+= " -vn -y " + out;
		cout << cmd << endl;
		system(cmd.c_str());
		return true;
	}

	bool Merge(std::string v, std::string a, std::string out) {
		std::lock_guard<std::mutex> lock(_mutex);
		//ffmpeg -i test.avi -i test.mp3 -c copy output.mp4
		string cmd = "ffmpeg -i ";
		cmd +=  v + " -i " + a + " -c copy " + out;
		cout << cmd << endl;
		system(cmd.c_str());
		return true;

	}

	~CcxAudio(){}
private:
	std::mutex _mutex;
};


CcAudio* CcAudio::GetInstance()
{
	static CcxAudio cc;
	return &cc;
}


CcAudio::CcAudio()
{
}

CcAudio::~CcAudio()
{
}

#pragma once
#include <string>
class CcAudio
{
public:
	static CcAudio* GetInstance();
	//������Ƶ�ļ�
	virtual bool ExPortA(std::string src, std::string out,int beginMs=0,int endMs=0)=0;

	//�ϲ�����Ƶ v:��Ƶ�ļ� a:��Ƶ�ļ� out:����ļ�
	virtual bool Merge(std::string v, std::string a, std::string out)=0;

protected:
	CcAudio();
	virtual ~CcAudio();

};


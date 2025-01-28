#pragma once
#include <string>
class CcAudio
{
public:
	static CcAudio* GetInstance();
	//导出音频文件
	virtual bool ExPortA(std::string src, std::string out,int beginMs=0,int endMs=0)=0;

	//合并音视频 v:视频文件 a:音频文件 out:输出文件
	virtual bool Merge(std::string v, std::string a, std::string out)=0;

protected:
	CcAudio();
	virtual ~CcAudio();

};


#pragma once
#include <opencv2/core.hpp>
#include "CcImagePro.h"
#include <vector>

enum TaskType {
	TASK_NONE,
	TASK_GAIN,
	TASK_ROTATE90,
	TASK_ROTATE180,
	TASK_ROTATE270,
	TASK_FLIPX,
	TASK_FLIPY,
	TASK_FLIPXY,
	TASK_RESIZE,
	TASK_PYDOWN,
	TASK_PYUP,
	TASK_CLIP,
	TASK_GRAY,
	TASK_MARK,
	TASK_BLEND,

};
struct Task {
	TaskType type;
	std::vector<double> para;
};
class CcFilter
{
public:
	virtual cv::Mat Filter(const cv::Mat& src1,const cv::Mat& src2)=0;
	virtual void Add(Task task)=0;
	virtual void Clear() = 0;
	static CcFilter* Get();
	virtual ~CcFilter();
protected:
	CcFilter();
};


#include "CcFilter.h"
#include <mutex>
#include "CcImagePro.h"

class CcxFilter :public CcFilter {

public:
    CcxFilter() {}
    cv::Mat Filter(const cv::Mat& src1, const cv::Mat& src2) {
       //std::lock_guard <std::mutex> lock(_mutex);
        
        CcImagePro pro;
        pro.Set(src1, src2);
        for (int i = 0; i < tasks.size(); i++) {
            switch (tasks[i].type) {
            case TASK_GAIN:
                pro.Gain(tasks[i].para[0], tasks[i].para[1]);
                break;
            case TASK_ROTATE90:
                pro.Rotate90();
                break;
            case TASK_ROTATE180:
                pro.Rotate180();
                break;
            case TASK_ROTATE270:
                pro.Rotate270();
                break;
            case TASK_FLIPX:
                pro.FlipX();
                break;
            case TASK_FLIPY:
                pro.FlipY();
                break;
            case TASK_FLIPXY:
                pro.FlipXY();
                break;
            case TASK_RESIZE:
                pro.Resize(tasks[i].para[0], tasks[i].para[1]);
                break;
            case TASK_PYDOWN:
                pro.PyDown(tasks[i].para[0]);
                break;
            case TASK_PYUP:
                pro.PyUp(tasks[i].para[0]);
                break;
            case TASK_CLIP:
                pro.Clip(tasks[i].para[0], tasks[i].para[1], tasks[i].para[2], tasks[i].para[3]);
                break;
            case TASK_GRAY:
                pro.Gray();
                break;
            case TASK_MARK:
                pro.Mark(tasks[i].para[0], tasks[i].para[1], tasks[i].para[2]);
                break;
            case TASK_BLEND:
                pro.Blend(tasks[i].para[0]);
                break;
            case TASK_MERGE:
                pro.Merge();
            default:
                break;
            }
        }
        cv::Mat ret = pro.Get();
        return ret;
    }

    void Add(Task task) {
        std::lock_guard <std::mutex> lock(_mutex);
        tasks.push_back(task);

    }
    void Clear() {
        std::lock_guard <std::mutex> lock(_mutex);
        tasks.clear();
    }

    ~CcxFilter() {
        Clear();
     }
  
private:
    std::vector<Task> tasks;
    std::mutex _mutex;
};


CcFilter* CcFilter::Get()
{
    static CcxFilter filter;
    return &filter;
}

CcFilter::~CcFilter()
{

}

CcFilter::CcFilter() {

}
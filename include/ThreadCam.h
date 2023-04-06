#ifndef THREADCAM_H
#define THREADCAM_H

#include <thread>
#include <atomic>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
//----------------------------------------------------------------------------------
class ThreadCam
{
public:
    std::atomic<bool> GrabOpen;
    std::atomic<bool> GrabStop;
    std::atomic<size_t> GrabTag;

    ThreadCam();
    virtual ~ThreadCam();

    void Init(std::string dev);
    void GetFrame(cv::Mat& m);      //returns empty when no frame is available.
    void Quit(void);
protected:
    std::thread ThGr;
    void GrabThread(void);
private:
    size_t Tag,PrevTag;
    std::string device;
    cv::Mat mm;
    cv::VideoCapture cap;
    std::mutex mtx;
};
//----------------------------------------------------------------------------------
#endif // THREADCAM_H

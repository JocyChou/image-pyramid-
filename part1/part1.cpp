#include <iostream>
#include <opencv2/opencv.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <opencv/cv.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
//#include <tr1/unordered_map>


using namespace cv;
using namespace std;

Mat imresize(Mat source, int iw, int ih, int tw, int th){
    Mat result = cvCreateMat(th, tw, CV_8UC1);
    float stepw = (float)(iw - 1)/(tw - 1);
    float steph = (float)(ih - 1)/(th - 1);
    result.at<uchar>(0, 0) = source.at<uchar>(0, 0);
    result.at<uchar>(0, tw - 1) = source.at<uchar>(0, iw - 1);
    result.at<uchar>(th - 1, 0) = source.at<uchar>(ih - 1, 0);
    result.at<uchar>(th - 1, tw - 1) = source.at<uchar>(ih - 1, iw - 1);
    for(int i = 0; i < th; i++){
        for(int j = 0; j < tw; j++){
            if(((i == 0) && (j == 0)) || ((i == 0) && (j == tw - 1)) || ((i == th - 1) && (j == 0)) || ((i == th - 1) && (j == tw - 1))){
                continue;
            }
            result.at<uchar>(i, j) = 0;
            float f = 0;
            float curx = j * stepw;
            float disx = curx - floor(curx);
            float cury = i * steph;
            float disy = cury - floor(cury);
            f += (float)((1 - disx) * (1 - disy) * (source.at<uchar>((int)floor(cury), (int)floor(curx))) +
            (disx) * (1 - disy) * (source.at<uchar>((int)floor(cury), (int)ceil(curx))) +
            (1 - disx) * (disy) * (source.at<uchar>((int)ceil(cury), (int)floor(curx))) +
            (disx) * (disy) * (source.at<uchar>((int)ceil(cury), (int)ceil(curx))));
            result.at<uchar>(i, j) = (uchar)f;
        }
    }
    imshow("Original:", source);
    imshow("Resized", result);
    cvWaitKey(1);
    string savename = "/Users/Jocy/Dropbox/2016 Fall/CSI 111/Program assignment/ProgramAssignment2.1/part1/text0.75.jpg";
    imwrite(savename, result);
    return result;
}

int main(int argc, char* argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    cv::Mat img = cv::imread(argv[1], 1);
    std::string savename = "/Users/Jocy/Dropbox/2016 Fall/CSI 111/Program assignment/ProgramAssignment2/flowergray/part1/flowergray";
    if (img.empty()) {
        std::cout << "Error : Image loading failed!" << std::endl;
        return -1;
    }
    cv::Mat source = cvCreateMat(img.rows, img.cols, CV_8UC1);
    if (img.channels() != 1){
        std::cout << "Warning : Input image is not a grayscale image" << std::endl;
        cv::cvtColor(img, source, CV_RGBA2GRAY);
    }else{
        source = img;
    }
    cv::Mat filter = Mat(2, 2, CV_32F, Scalar(1.0/4));
    //cv::Mat filtertri = Mat(3, 3, CV_32F, Scalar(1.0/9));
    
    imresize(source, source.cols, source.rows, (source.cols) * 0.75, (source.rows) * 0.75);
    
    //cout << filtertri << endl;
    //pyramida(source,savename, filter,source.cols, source.rows, filter.cols, filter.rows);
    //pyramidb(source,savename, filter,source.cols, source.rows, filter.cols, filter.rows);
    //convolve(source, filter,source.cols, source.rows, filter.cols, filter.rows);
    //convolve(source, filtertri, source.cols, source.rows, filtertri.cols, filtertri.rows);
    //Lappyramid(source, savename, filter, source.cols, source.rows, filter.cols, filter.rows);
    //Highpassa(source, savename, filter, source.cols, source.rows, filter.cols, filter.rows);
    //Highpassb(source, savename, filter, source.cols, source.rows, filter.cols, filter.rows);
    return -1;
}

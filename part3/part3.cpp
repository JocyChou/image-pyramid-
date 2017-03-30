
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
    //imshow("Original:", source);
    //imshow("Resized", result);
    //cvWaitKey(1);
    //string savename = "/Users/Jocy/Dropbox/2016 Fall/CSI 111/Program assignment/ProgramAssignment2.1/part1/text0.75.jpg";
    //imwrite(savename, result);
    return result;
}

Mat convolve(Mat source, Mat filter, int iw, int ih, int fw, int fh){
    Mat conv = cvCreateMat(iw, ih, CV_8UC1);
    
    int samplei = 0;
    int samplej = 0;
    for (int i = 0; i < conv.rows; i++){
        for (int j = 0; j < conv.cols; j++){
            conv.at<uchar>(i, j) = 0;
            float f = 0;
            for (int m = 0; m < fh; m++){
                for (int n = 0; n < fw; n++){
                    samplei = i - fh/2 + m;
                    samplej = j - fw/2 + n;
                    if (samplei >= 0 && samplei < ih && samplej >= 0 && samplej < iw){
                        f += (float)(source.at<uchar>(samplei, samplej)) * filter.at<float>(m,n);
                    }
                }
            }
            conv.at<uchar>(i, j) += (uchar)f;
        }
    }
    //cvNamedWindow("Original:", CV_WINDOW_AUTOSIZE);
    //imshow("Original:", source);
    //cvNamedWindow("Convoluted:", CV_WINDOW_AUTOSIZE);
    //imshow("Convoluted", conv);
    //imwrite("/Users/Jocy/Dropbox/2016 Fall/CSI 111/Program assignment/ProgramAssignment1/ProgramAssignment1/convoluted1Polarcities.jpg", conv);
    //cvWaitKey(1);
    //cvDestroyAllWindows();
    return conv;
}

Mat subsample(Mat source, int iw, int ih){
    Mat result = cvCreateMat(iw/2, ih/2, CV_8UC1);
    for(int i = 0; i < result.rows; i++){
        for(int j = 0; j < result.cols; j++){
            result.at<uchar>(i, j) = source.at<uchar>((i + 1) * 2 - 1, (j + 1) * 2 - 1);
        }
    }
    return result;
}





Vector<Mat> pyramida(Mat source, string savename, cv::Mat filter, int iw, int ih, int fw, int fh){
    int i = 0;
    Vector<Mat> pyra(9);
    pyra[0] = source;
    
    savename = "/Users/Jocy/Dropbox/2016 Fall/CSI 111/Program assignment/ProgramAssignment2.1/part2/text";
    string presavename = savename + "pyramidA0.jpg";
    //imwrite(presavename, source);
    for(i = 0; i < 8; i++){
        Mat preimage = convolve(source , filter, source.cols, source.rows, fw, fh);
        for(int m = 0; m < preimage.rows; m++){
            preimage.at<uchar>(m, 0) = preimage.at<uchar>(m, 0)/0.7;
            preimage.at<uchar>(m, 1) = preimage.at<uchar>(m, 1)/0.95;
            preimage.at<uchar>(m, preimage.cols - 1) = preimage.at<uchar>(m, preimage.cols - 1)/0.7;
            preimage.at<uchar>(m, preimage.cols - 2) = preimage.at<uchar>(m, preimage.cols - 2)/0.95;
        }
        preimage = convolve(preimage , filter.t(), preimage.cols, preimage.rows, fh, fw);
        for(int m = 0; m < preimage.cols; m++){
            preimage.at<uchar>(0, m) = preimage.at<uchar>(0, m)/0.7;
            preimage.at<uchar>(1, m) = preimage.at<uchar>(1, m)/0.95;
            preimage.at<uchar>(preimage.rows - 1, m) = preimage.at<uchar>(preimage.rows - 1, m)/0.7;
            preimage.at<uchar>(preimage.rows - 2, m) = preimage.at<uchar>(preimage.rows - 2, m)/0.95;
        }
        preimage = subsample(preimage, preimage.rows, preimage.cols);
        
        string presavename = savename + "pyramidA" + to_string(i+1) + ".jpg";
        //imwrite(presavename, preimage);
        source = preimage;
        pyra[i + 1] = source;
        //cout<<preimage.rows<<endl;
        //imshow("gaussian", source);
         //cvWaitKey(0);
    }
    return pyra;
    
}

void Lappyramid(Mat source, string savename, cv::Mat filter, int iw, int ih, int fw, int fh){
    int i = 0;
    double min = 0;
    double max = 0;
    double* minp = &min;
    double* maxp = &max;
    minMaxIdx(source,minp,maxp);
    Vector<Mat> lappyramid(7);
    Vector<Mat> gaupyramid = pyramida(source, savename, filter,source.cols, source.rows, filter.cols, filter.rows);
    
    savename = "/Users/Jocy/Dropbox/2016 Fall/CSI 111/Program assignment/ProgramAssignment2.1/part3/flowergray";
    for(i = 1; i < 8; i++){
        Mat doublesize = imresize(gaupyramid[i], gaupyramid[i].cols, gaupyramid[i].rows, gaupyramid[i].cols * 2, gaupyramid[i].rows * 2);
        //cout<<doublesize.cols<<" "<<gaupyramid[i - 1].cols<<endl;
        Mat prelevel = doublesize - gaupyramid[i - 1];
        double pyramin = 0;
        double pyramax = 0;
        double* pyraminp = &pyramin;
        double* pyramaxp = &pyramax;
        minMaxIdx(prelevel, pyraminp, pyramaxp);
        prelevel = (prelevel + (min - pyramin))/((pyramax - pyramin)/(max - min));
        lappyramid[i - 1] = prelevel;
        string presavename = savename + "lappyramidB" + to_string(i) + ".jpg";
        imwrite(presavename, prelevel);
        cvNamedWindow("Laplacian:", CV_WINDOW_AUTOSIZE);
        imshow("Laplacian", prelevel);
        cvWaitKey(1);
    }
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
    float gaussian[5] = {0.05, 0.25, 0.4, 0.25, 0.05};
    cv::Mat filter = Mat(1,5, CV_32F, gaussian);
    //cv::Mat filtertri = Mat(3, 3, CV_32F, Scalar(1.0/9));
    
    //imresize(source, source.cols, source.rows, (source.cols) * 0.75, (source.rows) * 0.75);
    
    //cout << filtertri << endl;
    //pyramida(source,savename, filter,source.cols, source.rows, filter.cols, filter.rows);
    //pyramidb(source,savename, filter,source.cols, source.rows, filter.cols, filter.rows);
    //convolve(source, filter,source.cols, source.rows, filter.cols, filter.rows);
    //convolve(source, filtertri, source.cols, source.rows, filtertri.cols, filtertri.rows);
    
    Lappyramid(source, savename, filter, source.cols, source.rows, filter.cols, filter.rows);
    //Highpassa(source, savename, filter, source.cols, source.rows, filter.cols, filter.rows);
    //Highpassb(source, savename, filter, source.cols, source.rows, filter.cols, filter.rows);
    return -1;
}

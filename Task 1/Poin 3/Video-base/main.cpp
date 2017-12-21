/*
    Rya Meyvriska
    G64164008
    Task 1 Poin 2
*/

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iomanip>
#include <iostream>
#include <sstream>

using namespace cv;
using namespace std;
#include "../../../contrast.h"
#include "header/blob.cpp"
#include "header/countour.cpp"
#include "header/template_matching.cpp"

class Circle{
    private:
        Mat dst,src;
        int sum;
    public:
        void processHoughCircle(Mat picture){
            src = picture.clone();
            dst = src.clone();
            sum = 0;

            /// Ubah to Gray
            if(src.channels()==3){
                cvtColor(src,src,CV_BGR2GRAY);
            }

            /// img enhanchment untuk mereduce noise
            src = contrast(src);
            GaussianBlur(src,dst, Size(3, 3), 2, 2 );
            medianBlur(dst,dst,3);

            vector<Vec3f> circles;

            /// LIBRARY HOUGH UNTUK MENDETEKSI LINGKARAN
            HoughCircles( dst, circles, CV_HOUGH_GRADIENT, 1, 30, 200, 50, 0, 0 );

            /// ALGORITMA MENGGAMBAR LINGKARAN
            for( size_t i = 0; i < circles.size(); i++ )
            {
                Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
                int radius = cvRound(circles[i][2]);
                circle( src, center, 3, Scalar(0,255,0), -1, 8, 0 );// circle center
                circle( src, center, radius, Scalar(0,0,255), 3, 8, 0 );// circle outline
            }

            sum = circles.size();
        }
        Mat getSrc(){
            return src;
        }
        int getSum(){
            return sum;
        }
};

Mat addText(Mat src, String str, int x, int y){
    putText(src,str,Point(x,y),FONT_HERSHEY_COMPLEX_SMALL,1,Scalar(255,255,255),1);
    return src;
}

void printDescription(int type){
    Mat des = Mat::zeros(200,380,type);
    des = addText(des,"Tekan:",10,25);
    des = addText(des,"'b' untuk  teknik blob",10,50);
    des = addText(des,"'c' untuk teknik countour",10,75);
    des = addText(des,"'t' untuk template maching",10,100);
    des = addText(des,"'r' untuk original",10,125);
    des = addText(des,"'q' untuk keluar",10,150);

    imshow("Description",des);
    moveWindow("Description",650,0);

}

int main(){
    VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;

    Circle c;
    bool blob = false;
    bool countour = false;
    bool templateMatching = false;

    while(true)
    {
        Mat frame,result;
        cap >> frame; // get a new frame from camera
        flip(frame, frame,1); //agar tidak mirror
        c.processHoughCircle(frame);
        result = c.getSrc().clone();

        if(blob){
            result = blobTekhnik(result);
        }

        if(countour){
            result = countourTekhnik(result);
        }

        if(templateMatching){
            Mat temp = imread("template.jpg");
            cvtColor(temp,temp,CV_BGR2GRAY);
            result = templateMatchingTekhnik(result,temp);
        }

        if(result.channels()==1)
            cvtColor(result,result,CV_GRAY2BGR);

        ostringstream sum;
        sum << c.getSum();
        putText(result,"Jumlah lingkarang: " + sum.str(),Point(10,25),FONT_HERSHEY_SIMPLEX,1,Scalar(0,255,0),4);
        printDescription(result.type());
        imshow("Hough Circle", result);
        moveWindow("Hough Circle", 0,0);

        int key = waitKey(100);

        if(key=='b'){
            blob = true;
            countour = false;
            templateMatching = false;
        }else if(key=='c'){
            blob = false;
            countour = true;
            templateMatching = false;
        }else if(key=='t'){
            blob = false;
            countour = false;
            templateMatching = true;
        }else if(key=='r'){
            blob = false;
            countour = false;
            templateMatching = false;
        }else if(key=='q'){
            cout<<"terimakasih"<<endl;
            break;
        }
    }
    return 0;
}

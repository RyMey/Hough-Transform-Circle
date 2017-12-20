/*
    Rya Meyvriska
    G64164008
    Task 1 - Poin 1
*/

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>

using namespace cv;
using namespace std;
#include "contrast.h"

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
            Mat temp = src.clone();
            temp = temp - 64;
            GaussianBlur(temp,dst, Size(3, 3), 2, 2 );
            //medianBlur(dst,dst,3);
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

int main(){
    Mat src = imread("picture/coin3.jpg", 0);
    Mat result;

    if(!src.data){
        cout<<"file gambar tidak tersedia"<<endl;
        return -1;
    }

    Circle c;
    c.processHoughCircle(src);
    result = c.getSrc().clone();

    cvtColor(result,result,CV_GRAY2BGR);
    ostringstream sum;
    sum << c.getSum();
    putText(result,sum.str(),Point(10,25),FONT_HERSHEY_SIMPLEX,1,Scalar(255,0,0),4);

    imshow("Hough Circle", result);
    moveWindow("Hough Circle",512,0);
    cout<< "jumlah lingkarang : "<<c.getSum()<<endl;

    waitKey();
    return 0;
}

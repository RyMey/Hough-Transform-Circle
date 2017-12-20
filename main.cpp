/*
    Rya Meyvriska
    G64164008
*/

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <iomanip>

using namespace cv;
using namespace std;

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
            GaussianBlur(src,dst, Size(3, 3), 2, 2 );
            medianBlur(dst,dst,3);
            imshow("Image", dst);

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
    Mat src = imread("picture/coin1.jpg", 0);
    Mat result;

    if(!src.data){
        cout<<"file gambar tidak tersedia"<<endl;
        return -1;
    }

    Circle c;
    c.processHoughCircle(src);
    result = c.getSrc().clone();
    imshow("Hough Circle", result);
    cout<< c.getSum();

    waitKey();
    return 0;
}

/*
    Rya Meyvriska
    G64164008
    Task 2
*/

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iomanip>
#include <iostream>
#include <sstream>

using namespace cv;
using namespace std;

class Circle{
    private:
        Mat dst,src;
        int sum;
    public:
        void processHoughCircle(Mat picture){
            src = picture.clone();
            dst = Mat::zeros((src.rows/2)+1, src.cols, src.type());
            sum = 0;

            /// Ubah to Gray
            if(src.channels()==3){
                cvtColor(src,src,CV_BGR2GRAY);
            }

            cvtColor(dst,dst,CV_BGR2GRAY);
            for(int i=0;i<dst.rows;i++){
                for(int j=0;j<dst.cols;j++){
                    dst.at<uchar>(i,j) =  src.at<uchar>((src.rows/2-1)+i,j);
                }
            }

            /// img enhanchment untuk mereduce noise
            GaussianBlur(dst,dst, Size(3, 3), 2, 2 );
            medianBlur(dst,dst,3);

            vector<Vec3f> circles;

            /// LIBRARY HOUGH UNTUK MENDETEKSI LINGKARAN
            HoughCircles( dst, circles, CV_HOUGH_GRADIENT, 1, 30, 200, 50, 0, 0 );

            /// ALGORITMA MENGGAMBAR LINGKARAN
            for( size_t i = 0; i < circles.size(); i++ )
            {
                Point center(cvRound(circles[i][0]), cvRound(circles[i][1])+(src.rows/2));
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

Mat ProcessHoughLines(Mat src){
    Mat dst, cdst;

    Canny(src, dst, 50, 200, 3); ///EDGE DETECTION
    cvtColor(dst, cdst, CV_GRAY2BGR);
    //cdst = dst.clone();

    vector<Vec2f> lines, lines2;///VARIABLE UNTUK MENAMPUNG GAMBAR GARIS

    /// LIBRARY HOUGH UNTUK MENDETEKSI GARIS
    HoughLines(dst, lines, 1, CV_PI/180, 150, 0, 0 );

    /// ALGORITMA MENGGAMBAR GARIS
    for( size_t i = 0; i < lines.size(); i++ )
    {
        float rho = lines[i][0], theta = lines[i][1];
        Point pt1, pt2;
        double a = cos(theta), b = sin(theta);
        double x0 = a*rho, y0 = b*rho;
            pt1.x = cvRound(x0 + 1000*(-b));
            pt1.y = cvRound(y0 + 1000*(a));
            pt2.x = cvRound(x0 - 1000*(-b));
            pt2.y = cvRound(y0 - 1000*(a));
        line( cdst, pt1, pt2, Scalar(0,0,255), 3, CV_AA);
    }
    return cdst;
}

int main(){
    VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;

    Circle c;

    while(true)
    {
        Mat frame,result;
        cap >> frame; // get a new frame from camera
        flip(frame, frame,1); //agar tidak mirror
        c.processHoughCircle(frame);
        result = c.getSrc().clone();
        cvtColor(result,result,CV_GRAY2BGR);
        //result = ProcessHoughLines(frame); //un comment and comment this to make circle or line

        ostringstream sum;
        sum << c.getSum();
        putText(result,"Jumlah objek: " + sum.str(),Point(10,25),FONT_HERSHEY_SIMPLEX,1,Scalar(0,255,0),4);
        line(result,Point(0,result.rows/2),Point(result.cols,result.rows/2),Scalar(255,0,0),4,3);
        imshow("Hough Circle", result);

        if(waitKey(1) >= 0) break;
    }
    return 0;
}

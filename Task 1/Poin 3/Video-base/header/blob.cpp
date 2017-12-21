#include "opencv2/opencv.hpp"

Mat blobTekhnik(Mat src){
	Mat im = src.clone();
	im = contrast(im);
    GaussianBlur(im,im, Size(3, 3), 2, 2 );
    medianBlur(im,im,3);

	// Setup SimpleBlobDetector parameters.
	SimpleBlobDetector::Params params;

	// Change thresholds
	params.minThreshold = 10;
	params.maxThreshold = 200;

	// Filter by Area.
	params.filterByArea = true;
	params.minArea = 1500;

	// Filter by Circularity
	params.filterByCircularity = true;
	params.minCircularity = 0.1;

	// Filter by Convexity
	params.filterByConvexity = true;
	params.minConvexity = 0.87;

	// Filter by Inertia
	params.filterByInertia = true;
	params.minInertiaRatio = 0.01;


	// Storage for blobs
	vector<KeyPoint> keypoints;

	// Set up detector with params
	SimpleBlobDetector detector(params);

	// Detect blobs
	detector.detect( im, keypoints);
	Mat im_with_keypoints;
	drawKeypoints( src, keypoints, im_with_keypoints, Scalar(0,0,255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS );

	// return blobs
	return im_with_keypoints;
}

//original code was here https://www.learnopencv.com/blob-detection-using-opencv-python-c/

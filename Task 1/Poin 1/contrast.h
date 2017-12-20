/*
    Rya Meyvriska
    G64164008
    Ilmu Komputer IPB
*/

Mat contrast(Mat source){
    Mat result;
    result = source.clone();
    int width = source.size().width;
    int height = source.size().height;
    int minContrast = 255;
    int maxContrast = 0;

    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            int value = (int) source.at<uchar>(i,j);
            if(value<minContrast)
                minContrast = value;
            else if(value>maxContrast)
                maxContrast = value;
        }
    }

    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            int value = (int) source.at<uchar>(i,j);
            result.at<uchar>(i,j)= (value-minContrast)*(255/(maxContrast-minContrast));
        }
    }

    return result;
}

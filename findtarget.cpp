#include <opencv2/opencv.hpp>

int main(){
    cv::Mat img, img_processed, img_canny;

	//run with camera input
	cv::VideoCapture input(1);
	//run with test footage
    //cv::VideoCapture input("targetfootage.mp4");
    while(true){
        if(!input.read(img))
            break;

        cv::threshold(img, img_processed, 240, 255, CV_THRESH_BINARY);

        cv::Canny(img_processed, img_canny, 50, 200, 3);

        std::vector<std::vector<cv::Point> > contours;
        cv::findContours(img_canny, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

        for(size_t i = 0; i < contours.size(); i++){
            std::vector<cv::Point> shape;
            cv::approxPolyDP(contours[i], shape, cv::arcLength(cv::Mat(contours[i]), true) * 0.02, true);
            
            if(shape.size() == 4){
                //check if it has an area more than 100
                if (cv::contourArea(shape) > 100)
                    cv::polylines(img, shape, true, cv::Scalar(0, 255, 0), 2, 8);
                else{
					//cv::polylines(img, shape, true, cv::Scalar(0, 0, 255), 2, 8);
				}
                
            }
        }

        cv::imshow("img", img);
        char c = cv::waitKey(1);

        if(c == 27)
            break;
    }
}
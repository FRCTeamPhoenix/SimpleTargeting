#include <opencv2/opencv.hpp>

cv::Point getCenter(const std::vector<cv::Point>& shape){
    cv::Point center(0);
    for(auto point : shape){
        center.x += point.x;
        center.y += point.y;
    }

    center.x /= (int)shape.size();
    center.y /= (int)shape.size();

    return center;
}

cv::Point getCenter(const cv::Point& point1, const cv::Point& point2){
    return cv::Point((point1.x + point2.x) / 2, (point1.y + point2.y) / 2);
}

int getDistance(const cv::Point& point1, const cv::Point& point2){
    int dX = abs(point1.x - point2.x);
    int dY = abs(point1.y - point2.y);

    return sqrt(pow(dX, 2) + pow(dY, 2));
}

int distance = 0;

int main(){
    cv::Mat img, img_processed, img_canny;  

    //run with camera input
    //cv::VideoCapture input(1);
    //run with test footage
    cv::VideoCapture input("targetfootage.mp4");
    while(true){
        if(!input.read(img))
            break;

        bool targetSeen = false;

        cv::threshold(img, img_processed, 230, 255, CV_THRESH_BINARY);

        cv::Canny(img_processed, img_canny, 50, 150, 3);

        std::vector<std::vector<cv::Point> > contours;
        cv::findContours(img_canny, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

        std::vector<std::vector<cv::Point> > quadrilaterals;
        for(size_t i = 0; i < contours.size(); i++){
            std::vector<cv::Point> shape;
            cv::approxPolyDP(contours[i], shape, cv::arcLength(cv::Mat(contours[i]), true) * 0.02, true);
            
            if(shape.size() == 4){
                //check if it has an area more than 100
                if (cv::contourArea(shape) > 200){
                    quadrilaterals.push_back(shape);
                    cv::polylines(img, shape, true, cv::Scalar(255, 0, 0), 2, 8);
                }
                
            }
        }

        for(auto shape1 : quadrilaterals){
            for(auto shape2: quadrilaterals){
                //check if shapes are at similare y values
                if(shape1 != shape2 && abs(getCenter(shape1).y - getCenter(shape2).y) <= 100){
                    targetSeen = true;
                    cv::polylines(img, shape1, true, cv::Scalar(0, 255, 0), 4, 8);
                    cv::polylines(img, shape2, true, cv::Scalar(0, 255, 0), 4, 8);
                    cv::line(img, getCenter(shape1), getCenter(shape2), cv::Scalar(0, 0, 255), 2, 8);
                    
                    cv::Point center = getCenter(getCenter(shape1), getCenter(shape2));
                    cv::circle(img, center, 4, cv::Scalar(0, 0, 255), 4, 8);

                    distance = getDistance(getCenter(shape1), getCenter(shape2));
                }
            }
        }

        //output distance over center line
        cv::putText(img, "Distance: "+std::to_string(distance)+" Visible: " + (targetSeen ? "True" : "False"), 
            cv::Point(img.cols / 2, 20),
            CV_FONT_HERSHEY_DUPLEX,
            0.5,
            cv::Scalar(0, 0, 255),
            2, 8);

        cv::imshow("img", img);
        char c = cv::waitKey(1);

        if(c == 27)
            break;
    }
}
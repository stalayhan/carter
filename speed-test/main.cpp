/*
	Speed Estimation
*/

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/background_segm.hpp>
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <iostream>

using namespace cv;
using namespace std;

double y_ortalama = 0.0;
double speed = 0.0;
int avarageSpeed = 0.0;

void drawFlow(const Mat& flow, Mat& cflowmap, int step, const Scalar& color) {
	y_ortalama = 0.0f;
	int  y_num = 0;
	float y_sum = 0.0f;

	for (int y = 0; y < cflowmap.rows; y += step)
		for (int x = 0; x < cflowmap.cols; x += step)
		{
			const Point2f& fxy = flow.at< Point2f>(y, x);
			if (fxy.y > 0.8){
				line(cflowmap, Point(x, y), Point(cvRound(x + fxy.x), cvRound(y + fxy.y)),color);

				++y_num;
				y_sum += fxy.y;

				circle(cflowmap, Point(cvRound(x + fxy.x), cvRound(y + fxy.y)), 1, color, -1);
			}
		}
	if (y_num != 0){
		y_ortalama = y_sum / y_num;
	}
}

int main(int argc, char *argv[])
{
	float  y_max = 0.0f;
	int k = 0;

	Mat img;
	Mat prevFrame, frame; //current frame
	Ptr<BackgroundSubtractor> bg ;
	bg = createBackgroundSubtractorMOG2();

	VideoCapture cap(argv[1]);
	if (!cap.isOpened())
	{
		cout<<"Video açılmadı!!!"<<endl;
		return -1;
	}

	cap >> img;

	resize(img, prevFrame, Size(img.size().width / 4, img.size().height / 4));

	cvtColor(prevFrame, prevFrame, CV_BGR2GRAY);

	while (true) {
		cap >> img;
		//Resize
		resize(img, frame, Size(img.size().width / 4, img.size().height / 4));
		cvtColor(frame, frame, CV_BGR2GRAY);
		Mat flow, fore;
		bg->apply(img,fore);
		erode(fore,fore,Mat());

		calcOpticalFlowFarneback(prevFrame, frame, flow, 0.5, 3, 15, 3, 5, 1.2, 1);

		Mat cflow;
		cvtColor(prevFrame, cflow, CV_GRAY2BGR);
		drawFlow(flow, cflow, 10, CV_RGB(0, 255, 0));
		imshow("Speed Estimation", cflow);

		if (y_max < y_ortalama)
			y_max = y_ortalama;

		int kmSpeed = (y_ortalama / y_max) * 90;

		if (kmSpeed != 0){
			cout <<  " Speed of Car " << kmSpeed << endl;
			if (k < 30){
				speed += kmSpeed;
				++k;
			}
			else{
				avarageSpeed = (speed / k) ;
				cout << "\n ORTALAMA HIZ " << avarageSpeed << endl;
		        speed = 0;
				k = 0;
			}
		}

		 if (waitKey(20) == 27) {
		        break;
		 }

		 if(avarageSpeed > 0 && avarageSpeed <= 15)
		    putText(frame, "STOPPED"  , cv::Point(10, cflow.rows - 20), cv::FONT_HERSHEY_DUPLEX, 1, Scalar(255,0,0), 2);
		 else if(avarageSpeed > 15 && avarageSpeed <= 40)
	        putText(frame, "SLOW "  , cv::Point(10, cflow.rows - 20), cv::FONT_HERSHEY_DUPLEX, 1, Scalar(255,0,0), 2);
		 else if(avarageSpeed > 40 && avarageSpeed <= 70)
		    putText(frame, "NORMAL"  , cv::Point(10, cflow.rows - 20), cv::FONT_HERSHEY_DUPLEX, 1, Scalar(255,0,0), 2);
		 else if(avarageSpeed > 70 && avarageSpeed <= 100)
		    putText(frame, "FAST"  , cv::Point(10, cflow.rows - 20), cv::FONT_HERSHEY_DUPLEX, 1, Scalar(255,0,0), 2);
		 else if(avarageSpeed > 100)
	        putText(frame, "VERY FAST"  , cv::Point(10, cflow.rows - 20), cv::FONT_HERSHEY_DUPLEX, 1, Scalar(255,0,0), 2);

		 std::swap(prevFrame, frame);
	}
}

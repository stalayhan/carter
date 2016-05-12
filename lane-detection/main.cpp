/*TODO
 * improve edge linking
 * remove blobs whose axis direction doesnt point towards vanishing pt
 * lane prediction
*/

#include <algorithm>
#include <stdio.h>
#include <math.h>
#include <time.h> // for clock_t
#include "lane_detector.hpp"
#include <pthread.h> // for parallelisation
#include "utils.h"

using namespace std;
using namespace cv;
using namespace talayhan;

clock_t start, stop;

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

void get_frame_from_video(VideoCapture cap)
{
float  y_max = 0.0f;
int k = 0;

Mat prevFrame; //current frame
Ptr<BackgroundSubtractor> bg ;
bg = createBackgroundSubtractorMOG2();

    Mat frame, croppedImage;
    //Rect myROI(0, 0, RECT_FRAME_X2, RECT_FRAME_Y2);		    // restrict the video frame (x1, y1, x2, y2)

    double fps = cap.get(CV_CAP_PROP_FPS); 	                //get the frames per seconds of the video
    debug("Input video's Frame per seconds : %f", fps);

    cap.read(frame);
Mat img(frame);
    LaneDetector detector(frame);
resize(img, prevFrame, Size(img.size().width / 4, img.size().height / 4));
cvtColor(prevFrame, prevFrame, CV_BGR2GRAY);


    while(TRUE)
    {
        bool bSuccess = cap.read(frame);                    // read a new frame from video
	frame.copyTo(img);
        if (!bSuccess){
            debug("Cannot read the frame from video file");
            break;
        }
/* ------- */
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
imshow("imshow", cflow);

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


if(avarageSpeed > 0 && avarageSpeed <= 15)
	putText(cflow, "STOPPED"  , cv::Point(10, cflow.rows - 20), cv::FONT_HERSHEY_DUPLEX, 1, Scalar(255,0,0), 2);
else if(avarageSpeed > 15 && avarageSpeed <= 40)
putText(cflow, "SLOW "  , cv::Point(10, cflow.rows - 20), cv::FONT_HERSHEY_DUPLEX, 1, Scalar(255,0,0), 2);
else if(avarageSpeed > 40 && avarageSpeed <= 70)
	putText(cflow, "NORMAL"  , cv::Point(10, cflow.rows - 20), cv::FONT_HERSHEY_DUPLEX, 1, Scalar(255,0,0), 2);
else if(avarageSpeed > 70 && avarageSpeed <= 100)
	putText(cflow, "FAST"  , cv::Point(10, cflow.rows - 20), cv::FONT_HERSHEY_DUPLEX, 1, Scalar(255,0,0), 2);
else if(avarageSpeed > 100)
putText(cflow, "VERY FAST"  , cv::Point(10, cflow.rows - 20), cv::FONT_HERSHEY_DUPLEX, 1, Scalar(255,0,0), 2);

std::swap(prevFrame, frame);
/* ------- */
        //croppedImage = frame(myROI);
        //cvtColor(frame, frame, CV_BGR2GRAY);

        start = clock();
        detector.nextFrame(frame);
        stop = clock();

	//wait for 'esc' key press for 10 ms. If 'esc' key is pressed, break loop
        if(waitKey(DELAY_MS) == 27){
            cout << "video paused!, press q to quit, any other key to continue" << endl;
            if(waitKey(0) == 'q'){
                cout << "terminated by user" << endl;
                break;
            }
        }
    }
}
/*
 * Print the usage
 * */
void usage(){
    printf( "carter - car computer system\n"
            "carter [-c default=CAMERA-0] [-v VIDEO_PATH] [-h]\n"
            );
}
int checkArgument(int argc, const char **argv)
{
    if(argc == 0){
        usage();
        return 1;
    }

    return 0;
}

int main(int argc, const char **argv)
{
    checkArgument(argc, argv);
    VideoCapture cap(TEST_VIDEO_PATH);                      // open the video file for reading
    //VideoCapture cap(0);                      // open the video file for reading
    if ( !cap.isOpened() )  	                            // if not success, exit program
        log_err("Cannot open the video file:%s", TEST_VIDEO_PATH);

    //cap.set(CV_CAP_PROP_POS_MSEC, 300); //start the video at 300ms
    get_frame_from_video(cap);

    waitKey(0);
    destroyAllWindows();                                    // destroy all of the opened HighGUI windows
}

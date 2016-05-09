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

#ifdef SPEED_DETECTION
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
#endif

void get_frame_from_video(VideoCapture cap)
{
    Mat frame, croppedImage;
    Rect myROI(0, 0, RECT_FRAME_X2, RECT_FRAME_Y2);		    // restrict the video frame (x1, y1, x2, y2)

    double fps = cap.get(CV_CAP_PROP_FPS); 	                //get the frames per seconds of the video
    debug("Input video's Frame per seconds : %f", fps);

    cap.read(frame);
    LaneDetector detector(frame);

    while(TRUE)
    {
        bool bSuccess = cap.read(frame);                    // read a new frame from video
        if (!bSuccess){
            debug("Cannot read the frame from video file");
            break;
        }
        croppedImage = frame(myROI);
        cvtColor(croppedImage, croppedImage, CV_BGR2GRAY);

        start = clock();
        detector.nextFrame(croppedImage);
        stop = clock();
        debug("fps : %f", 1.0/(((double)(stop-start))/ CLOCKS_PER_SEC));
#ifdef SPEED_DETECTION
        // add speed detection module
		calcOpticalFlowFarneback(prevFrame, frame, flow, 0.5, 3, 15, 3, 5, 1.2, 1);
#endif
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

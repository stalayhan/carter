/*TODO
 * improve edge linking
 * remove blobs whose axis direction doesnt point towards vanishing pt
 * Parallelisation
 * lane prediction
*/

#include <algorithm>
#include <math.h>
#include <time.h> // for clock_t
#include "lane_detector.hpp"

#define TEST_VIDEO_PATH2 "/home/talayhhan/test11_divx6.1.1.avi"
#define TEST_VIDEO_PATH "/home/talayhhan/road-01.avi"

using namespace std;
using namespace cv;
using namespace talayhan;
clock_t start, stop;

void get_frame_from_video(const string path)
{
    Mat frame;
    Rect myROI(0, 0, 720, (480-220));		// restrict the video frame (x1, y1, x2, y2)
    VideoCapture cap(path); 				// open the video file for reading

    if ( !cap.isOpened() )  				// if not success, exit program
        cout << "Cannot open the video file" << endl;

    //cap.set(CV_CAP_PROP_POS_MSEC, 300); //start the video at 300ms

    double fps = cap.get(CV_CAP_PROP_FPS); 	//get the frames per seconds of the video
    cout << "Input video's Frame per seconds : " << fps << endl;

    cap.read(frame);
    LaneDetector detector(frame);

    while(1)
    {
        bool bSuccess = cap.read(frame); // read a new frame from video
        if (!bSuccess){
            cout << "Cannot read the frame from video file" << endl;
            break;
        }
        Mat croppedImage = frame(myROI);
        cvtColor(croppedImage, croppedImage, CV_BGR2GRAY);

        //start = clock();
        detector.nextFrame(croppedImage);
        //stop = clock();
        // cout<<"fps : "<<1.0/(((double)(stop-start))/ CLOCKS_PER_SEC)<<endl;

		//wait for 'esc' key press for 10 ms. If 'esc' key is pressed, break loop
        if(waitKey(10) == 27){
            cout << "video paused!, press q to quit, any other key to continue" << endl;
            if(waitKey(0) == 'q'){
                cout << "terminated by user" << endl;
                break;
            }
        }
    }
}

int main(void)
{
    get_frame_from_video(TEST_VIDEO_PATH);

    waitKey(0);
    destroyAllWindows(); // destroy all of the opened HighGUI windows
}

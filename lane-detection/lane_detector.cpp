/*
 * =====================================================================================
 *
 *       Filename:  lane_detector.cpp
 *
 *    Description:  LaneDetector class implementation
 *
 *        Version:  1.0
 *        Created:  23-04-2016 15:20:04
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Talayhan
 *   Organization:
 *
 * =====================================================================================
 */

#include "lane_detector.hpp"
#include <unistd.h>

namespace talayhan
{
	RNG rng(12345);

    LaneDetector::LaneDetector(Mat startFrame)
    {
        //currFrame = startFrame;                                    //if image has to be processed at original size

        currFrame = Mat(IMAGE_WIDTH, IMAGE_HEIGHT, CV_8UC1, ZERO_F);        // initialised the image size to 320x480
        resize(startFrame, currFrame, currFrame.size());                    // resize the input to required size

        temp      = Mat(currFrame.rows, currFrame.cols, CV_8UC1, ZERO_F);   //stores possible lane markings
        temp2     = Mat(currFrame.rows, currFrame.cols, CV_8UC1, ZERO_F);   //stores finally selected lane marks

        vanishingPt    = currFrame.rows/VANISHING_DIVIDER;                  //for simplicity right now
        ROIrows        = currFrame.rows - vanishingPt;                      //rows in region of interest
        minSize        = 0.00015 * (currFrame.cols * currFrame.rows);       //min size of any region to be selected as lane
        maxLaneWidth   = 0.045 * currFrame.cols;                            //approximate max lane width based on image size
        smallLaneArea  = 7 * minSize;
        longLane       = 0.3 * currFrame.rows;
        ratio          = 4;

        //these mark the possible ROI for vertical lane segments and to filter vehicle glare
        vertical_left  = 2*currFrame.cols/5;
        vertical_right = 3*currFrame.cols/5;
        vertical_top   = 2*currFrame.rows/3;

        namedWindow("lane", WINDOW_AUTOSIZE);
        namedWindow("midstep", WINDOW_AUTOSIZE);
        namedWindow("currframe", WINDOW_AUTOSIZE);
        namedWindow("laneBlobs",WINDOW_AUTOSIZE);

        getLane();
    }

    void LaneDetector::updateSensitivity()
    {
        int total=0, average =0;
        for(int i= vanishingPt; i<currFrame.rows; i++)
            for(int j= 0 ; j<currFrame.cols; j++)
                total += currFrame.at<uchar>(i,j);
        average = total/(ROIrows*currFrame.cols);
        cout<<"average : "<<average<<endl;
    }

    void LaneDetector::getLane()
    {
		int i,j;
        //medianBlur(currFrame, currFrame,5 );
        //updateSensitivity();

        //ROI = bottom half
        debug("currFrame.rows: %d - currFrame.cols: %d", currFrame.rows, currFrame.cols);
        for(i = vanishingPt; i < currFrame.rows; i++){
            for(j = 0; j < currFrame.cols; j++){
                temp.at<uchar>(i,j)    = 0;
                temp2.at<uchar>(i,j)   = 0;
            }
		}
#ifndef NDEBUG
        usleep(DEBUG_DELAY_MS); // delay for debug
#endif
        debug("vanishingPt: %d", vanishingPt);
        imshow("currframe", currFrame);
        blobRemoval();
    }

    void LaneDetector::markLane()
    {
		int i,j;
        for(i = vanishingPt; i < currFrame.rows; i++)
        {
            //IF COLOUR IMAGE IS GIVEN then additional check can be done
            // lane markings RGB values will be nearly same to each other(i.e without any hue)

            //min lane width is taken to be 5
            laneWidth = 5 + maxLaneWidth*(i-vanishingPt)/ROIrows;
            for(j = laneWidth; j < currFrame.cols - laneWidth; j++)
            {

                diffL = currFrame.at<uchar>(i,j) - currFrame.at<uchar>(i,j-laneWidth);
                diffR = currFrame.at<uchar>(i,j) - currFrame.at<uchar>(i,j+laneWidth);
                diff  =  diffL + diffR - abs(diffL - diffR);

                //1 right bit shifts to make it 0.5 times
                diffThreshLow = currFrame.at<uchar>(i,j)>>1;
                //diffThreshTop = 1.2*currFrame.at<uchar>(i,j);

                // both left and right differences can be made to contribute
                // at least by certain threshold (which is >0 right now)
                // total minimum Diff should be atleast more than 5 to avoid noise
                if (diffL > 0 && diffR > 0 && diff > 5)
                    if(diff >= diffThreshLow) /*&& diff<= diffThreshTop*/
                        temp.at<uchar>(i,j) = 255;
            }
        }

    }

    void LaneDetector::blobRemoval()
    {
        markLane();
		Scalar color( rand()&255, rand()&255, rand()&255 );
        // find all contours in the binary image
        temp.copyTo(binary_image);
        findContours(binary_image, contours,
                     hierarchy, CV_RETR_CCOMP,
                     CV_CHAIN_APPROX_SIMPLE);

        // for removing invalid blobs
        if (!contours.empty())
        {
            for (size_t i=0; i<contours.size(); ++i)
            {
                //====conditions for removing contours====//
                contour_area = contourArea(contours[i]) ;

                //blob size should not be less than lower threshold
                if(contour_area > minSize)
                {
                    rotated_rect    = minAreaRect(contours[i]);
                    sz              = rotated_rect.size;
                    bounding_width  = sz.width;
                    bounding_length = sz.height;

                    //openCV selects length and width based on their orientation
                    //so angle needs to be adjusted accordingly
                    blob_angle_deg = rotated_rect.angle;
                    if (bounding_width < bounding_length)
                        blob_angle_deg = 90 + blob_angle_deg;

                    //if such big line has been detected then it has to be a (curved or a normal)lane
                    if(bounding_length > longLane || bounding_width > longLane)
                    {
                        drawContours(currFrame, contours,i, color, CV_FILLED, 8);
                        drawContours(temp2, contours,i, color, CV_FILLED, 8);
                    }

                    //angle of orientation of blob should not be near horizontal or vertical
                    //vertical blobs are allowed only near center-bottom region, where centre lane mark is present
                    //length:width >= ratio for valid line segments
                    //if area is very small then ratio limits are compensated
                    else if ((blob_angle_deg <-10 || blob_angle_deg >-10 ) &&
                             ((blob_angle_deg > -70 && blob_angle_deg < 70 ) ||
                              (rotated_rect.center.y > vertical_top &&
                               rotated_rect.center.x > vertical_left && rotated_rect.center.x < vertical_right)))
                    {

                        if ((bounding_length/bounding_width)>=ratio || (bounding_width/bounding_length)>=ratio
                                ||(contour_area< smallLaneArea &&  ((contour_area/(bounding_width*bounding_length)) > .75) &&
                                   ((bounding_length/bounding_width)>=2 || (bounding_width/bounding_length)>=2)))
                        {
                            drawContours(currFrame, contours,i, color, CV_FILLED, 8);
                            drawContours(temp2, contours,i, color, CV_FILLED, 8);
                        }
                    }
                }
            }
        }
        imshow("midstep", temp);
        imshow("laneBlobs", temp2);
        imshow("lane",currFrame);
    }

    void LaneDetector::nextFrame(Mat &nxt)
    {
        //currFrame = nxt;                        //if processing is to be done at original size

        resize(nxt ,currFrame, currFrame.size()); //resizing the input image for faster processing
        getLane();
    }

    Mat LaneDetector::getResult()
    {
        return temp2;
    }
} /* namespace Talayhan */

/*
 * =====================================================================================
 *
 *       Filename:  lane_detector.hpp
 *
 *    Description:  Header file for LaneDetector class
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

#ifndef _LANE_DETECTOR_H_
#define _LANE_DETECTOR_H_

#include <opencv2/core/core.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

namespace talayhan{

    class LaneDetector
    {
    public:
        Mat currFrame; //stores the upcoming frame
        Mat temp;      //stores intermediate results
        Mat temp2;     //stores the final lane segments

        int diff, diffL, diffR;
        int laneWidth;
        int diffThreshTop;
        int diffThreshLow;
        int ROIrows;
        int vertical_left;
        int vertical_right;
        int vertical_top;
        int smallLaneArea;
        int longLane;
        int vanishingPt;
        float maxLaneWidth;

        // to store various blob properties
        Mat binary_image; // used for blob removal
        int minSize;
        int ratio;
        float contour_area;
        float blob_angle_deg;
        float bounding_width;
        float bounding_length;
        Size2f sz;
        vector< vector<Point> > contours;
        vector<Vec4i> hierarchy;
        RotatedRect rotated_rect;

        LaneDetector(Mat startFrame);
        void updateSensitivity();
        void getLane();
        void markLane();
        void blobRemoval();
        void nextFrame(Mat &nxt);
        Mat getResult();

    };/* end of class LaneDetector */

} /* namespace Talayhan */
#endif /* _LANE_DETECTOR_H_ */

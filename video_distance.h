/*
*
* Copyright 2016 Ignacio San Roman Lana
*
* This file is part of Video_Distance
*
* Video_Distance is free software: you can redistribute it and/or
* modify it under the terms of the GNU General Public License as
* published by the Free Software Foundation, either version 3 of the
* License, or (at your option) any later version.
*
* Video_Distance is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
* General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with Video_Distance. If not, see http://www.gnu.org/licenses/.
*
* For those usages not covered by this license please contact with
* isanromanlana@gmail.com
*/

#ifndef VIDEO_DISTANCE_H
#define VIDEO_DISTANCE_H

#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>

using namespace std;
using namespace cv;

class Video_Distance
{
public:
    Video_Distance(int thr, float tam_circle);
    void extract_feature(vector<Vec3b> pixels, Point3f &features);
    float dist(cv::Point3f p1, cv::Point3f p2);
    float dtw(vector<cv::Point3f> t1, vector<cv::Point3f> t2);
    float calc_distance(string video_a, string video_b);
private:
    int thr;
    float tam_circle;
};

#endif // VIDEO_DISTANCE_H

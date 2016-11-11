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

#include "video_distance.h"

Video_Distance::Video_Distance(int thr=1, float tam_circle=0.5)
{
    this->thr=thr;
    this->tam_circle=tam_circle;
}

//Features are percentages of blue, green and red
void Video_Distance::extract_feature(vector<Vec3b> pixels, Point3f &features){
    vector<float> f_count;
    f_count.push_back(0);
    f_count.push_back(0);
    int cont=0;
    for(uint i=0;i<pixels.size();i++){
        Vec3b pixel = pixels[i];
        if(pixel[0]>pixel[1] && pixel[0]>pixel[2]){
            f_count[0]++;
            cont++;
        }
        else if(pixel[1]>pixel[0] && pixel[1]>pixel[2]){
            f_count[1]++;
            cont++;
        }
        else if(pixel[2]>pixel[0] && pixel[2]>pixel[1])
            cont++;
    }
    if(f_count[0]>0 && cont>0)
        f_count[0]=100*f_count[0]/cont;
    else
        f_count[0]=0;

    if(f_count[1]>0 && cont>0)
        f_count[1]=100*f_count[1]/cont;
    else
        f_count[1]=0;

    features.x=f_count[0];
    features.y=f_count[1];
    features.z=100-(features.x+features.y);
}

//Euclidean distance
float Video_Distance::dist(cv::Point3f p1, cv::Point3f p2) {
    return sqrt(pow(((float)p1.x-(float)p2.x), 2)+pow(((float)p1.y-(float)p2.y), 2)+pow(((float)p1.z-(float)p2.z), 2));
}


//DTW as a distance
float Video_Distance::dtw(vector<cv::Point3f> t1, vector<cv::Point3f> t2){
    cv::Mat cost=cv::Mat::zeros(t1.size(),t2.size(),CV_32FC1);
    cost.at<float>(0,0)=dist(t1[0],t2[0]);
    for(int k=1; k<cost.rows; k++)
        cost.at<float>(k,0)=cost.at<float>(k-1,0)+dist(t1[k],t2[0]);
    for(int k=1; k<cost.cols; k++)
        cost.at<float>(0,k)=cost.at<float>(0,k-1)+dist(t1[0],t2[k]);
    for(int k=1; k<cost.rows; k++)
        for(int l=1; l<cost.cols; l++)
            cost.at<float>(k,l)=std::min(cost.at<float>(k-1,l), std::min(cost.at<float>(k,l-1),
                       cost.at<float>(k-1,l-1)))+dist(t1[k],t2[l]);
    return cost.at<float>(cost.rows-1,cost.cols-1)/(cost.rows+cost.cols);
}

float Video_Distance::calc_distance(string video_a, string video_b){
    VideoCapture cap1(video_a);
    VideoCapture cap2(video_b);

    if(!cap1.isOpened()){
        cout<<"ERROR Video 1 could no be loaded"<<endl;
        return -1;
    }
    if(!cap2.isOpened()){
        cout<<"ERROR Video 2 could no be loaded"<<endl;
        return -1;
    }

    //Features vectors
    vector<Point3f> video1_features,video2_features;

    //First frame of video 1
    Mat frame1;
    cap1>>frame1;

    //Calc mask to take only the center
    Mat mask1=Mat::zeros(frame1.rows, frame1.cols, CV_8UC1);
    float radius1;
    if(mask1.rows<mask1.cols)
        radius1=(float)mask1.rows*this->tam_circle/2;
    else
        radius1=(float)mask1.cols*this->tam_circle/2;
    circle(mask1, Point(mask1.cols/2,mask1.rows/2),(int)radius1,Scalar::all(255),-1);

    //Bucle frame after frame
    do{
        //Apply mask
        Mat frame1_seg;
        frame1.copyTo(frame1_seg,mask1);
        Mat frame_roi=frame1_seg(Rect((frame1_seg.cols/2)-radius1,(frame1_seg.rows/2)-radius1,
                                  2*radius1,2*radius1));

        //Get keypoints
        Mat gray_roi;
        cvtColor(frame_roi,gray_roi,CV_BGR2GRAY);
        vector<KeyPoint> keypoints;
        FAST(gray_roi,keypoints,this->thr);

        //Get pixel values from keypoints
        vector<Vec3b> pixels;
        for(uint i=0; i<keypoints.size(); i++){
            Vec3b pixel = frame_roi.at<Vec3b>(keypoints[i].pt.x, keypoints[i].pt.y);
            pixels.push_back(pixel);
        }

        //Get features
        Point3f feature;
        extract_feature(pixels,feature);
        video1_features.push_back(feature);
    }while(cap1.read(frame1));


    //First frame of video 2
    Mat frame2;
    cap2>>frame2;

    //Calc mask to take only the center
    Mat mask2=Mat::zeros(frame2.rows, frame2.cols, CV_8UC1);
    float radius2;
    if(mask2.rows<mask2.cols)
        radius2=(float)mask2.rows*this->tam_circle/2;
    else
        radius2=(float)mask2.cols*this->tam_circle/2;
    circle(mask2, Point(mask2.cols/2,mask2.rows/2),(int)radius2,Scalar::all(255),-1);

    //Bucle frame after frame
    do{
        //Apply mask
        Mat frame2_seg;
        frame2.copyTo(frame2_seg,mask2);
        Mat frame_roi=frame2_seg(Rect((frame2_seg.cols/2)-radius2,(frame2_seg.rows/2)-radius2,
                                  2*radius2,2*radius2));

        //Get keypoints
        Mat gray_roi;
        cvtColor(frame_roi,gray_roi,CV_BGR2GRAY);
        vector<KeyPoint> keypoints;
        FAST(gray_roi,keypoints,thr);

        //Get pixel values from keypoints
        vector<Vec3b> pixels;
        for(uint i=0; i<keypoints.size(); i++){
            Vec3b pixel = frame_roi.at<Vec3b>(keypoints[i].pt.x, keypoints[i].pt.y);
            pixels.push_back(pixel);
        }

        //Get features
        Point3f feature;
        extract_feature(pixels,feature);
        video2_features.push_back(feature);
    }while(cap2.read(frame2));

    //Calc distance between features
    float distance=dtw(video1_features,video2_features);
    return(distance);
}

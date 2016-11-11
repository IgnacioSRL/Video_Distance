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

#include <iostream>
#include <video_distance.h>

using namespace std;

int main()
{
    string video1="video1.avi";
    string video2="video2.mp4";

    Video_Distance v_dist(1,0.5);

    float dist=v_dist.calc_distance(video1,video2);
    cout<<dist<<endl;
}

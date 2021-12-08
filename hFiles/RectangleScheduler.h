#pragma once
#include<vector>
#include "Timeline.h"
#include "Rectangle.h"

//任务矩形规划
class RectangleScheduler{

public:
    vector<Timeline> lines;
    double totalTime;
    vector<vector<Rectangle>> rects;//(1-10)-Rectangle

    RectangleScheduler(vector<Timeline> timeline, vector<vector<Rectangle>> rectangles){
        lines = timeline;
        rects = rectangles;
        totalTime = 0.0;
    }
    //分配一个job Rectangle
    void schedule(int id/*job ID*/, vector<vector<vector<tuple<int, int, double, double>>>>, vector<vector<tuple<int, int, int>>>, vector<double>, vector<int>);
    //节点的切分
    void lineSplit(Rectangle rect, int idx, int cores);

    //每添加一个矩形对timeline进行重新排序
    void reSort(){
        sort(lines.begin(), lines.end());
    }
};
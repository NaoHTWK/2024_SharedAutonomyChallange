#pragma once

#include <line.h>
#include <point_2d.h>
#include <soccerfield.h>

class PositionArea {
public:
    const htwk::point_2d startPoint;
    const htwk::point_2d endPoint;
    const htwk::point_2d stepSize;

    PositionArea(htwk::point_2d startPoint, htwk::point_2d endPoint, htwk::point_2d _stepSize)
        : startPoint(borderCheck(startPoint)),
          endPoint(borderCheck(endPoint)),
          stepSize(_stepSize),
          left_top_bottom(startPoint, {startPoint.x, endPoint.y}),
          right_top_bottom({endPoint.x, startPoint.y}, endPoint),
          top_left_right(startPoint, {endPoint.x, startPoint.y}),
          bottom_left_right({startPoint.x, endPoint.y}, endPoint) {}

    htwk::point_2d borderCheck(const htwk::point_2d& point) {
        return {clamp(point.x, -SoccerField::length() * 0.5f, SoccerField::length() * 0.5f),
                clamp(point.y, -SoccerField::width() * 0.5f, SoccerField::width() * 0.5f)};
    }

    bool isWithinArea(const htwk::point_2d& point) const {
        return within(point.x, startPoint.x, endPoint.x) && within(point.y, startPoint.y, endPoint.y);
    }

    bool isCrossingArea(const htwk::Line& line) const {
        return left_top_bottom.intersect_within(line) || right_top_bottom.intersect_within(line) ||
               top_left_right.intersect_within(line) || bottom_left_right.intersect_within(line);
    }

private:
    const htwk::Line left_top_bottom;
    const htwk::Line right_top_bottom;
    const htwk::Line top_left_right;
    const htwk::Line bottom_left_right;
};

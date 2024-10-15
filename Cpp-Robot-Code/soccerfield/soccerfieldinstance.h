#ifndef SOCCERFIELDINSTANCE_H
#define SOCCERFIELDINSTANCE_H

#include <utility>
#include <vector>

#include <line.h>
#include <point_2d.h>

class SoccerFieldInstance {
public:
    struct Parameter {
        float fieldLength;
        float fieldWidth;
        float fieldBorder;
        float circleDiameter;
        float goalPostDistance;
        float goalDepth;
        float penaltySpot2Goal;
        float penaltyAreaWidth;
        float penaltyAreaHeight;
        float goalBoxWidth;
        float goalBoxHeight;
        float lineWidth;
        bool hasGoalBox;
    };

    SoccerFieldInstance(Parameter params);

    std::vector<htwk::point_2d> getGoalPosts() const {
        return goalPosts;
    }

    std::vector<htwk::Line> getFieldLines() const {
        return fieldLines;
    }

    std::vector<htwk::Line> getBorderLines() const {
        return borderLines;
    }

    float circleDiameter() const {
        return params.circleDiameter;
    }

    std::vector<htwk::point_2d> getPenaltySpots() {
        return penaltySpots;
    }

    float fieldLength() const {
        return params.fieldLength;
    }

    float fieldWidth() const {
        return params.fieldWidth;
    }

    float penaltySpot2Goal() const {
        return params.penaltySpot2Goal;
    }

    float penaltyAreaWidth() const {
        return params.penaltyAreaWidth;
    }

    float penaltyAreaHeight() const {
        return params.penaltyAreaHeight;
    }

    float goalBoxWidth() const {
        return params.hasGoalBox ? params.goalBoxWidth : params.penaltyAreaWidth;
    }

    float goalBoxHeight() const {
        return params.hasGoalBox ? params.goalBoxHeight : params.penaltyAreaHeight;
    }

    float lineWidth() const {
        return params.lineWidth;
    }

    bool hasGoalBox() const {
        return params.hasGoalBox;
    }

    float goalPostDistance() const {
        return params.goalPostDistance;
    }

    const Parameter& getParameter() const {
        return params;
    }
protected:
    Parameter params;
    static constexpr float eps = 0.0001f;

    std::vector<htwk::point_2d> goalPosts;
    std::vector<htwk::Line> fieldLines;
    std::vector<htwk::Line> borderLines;
    std::vector<htwk::point_2d> penaltySpots;
};

#endif  // SOCCERFIELDINSTANCE_H

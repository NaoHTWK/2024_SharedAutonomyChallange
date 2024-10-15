#include <soccerfield.h>

#include <soccerfielddefinitions.h>

using namespace htwk;

std::vector<point_2d> SoccerField::getGoalPosts() {
    return instance.getGoalPosts();
}

std::vector<Line> SoccerField::getFieldLines() {
    return instance.getFieldLines();
}

std::vector<Line> SoccerField::getBorderLines() {
    return instance.getBorderLines();
}

float SoccerField::circleDiameter() {
    return instance.circleDiameter();
}

std::vector<point_2d> SoccerField::penaltySpots() {
    return instance.getPenaltySpots();
}

float SoccerField::length() {
    return instance.fieldLength();
}

float SoccerField::width() {
    return instance.fieldWidth();
}

float SoccerField::penaltySpot2Goal() {
    return instance.penaltySpot2Goal();
}

float SoccerField::penaltyAreaWidth() {
    return instance.penaltyAreaWidth();
}

float SoccerField::penaltyAreaHeight() {
    return instance.penaltyAreaHeight();
}

float SoccerField::goalBoxWidth() {
    return instance.goalBoxWidth();
}

float SoccerField::goalBoxHeight() {
    return instance.goalBoxHeight();
}

float SoccerField::goalPostDistance() {
    return instance.goalPostDistance();
}

SoccerFieldInstance SoccerField::instance = SoccerFieldInstance(getSoccerField("9x6"));

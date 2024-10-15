#pragma once

#include <line.h>
#include <point_2d.h>
#include <soccerfieldinstance.h>

#include <cmath>
#include <list>
#include <vector>

#include "envinfo.h"
#include "position.h"

class SoccerField {
public:
    static std::vector<htwk::point_2d> getGoalPosts();
    static std::vector<htwk::Line> getFieldLines();
    static std::vector<htwk::Line> getBorderLines();
    static std::vector<htwk::point_2d> penaltySpots();

    static float length();
    static float width();
    static float circleDiameter();

    static float penaltySpot2Goal();
    static float penaltyAreaWidth();
    static float penaltyAreaHeight();
    static float goalBoxHeight();
    static float goalBoxWidth();
    static float goalPostDistance();

    static htwk::point_2d ownGoal() {
        return {-instance.fieldLength() / 2.f, 0.f};
    }
    static htwk::point_2d oppGoal() {
        return {instance.fieldLength() / 2.f, 0.f};
    }
    static htwk::point_2d ownPenaltySpot() {
        return {-instance.fieldLength() / 2.f + penaltySpot2Goal(), 0.f};
    }
    static bool hasGoalBox() {
        return instance.hasGoalBox();
    }

    static const SoccerFieldInstance::Parameter& getParameter() {
        return instance.getParameter();
    }

    static void configureSoccerField(SoccerFieldInstance::Parameter params) {
        instance = SoccerFieldInstance(params);
    }

    // When standing in the goal, odd jersey numbers are on the left, even on the right, looking into the field at 90
    // degrees, smaller numbers closer to the goal, spaced evenly.
    static std::vector<Position> startingPositions() {
        std::vector<Position> positions;
        static_assert(WM_NUM_PLAYERS == 7);
        for (int i = 0; i < 7; i++) {
            if (i % 2 == 0) {
                positions.emplace_back(-instance.fieldLength() / 2.f / 5.f * (4 - i / 2), instance.fieldWidth() / 2.f,
                                       -M_PIf / 2.f);
            } else {
                positions.emplace_back(-instance.fieldLength() / 2.f / 4.f * (3 - i / 2), -instance.fieldWidth() / 2.f,
                                       M_PIf / 2.f);
            }
        }
        positions[0] = {-instance.fieldLength() / 2.f + 0.3f, 0, 0}; // goaly | Defender
        positions[1] = {-2.25, 1.5, 0}; // Attacker
        positions[2] = {-1.0, 0, 0}; // Manual
        return positions;
    }

private:
    SoccerField() = default;

    static SoccerFieldInstance instance;
};

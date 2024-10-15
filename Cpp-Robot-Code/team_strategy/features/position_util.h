#pragma once

#include <map>
#include <optional>
#include <set>
#include <tuple>

#include <envinfo.h>
#include <position.h>
#include <position_area.h>
#include <line.h>

extern htwk::point_2d generatePosition(const PositionArea &posArea,
                                       std::function<float(const htwk::point_2d &)> scoreFunc);

/**
 * @brief getGeneratedPositionScores Return all scores and positions which where generated via generatePosition
 * @return key Position(x, y), value is the score
 * @note This is only interesting for debugging or team strategy debugger purposes. (if you call this you are stupid :-P)
 */
extern std::map<std::tuple<float,float>, float> getGeneratedPositionScores();
void clearGeneratedPositionScores();

extern float distToClosestNeighbor(const htwk::point_2d &pos, const std::map<PlayerIdx, Robot> &alive_robots,
                                   PlayerIdx player_idx);
float distToClosestObstacle(const Robot& bot, const std::vector<ObstacleObservation>& obstacles);
extern std::optional<float> distToClosestNeighborGreaterX(const htwk::point_2d &pos, const std::map<PlayerIdx, Robot> &alive_robots,
                                   PlayerIdx player_idx);
extern std::optional<float> distToClosestNeighborLesserX(const htwk::point_2d &pos, const std::map<PlayerIdx, Robot> &alive_robots,
                                            PlayerIdx player_idx);
extern PlayerIdx nearestPlayerToPoint(const htwk::point_2d &point, const std::map<PlayerIdx, Robot> &alive_robots);
extern PlayerIdx nearestPlayerToLine(htwk::Line &line, const std::map<PlayerIdx, Robot> &alive_robots);
extern PlayerIdx nearestFieldPlayerToPoint(const htwk::point_2d &point, const std::set<PlayerIdx> &except,
                                           const std::map<PlayerIdx, Robot> &alive_robots);
extern PlayerIdx nearestPlayerToPlayer(const PlayerIdx playerIdx, const std::map<PlayerIdx, Robot> &alive_robots);
extern int playerWithinArea(const std::map<PlayerIdx, Robot> &alive_robots, PlayerIdx player_idx,
                            const PositionArea &posArea);
extern htwk::point_2d smoothPosition(std::deque<htwk::point_2d> &history, size_t max_size);
extern htwk::point_2d maxPosition(std::deque<htwk::point_2d> &history, size_t max_size);


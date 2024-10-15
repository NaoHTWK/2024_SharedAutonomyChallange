#include "collect_data.h"

#include <collectdataorder.h>

MotionCommand CollectDataAgent::proceed(std::shared_ptr<Order> order, WorldModel* world_model) {
    if (isOrder<CollectDataOrder>(order))
        return MotionCommand::Stand(HeadFocus::LOC);

    return MotionCommand::Nothing;
}

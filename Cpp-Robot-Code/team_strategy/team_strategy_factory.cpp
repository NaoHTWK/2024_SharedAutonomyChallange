#include "team_strategy_factory.h"
// #include "dbhc_strategy.h"

// #include <penalty_strategy.h>
// #include <robocup_strategy.h>
// #include <stay_strategy.h>
// #include <tutorial_strategy.h>
// #include <moveball_strategy.h>
#include <shared_autonomy_strategy.h>
#include <shared_manual_strategy.h>

TeamStrategy* TeamStrategyFactory::create(const std::string& strategyName, uint8_t team_nr, PlayerIdx player_idx,
                                          WorldModel* world_model, GameControllerClient* gc_client
                                          /*, TeamComm* team_comm*/) {
    /*
    if (strategyName == "penalty") {
        return new PenaltyStrategy(team_nr, player_idx, world_model, gc_client);
    } else if (strategyName == "robocup") {
        return new RobocupStrategy(team_nr, player_idx, world_model, gc_client, team_comm);
    } else if (strategyName == "tutorial") {
        return new TutorialStrategy(team_nr, player_idx, world_model, gc_client);
    }else if(strategyName == "dbhc"){
        return new DBHCStrategy(team_nr, player_idx, world_model, gc_client);
    }else if(strategyName == "stay"){
        return new StayStrategy(team_nr, player_idx, world_model, gc_client);
    }else if(strategyName == "moveball"){
        return new MoveBallStrategy(team_nr, player_idx, world_model, gc_client);
    }
    */
    if(strategyName == "sharedautonomy"){
        return new SharedAutonomyStrategy(team_nr, player_idx, world_model, gc_client/*, team_comm*/);
    }else if(strategyName == "sharedmanual"){
        return new SharedManualStrategy(team_nr, player_idx, world_model, gc_client/*, team_comm*/);

    }
throw(std::invalid_argument("Unknown teamstrategy of name:" + strategyName));
}

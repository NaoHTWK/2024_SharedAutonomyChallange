#include <noorder.h>
#include <team_strategy.h>

std::shared_ptr<Order> TeamStrategy::proceed(RoboCupGameControlData gc_data_) {
    gc_data = gc_data_;
    std::shared_ptr<Order> o = std::make_shared<NoOrder>();
    robots = world_model->getAliveNetworkRobots();
    switch (gc_data.state) {
        case STATE_READY:
            time_us_lastKickoff = time_us();
            if (gc_data.setPlay != SET_PLAY_PENALTY_KICK){
                o = ready();
            }else{
                o = setPlay_ready();
            }
            break;
        case STATE_SET:
            time_us_lastKickoff = time_us();
            o = set();
            break;
        case STATE_PLAYING:
            time_us_inPlay = time_us() - time_us_lastKickoff;
            if (gc_data.setPlay == SET_PLAY_NONE) {
                o = play();
            } else {
                o = setPlay_play();
            }
            break;
        default:
            time_us_lastKickoff = time_us();
    }
    last_robots = robots;
    prev_state = gc_data.state;
    prev_setPlay = gc_data.setPlay;
    return o;
}

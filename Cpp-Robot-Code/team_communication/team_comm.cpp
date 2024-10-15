#include "team_comm.h"

#include <arpa/inet.h>
#include <sockethelper.h>
#include <unistd.h>

#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/udp.hpp>

namespace htwk {
using namespace std;
//using namespace protobuf::worldmodel;
using boost::asio::ip::udp;

bool TeamComm::sendEvent(Type type, bool important, const char* sender_func_name, uint32_t line_number) {
    std::string sender_position = std::string(sender_func_name) + ":" + std::to_string(line_number);
    if (remaining_message_budget <= hard_safety) {
        //log->debug(sender_position.c_str(), sender_position.length() + 1, "HardSafetyDeniedTCFunctionName");
        return false;
    }
    int64_t this_event_us = time_us();
    if (last_event_us > this_event_us - 1._s) {
        //log->debug(sender_position.c_str(), sender_position.length() + 1, "FrequencyDeniedTCFunctionName");
        return false;
    }
    remaining_message_budget--;
    //TeamCommMsg message;
    Robot myself = world_model->getOwnRobot();
    // message.set_player_idx(myself.idx);
    // message.set_is_fallen(myself.isFallen);
    // message.mutable_pos()->set_x(myself.pos.x);
    // message.mutable_pos()->set_y(myself.pos.y);
    // message.mutable_pos()->set_a(myself.pos.a);
    // if (type == Type::STRIKER || type == Type::NEW_STRIKER) {
    //     message.set_ball_age_us(0);
    //     message.mutable_ball_pos()->set_x(0);
    //     message.mutable_ball_pos()->set_y(0);
    //     message.set_has_ball(true);
    // }
    // message.set_loc_quality(myself.loc_quality);
    // message.set_type(static_cast<TeamCommMsg_Type>(type));
    // message.set_sent_time(myself.sent_time);
    // for (const TrackedObject& obstacle : myself.obstacles) {
    //     Obstacle* opp = message.add_opponents();
    //     opp->set_confindence(obstacle.confidence);
    //     opp->set_ownteamprob(*obstacle.ownTeamProb);
    //     Point2D* pos = opp->mutable_pos();
    //     pos->set_x(obstacle.posRel.x);
    //     pos->set_y(obstacle.posRel.y);
    // }

    //log->debug(message, "SendTC");
    //log->debug(sender_position.c_str(), sender_position.length() + 1, "SendTCFunctionName");
    // string msg = message.SerializeAsString();
    // while (msg.size() > max_message_size && message.opponents().size() > 0) {
    //     cerr << "Message is too big, removing an obstacle." << endl;
    //     message.mutable_opponents()->RemoveLast();
    //     msg = message.SerializeAsString();
    // }
    // if (msg.size() > max_message_size) {
    //     cerr << "Message is too big, no more obstacles to remove!!!" << endl;
    // } else {
    //     last_event_us = this_event_us;
    //     sendBroadcast(10000 + team_id, (uint8_t*)msg.data(), msg.size());
    //     if (important) {
    //         sendBroadcast(10000 + team_id, (uint8_t*)msg.data(), msg.size());
    //         remaining_message_budget--;
    //     }
    // }
    // world_model->updateNetworkSelf(message);
    //log->infoMsg("Message sent:");
    //log->infoMsg("%s", message.DebugString().c_str());
    return true;
}

void TeamComm::continuousSender() {
    while (true) {
        usleep(100_ms);
        int64_t now = time_us();
        Type type = continuous_type;
        if (remaining_message_budget <= soft_safety || !contains(continuous_message_ratios, type) ||
            (now - last_continuous_timestamp) / (float)1_s <
                    getContinuousDuration() / continuous_message_ratios.at(type))
            continue;
        last_continuous_timestamp = now;
        sendEvent(type, false, __PRETTY_FUNCTION__, __LINE__);
    }
}



void TeamComm::receive() {
    boost::asio::io_service io_service;
    boost::system::error_code ec;
    udp::socket socket(io_service);

    udp::endpoint endpoint = udp::endpoint(boost::asio::ip::address_v4::from_string("0.0.0.0"),
                                           static_cast<unsigned short>(10000 + team_id));

    auto error_handling = [this](const char* op, boost::system::error_code ec) {
        if (ec) {
            //log->errMsg("WMReceiver: Error %s socket because of error: %s.", op, ec.message().c_str());
            exit(-1);
        }
    };

    error_handling("open", socket.open(udp::v4(), ec));
    error_handling("set_option", socket.set_option(boost::asio::socket_base::reuse_address(true), ec));
    error_handling("bind", socket.bind(endpoint, ec));

    // TeamCommMsg message;
    // char msg_buf[max_message_size];
    // while (true) {
    //     udp::endpoint sender;
    //     size_t recv_size = socket.receive_from(boost::asio::buffer(&msg_buf, max_message_size), sender, 0, ec);
    //     //log->infoMsg("Message received:");
//
    //     if (ec) {
    //         //log->errMsg("Error receiving team comm: %s.", ec.message().c_str());
    //         continue;
    //     }
    //     if (!message.ParseFromArray(msg_buf, recv_size)) {
    //         //log->errMsg("Error parsing protobuf of size %zu from %s.", recv_size, sender.address().to_string().c_str());
    //         continue;
    //     }
    //     Robot myself = world_model->getOwnRobot();
    //     if (message.player_idx() == myself.idx) {
    //         continue;
    //     }
    //     //log->infoMsg("%s", message.DebugString().c_str());
    //     if (message.player_idx() < 0 || message.player_idx() >= WM_NUM_PLAYERS) {
    //         //log->errMsg("Received team comm for player idx %d -> something is wrong!", message.player_idx());
    //         continue;
    //     }
    //     world_model->update(message);
    //     //log->debug(message, "ReceivedTC");
    //     if (static_cast<Type>(message.type()) == Type::NEW_STRIKER ||
    //         static_cast<Type>(message.type()) == Type::NEW_STRIKER_MOVE) {
    //         std::unique_lock<std::mutex> lck(mtx);
    //         new_striker_ids.push_back(message.player_idx());
    //     }
    //     if (static_cast<Type>(message.type()) == Type::POSITION_UPDATE) {
    //         std::unique_lock<std::mutex> lck(mtx);
    //         is_newPositionUpdate_event = true;
    //     }
    //     if (static_cast<Type>(message.type()) == Type::SWITCH_TO_READY) {
    //         std::unique_lock<std::mutex> lck(mtx);
    //         is_newSwitchReady_event = true;
    //     }
    // }
}

std::vector<PlayerIdx> TeamComm::notify_newStriker() {
    std::unique_lock<std::mutex> lck(mtx);
    if (!new_striker_ids.empty()) {
        //log->infoMsg("notify new striker");
    }
    std::vector<PlayerIdx> ret;
    ret.swap(new_striker_ids);
    return ret;
}

bool TeamComm::notify_newPositionUpdate() {
    std::unique_lock<std::mutex> lck(mtx);
    if (is_newPositionUpdate_event) {
        //log->infoMsg("notify new pos update");
        is_newPositionUpdate_event = false;  // reset value for onetime notification
        return true;
    }
    return false;
}

bool TeamComm::notify_switchToReady() {
    std::unique_lock<std::mutex> lck(mtx);
    if (is_newSwitchReady_event) {
        //log->infoMsg("notify new switch to ready");
        is_newSwitchReady_event = false;  // reset value for onetime notification
        return true;
    }
    return false;
}

}  // namespace htwk

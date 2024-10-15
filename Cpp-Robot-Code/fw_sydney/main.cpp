#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <iostream>

namespace bpo = boost::program_options;

int main(int argc, char* argv[]) {
    // The Shared Autonomy Challenge logic is implemented in shared_manual_strategy and shared_autonomy_strategy which are both run via team_strategy_factory. All these are located in the team_strategy folder. The code of these is fully included in the release. Please integrate them into your code that is run on the robot to run the SAC behaviour.

    std::cout << "It works" << std::endl;
    return 0;
}

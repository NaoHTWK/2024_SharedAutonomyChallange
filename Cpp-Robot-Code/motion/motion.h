#pragma once

#include <string>
#include <vector>

#include <boost/filesystem/fstream.hpp>

#include <joints.h>

class Motion {
  public:
    Motion() {}
    explicit Motion(const std::string& filename);

    void loadV2(boost::filesystem::ifstream& f);

    std::vector<Joints> motion;
    std::vector<bool> stabilize;
};

// #include <flightrecorderlog.h>
#include <signalhandling.h>
#include <soccerfielddefinitions.h>
//#include <tts.h>

#include <boost/filesystem.hpp>
//#include <nlohmann/json.hpp>

namespace {
//using nlohmann::json;
namespace fs = boost::filesystem;

SoccerFieldInstance::Parameter getSoccerField9x6() {
    return {.fieldLength = 9.f,
            .fieldWidth = 6.f,
            .fieldBorder = 0.7f,
            .circleDiameter = 1.5f,
            .goalPostDistance = 1.6f,
            .goalDepth = 0.475f,
            .penaltySpot2Goal = 1.3f,
            .penaltyAreaWidth = 1.65f,
            .penaltyAreaHeight = 4.f,
            .goalBoxWidth = 0.6f,
            .goalBoxHeight = 2.2f,
            .lineWidth = 0.05f,
            .hasGoalBox = true};
};

SoccerFieldInstance::Parameter getSoccerField9x6old() {
    return {.fieldLength = 9.f,
            .fieldWidth = 6.f,
            .fieldBorder = 0.7f,
            .circleDiameter = 1.5f,
            .goalPostDistance = 1.6f,
            .goalDepth = 0.475f,
            .penaltySpot2Goal = 1.3f,
            .penaltyAreaWidth = 0.6f,
            .penaltyAreaHeight = 2.2f,
            .lineWidth = 0.05f,
            .hasGoalBox = false};
};

SoccerFieldInstance::Parameter getSoccerField75x5() {
    return {.fieldLength = 7.5f,
            .fieldWidth = 5.f,
            .fieldBorder = 0.7f,
            .circleDiameter = 1.5f,
            .goalPostDistance = 1.6f,
            .goalDepth = 0.475f,
            .penaltySpot2Goal = 1.3f,
            .penaltyAreaWidth = 1.65f,
            .penaltyAreaHeight = 3.5f,
            .goalBoxWidth = 0.6f,
            .goalBoxHeight = 2.2f,
            .lineWidth = 0.05f,
            .hasGoalBox = true};
};

SoccerFieldInstance::Parameter getSoccerField6x4() {
    return {.fieldLength = 6.f,
            .fieldWidth = 4.f,
            .fieldBorder = 0.7f,
            .circleDiameter = 1.2f,
            .goalPostDistance = 1.4f,
            .goalDepth = 0.475f,
            .penaltySpot2Goal = 1.3f,
            .penaltyAreaWidth = 0.6f,
            .penaltyAreaHeight = 2.2f,
            .lineWidth = 0.05f,
            .hasGoalBox = false};
};

SoccerFieldInstance::Parameter getSoccerField33x18() {
    return {.fieldLength = 3.25f,
            .fieldWidth = 1.75f,
            .fieldBorder = 0.1f,
            .circleDiameter = 0.9f,
            .goalPostDistance = 0.8f,
            .goalDepth = 0.475f,
            .penaltySpot2Goal = 1.0f,
            .penaltyAreaWidth = 0.4f,
            .penaltyAreaHeight = 1.0f,
            .lineWidth = 0.05f,
            .hasGoalBox = false};
};

bool checkField(/*FlightRecorder::LogPtr log, const json& j, */const std::string& field) {
    /*
    if (!j.contains(field)) {
        //log->errMsg("No '%s' object.", field.c_str());
        return true;
    } else if (j[field] <= 0) {
        //log->errMsg("Invalid '%s' object.", field.c_str());
        return true;
    }
    */

    return false;
}

SoccerFieldInstance::Parameter getSoccerFieldFromFile(/*FlightRecorder::LogPtr log, */const std::string& path) {
    /*
    json field_dims(path);
    bool error = false;
    if (!field_dims.contains("field")) {
        log->errMsg("No 'field' object.");
        error = true;
    } else {
        error |= checkField(log, field_dims["field"], "length");
        error |= checkField(log, field_dims["field"], "width");
        error |= checkField(log, field_dims["field"], "borderStripWidth");
        error |= checkField(log, field_dims["field"], "centerCircleDiameter");
        error |= checkField(log, field_dims["field"], "penaltyCrossDistance");
        error |= checkField(log, field_dims["field"], "penaltyAreaLength");
        error |= checkField(log, field_dims["field"], "penaltyAreaWidth");
        error |= checkField(log, field_dims["field"], "lineWidth");
        if (field_dims["field"].contains("goalBoxAreaLength") != field_dims["field"].contains("goalBoxAreaWidth")) {
            log->errMsg("Either none or both of 'goalBoxAreaWidth' and 'goalBoxAreaLength' have to be defined.");
            error = true;
        } else if (field_dims["field"].contains("goalBoxAreaLength")) {
            error |= checkField(log, field_dims["field"], "goalBoxAreaLength");
            error |= checkField(log, field_dims["field"], "goalBoxAreaWidth");
        }
    }
    if (!field_dims.contains("goal")) {
        log->errMsg("No 'goal' object.");
        error = true;
    } else {
        error |= checkField(log, field_dims["goal"], "innerWidth");
        error |= checkField(log, field_dims["goal"], "postDiameter");
    }
    if (error) {
        say("Error! Field dimensions file is invalid. Please review file contents.");
        SignalHandling::requestShutdown();
        return getSoccerField9x6();
    }
    SoccerFieldInstance::Parameter params{
            .fieldLength = field_dims["field"]["length"],
            .fieldWidth = field_dims["field"]["width"],
            .fieldBorder = field_dims["field"]["borderStripWidth"],
            .circleDiameter = field_dims["field"]["centerCircleDiameter"],
            .goalPostDistance =
                    field_dims["goal"]["innerWidth"].get<float>() + field_dims["goal"]["postDiameter"].get<float>(),
            .goalDepth = 0.475f,
            .penaltySpot2Goal = field_dims["field"]["penaltyCrossDistance"],
            .penaltyAreaWidth = field_dims["field"]["penaltyAreaLength"],
            .penaltyAreaHeight = field_dims["field"]["penaltyAreaWidth"],
            .lineWidth = field_dims["field"]["lineWidth"],
    };
    if (field_dims["field"].contains("goalBoxAreaLength")) {
        params.hasGoalBox = true;
        params.goalBoxWidth = field_dims["field"]["goalBoxAreaLength"];
        params.goalBoxHeight = field_dims["field"]["goalBoxAreaWidth"];
    }

    */
    SoccerFieldInstance::Parameter params{0};
    return params;
}
}  // namespace

SoccerFieldInstance::Parameter getSoccerField(const std::string& name) {
    //auto log = FlightRecorder::FlightRecorderLog::instance("SoccerFieldDefinitions");
    if (name.empty()) {
        fs::path path("/media/usb/field_dimensions.json");
        if (fs::exists(path))
            return getSoccerFieldFromFile(/*log ,*/ path.string());
        else
            return getSoccerField9x6();
    }
    if (name == "9x6")
        return getSoccerField9x6();
    if (name == "9x6old")
        return getSoccerField9x6old();
    if (name == "75x5")
        return getSoccerField75x5();
    if (name == "6x4")
        return getSoccerField6x4();
    if (name == "33x18")
        return getSoccerField33x18();
    fs::path path(name);
    if (fs::exists(path))
        return getSoccerFieldFromFile(/*log, */path.string());
    //log->errMsg("Soccer field '%s' not found!", name.c_str());
    SignalHandling::requestShutdown();
    return getSoccerField9x6();
}

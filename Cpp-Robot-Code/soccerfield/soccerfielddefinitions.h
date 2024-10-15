#include <soccerfieldinstance.h>

#include <string>

// Can be called with any of the predefined field names, a path to a json file, or an empty string. If called with an
// empty string it will take the dimensions from the json on the usb stick or otherwise default to 9x6.
SoccerFieldInstance::Parameter getSoccerField(const std::string& name);

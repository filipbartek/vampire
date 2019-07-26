#ifndef __json__
#define __json__

#include "rapidjson/ostreamwrapper.h"
#include "rapidjson/prettywriter.h"

namespace json {

using OutputStream = rapidjson::OStreamWrapper;
using Writer = rapidjson::PrettyWriter<OutputStream>;

}

#endif // __json__

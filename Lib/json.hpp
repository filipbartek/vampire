#ifndef __json__
#define __json__

#include "rapidjson/filewritestream.h"
#include "rapidjson/prettywriter.h"

namespace json {

using FileWriteStream = rapidjson::FileWriteStream;
using Writer = rapidjson::Writer<FileWriteStream>;

}

#endif // __json__

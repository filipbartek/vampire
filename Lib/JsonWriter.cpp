#include "JsonWriter.hpp"

#include <sstream>

void Lib::JsonWriter::init(const char *filename) {
  writer.reset();
  os.reset();
  fp.reset(fopen(filename, "wb"));
  if (!fp) {
    // https://stackoverflow.com/a/5987685/4054250
    // https://stackoverflow.com/a/5138903/4054250
    std::ostringstream oss;
    oss << "JSON output error: " << strerror(errno);
    throw std::ios_base::failure(oss.str());
  }
  os.reset(new json::FileWriteStream(fp.get(), buffer, bufferSize));
  writer.reset(new json::Writer(*os));
}

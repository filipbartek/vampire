#ifndef __JsonWriter__
#define __JsonWriter__

#include <memory>

#include "Allocator.hpp"
#include "json.hpp"

namespace Lib {

class JsonWriter {
public:
  BYPASSING_ALLOCATOR;

  std::unique_ptr<json::Writer> writer;

  JsonWriter(const char *filename) : fp(nullptr, &fclose) {
    init(filename);
  }

  virtual ~JsonWriter() {}

  void init(const char *filename);

private:
  std::unique_ptr<FILE, decltype(&fclose)> fp;
  static const size_t bufferSize = 65536;
  char buffer[bufferSize];
  std::unique_ptr<json::FileWriteStream> os;
};

class JsonArrayWriter : public JsonWriter {
public:
  JsonArrayWriter(const char *filename) : JsonWriter(filename) {
    writer->StartArray();
  }

  ~JsonArrayWriter() {
    writer->EndArray();
  }
};

} // namespace Lib

#endif // __JsonWriter__

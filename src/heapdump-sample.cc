#include <nan.h>
#include <stdio.h>
#include <v8-profiler.h>

using v8::FunctionTemplate;
using v8::OutputStream;

class FileOutputStream : public OutputStream {
 public:

  virtual int GetChunkSize() {
    return 65536;  // big chunks == faster
  }

  virtual void EndOfStream() {
  }

  virtual WriteResult WriteAsciiChunk(char* data, int size) {
    return kContinue;
  }

  virtual WriteResult WriteHeapStatsChunk(v8::HeapStatsUpdate* updateData, int count) {
    for (int i = 0; i < count; ++i) {
//      printf("index: %d, count: %d, size: %d\n", updateData[i].index,updateData[i].count,updateData[i].size);
    }
    return kContinue;
  }
};

NAN_METHOD(sample) {
    FileOutputStream stream;
    info.GetIsolate()->GetHeapProfiler()->GetHeapStats(&stream);
}

NAN_MODULE_INIT(InitAll) {
  Nan::Set(target, Nan::New("sample").ToLocalChecked(),
    Nan::GetFunction(Nan::New<FunctionTemplate>(sample)).ToLocalChecked());
}

NODE_MODULE(NativeExtension, InitAll)

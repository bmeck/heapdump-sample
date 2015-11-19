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
    printf("end\n");
  }

  virtual WriteResult WriteAsciiChunk(char* data, int size) {
    return kContinue;
  }
};

static FileOutputStream stream;
NAN_METHOD(sample) {
    info.GetIsolate()->GetHeapProfiler()->GetHeapStats(&stream);
}

NAN_MODULE_INIT(InitAll) {
  Nan::Set(target, Nan::New("sample").ToLocalChecked(),
    Nan::GetFunction(Nan::New<FunctionTemplate>(sample)).ToLocalChecked());
}

NODE_MODULE(NativeExtension, InitAll)

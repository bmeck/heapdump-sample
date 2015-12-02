#include <nan.h>
#include <stdio.h>
#include <v8-profiler.h>

using v8::FunctionTemplate;
using v8::OutputStream;

class FileOutputStream : public OutputStream {
 public:
   FileOutputStream(FILE* fd) : fd_(fd) {} 

  virtual int GetChunkSize() {
    return 65536;  // big chunks == faster
  }

  virtual void EndOfStream() {
  }

  virtual WriteResult WriteAsciiChunk(char* data, int size) {
    return kContinue;
  }

  virtual WriteResult WriteHeapStatsChunk(v8::HeapStatsUpdate* updateData, int count) {
    if (fd_ != nullptr) {
      for (int i = 0; i < count; ++i) {
        if (i > 0) {
          fprintf(fd_, ",");
        }
        fprintf(fd_, "{\"index\": %d, \"count\": %d, \"size\": %d}\n", updateData[i].index,updateData[i].count,updateData[i].size);
      }
    }
    return kContinue;
  }

 private:
  FILE* fd_;
};

NAN_METHOD(sample) {
    FILE* f = nullptr;
    if (info[0]->IsUint32()) {
      int fd = info[0]->Uint32Value();
      f = fdopen(fd, "w");
      fprintf(f, "{\"samples\":[\n");
    }
    FileOutputStream stream(f);
    int last_assigned_id = info.GetIsolate()->GetHeapProfiler()->GetHeapStats(&stream);
    if (f != nullptr) {
      fprintf(f, "],\"lastAssignedId\":%d}\n", last_assigned_id);
    }
}
NAN_METHOD(startTrackingHeapObjects) {
    info.GetIsolate()->GetHeapProfiler()->StartTrackingHeapObjects(info[0]->IsTrue());
}
NAN_METHOD(stopTrackingHeapObjects) {
    info.GetIsolate()->GetHeapProfiler()->StopTrackingHeapObjects();
}

NAN_MODULE_INIT(InitAll) {
  Nan::Set(target, Nan::New("sample").ToLocalChecked(),
    Nan::GetFunction(Nan::New<FunctionTemplate>(sample)).ToLocalChecked());
  Nan::Set(target, Nan::New("startTrackingHeapObjects").ToLocalChecked(),
    Nan::GetFunction(Nan::New<FunctionTemplate>(startTrackingHeapObjects)).ToLocalChecked());
  Nan::Set(target, Nan::New("stopTrackingHeapObjects").ToLocalChecked(),
    Nan::GetFunction(Nan::New<FunctionTemplate>(stopTrackingHeapObjects)).ToLocalChecked());
}

NODE_MODULE(NativeExtension, InitAll)

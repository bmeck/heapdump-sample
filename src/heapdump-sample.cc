#include <nan.h>
#include <stdio.h>
#include <v8-profiler.h>

using v8::FunctionTemplate;
using v8::OutputStream;

void writeInt(int fd, int i) {
  int size = snprintf(nullptr, 0, "%d", i);
  char* buff = static_cast<char*>(malloc(size+1));
  buff[size] = '\0';
  snprintf(buff, size+1, "%d", i);
//  printf("\n\n%s\n\n", buff);
  write(fd, buff, size);
  free(buff);
}
void writeStr(int fd, char* str) {
  write(fd, str, strlen(str));
}

class FileOutputStream : public OutputStream {
 public:
   FileOutputStream(int fd) : fd_(fd) {} 

  virtual int GetChunkSize() {
    return 65536;  // big chunks == faster
  }

  virtual void EndOfStream() {
  }

  virtual WriteResult WriteAsciiChunk(char* data, int size) {
    return kContinue;
  }

  virtual WriteResult WriteHeapStatsChunk(v8::HeapStatsUpdate* updateData, int count) {
    if (fd_ != 0) {
      for (int i = 0; i < count; ++i) {
        if (i > 0) {
          writeStr(fd_, ",");
        }
        writeStr(fd_, "{\"index\":");
        writeInt(fd_, updateData[i].index);
        writeStr(fd_, ",\"count\":");
        writeInt(fd_, updateData[i].count);
        writeStr(fd_, ",\"size\":");
        writeInt(fd_, updateData[i].size);
        writeStr(fd_, "}\n");
      }
    }
    return kContinue;
  }

 private:
  int fd_;
};

NAN_METHOD(sample) {
  int fd = 0;
    if (info[0]->IsUint32()) {
      fd = info[0]->Uint32Value();
      writeStr(fd, "{\"samples\":[\n");
    }
    FileOutputStream stream(fd);
    int last_assigned_id = info.GetIsolate()->GetHeapProfiler()->GetHeapStats(&stream);
    if (fd != 0) {
      writeStr(fd, "],\"lastAssignedId\":");
      writeInt(fd, last_assigned_id);
      writeStr(fd, "}\n");
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

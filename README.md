# heapdump-sample
Setup a sampling interval for heap snapshots. This adds content to the "samples" field of heap snapshots and allows for determining when objects where allocated.

## usage

This must be used in conjunction with `--track-heap-objects` or the corresponding C++ call. Otherwise it will cause a segfault.

```javascript
require('heapdump-sample').sample();
```

The sample will be available in the JSON representation of a heap snapshot (available via the [heapdump](https://github.com/bnoordhuis/node-heapdump) module).

## require flag usage

Use `-r heapdump-sample/register` to get samples on an interval.

The interval defaults to 50 milliseconds, and can be changed by setting the `NODE_HEAPDUMP_SAMPLE_RATE` environment variable.
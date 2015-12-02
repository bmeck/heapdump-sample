# heapdump-sample
Setup a sampling interval for heap snapshots. This adds content to the "samples" field of heap snapshots and allows for determining when objects where allocated.

## usage

This must be used in conjunction with `--track-heap-objects` or the corresponding C++ call. Otherwise it will cause a segfault.

```javascript
require('heapdump-sample').sample();
```

### starting and stopping tracking

The module exports functions to start and stop object tracking for convenience.

```javascript
heapdump_sample.startTrackingHeapObjects(bool_should_trace_allocation_stacks);
heapdump_sample.stopTrackingHeapObjects();
```

Tracing allocation stacks can be memory intensive, it should not be used indefinitely. For production purposes you should not run tracking at all times, instead turn it on temporarily when you are debugging behavior.

## output to fd

There is the ability to get data out of the samples in real time by providing an FD to work with.

```javascript
require('heapdump-sample').sample(process.stdout.fd);
```

The sample will be available in the JSON representation of a heap snapshot (available via the [heapdump](https://github.com/bnoordhuis/node-heapdump) module).

## require flag usage

Use `-r heapdump-sample/register` to get samples on an interval.

The interval defaults to 50 milliseconds, and can be changed by setting the `NODE_HEAPDUMP_SAMPLE_RATE` environment variable.

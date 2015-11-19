# heapdump-sample
Setup a sampling interval for heap snapshots. This adds content to the "samples" field of heap snapshots and allows for determining when objects where allocated.

## require flag usage

Use `-r heapdump-sample/register` to get samples on an interval.

The interval defaults to 50 milliseconds, and can be changed by setting the `NODE_HEAPDUMP_SAMPLE_RATE` environment variable.
const hd = require("heapdump");
const hds = require("heapdump-sample");

// turn on id tracking so sampling works
hds.startTrackingHeapObjects();

// use an FD here to get it to print data out in real time (not just in snapshots)
// note: if you see the same sample index with different values, something got GC'd
setInterval(_ => hds.sample(process.stdout.fd), 100);

// make objects as quick as we can, for visible sample sizes
const a = [];
setInterval(_ => a.push({}))

setTimeout(
    _ => {
      // this will produce a snapshot with samples array filled out
      hd.writeSnapshot("my.heapsnapshot");
      process.exit();
    }, 400);


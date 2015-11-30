var interval = parseInt(process.env.NODE_HEAPDUMP_SAMPLE_RATE, 10) || 50;
setInterval(require('./').sample, interval).unref();

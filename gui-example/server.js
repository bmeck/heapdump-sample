"use strict";
const hd = require("heapdump");
const hds = require("../");//"heapdump-sample");
const http = require("http");
const net = require("net");
const websocket = require("websocket-stream");

// turn on id tracking so sampling works
hds.startTrackingHeapObjects();

const ecstatic = require('ecstatic')({ root: __dirname });

let items = [];
setInterval(function () {
  const count = Math.random() * 10000 | 0;
  const toadd = [];
  items.push(toadd);
  if (items.length > 100) items = items.slice(-100);
  for (let i = 0; i < items.length; i++) {
    let buffered = items[i];
    if (!buffered.length) continue;
    let removed = Math.ceil(Math.random() * buffered.length / 100);
    buffered.length -= removed;
  }
  gc();
  for (let i = 0; i < count; i++) {
    toadd.push({});
  }
}, 30);

var active;
const nc = net.createServer();
nc.on('connection', conn => {
  conn.pipe(active);
});
  nc.listen(0, () => {
  const server = http.createServer((req, res) => {
    if (req.url == '/sample') {
      const conn = net.connect(nc.address().port);
      conn.on('connect', (e) => {
        const connfd = conn._handle.fd;
        setInterval(_ => hds.sample(connfd), 100);
      });
    }
    else {
      ecstatic(req, res);
    }
  });
  websocket.createServer({server: server}, ws => {
    active = ws;
  });
  server.listen(process.env.PORT || 8080, ()=>{
    console.log("server is listening on port %d", server.address().port);
  });
});

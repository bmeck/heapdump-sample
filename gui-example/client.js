var websocket = require('websocket-stream');
var stream = require('jsonstream').parse();

// {max,current}
var c = document.getElementById("mycanvas");
var ctx = c.getContext('2d');
var items = [];

setInterval(render, 100);
function render() {
  ctx.clearRect(0,0,c.width,c.height);
  if (items.length == 0) return;
  var w = c.width / 100;
  var h = c.height;
  var max_size = items.reduce(function (c,s) {return Math.max(c,s.max);}, 0);
  var hscale = h / max_size;
  for (var i = 0; i < items.length; i++) {
    var sample = items[i];
    ctx.fillStyle = 'gray';
    var sample_h = sample.max * hscale;
    ctx.fillRect(
        i * w,
        h-sample_h,
        w,
        sample_h);
    ctx.fillStyle = 'blue';
    var sample_h = sample.current * hscale;
    ctx.fillRect(
        i * w,
        h-sample_h,
        w,
        sample_h);
  }
}

var highest_index = -1;
stream.on('data', function (obj) {
  obj.samples.forEach(function (sample) {
    if (sample.index > highest_index) {
      highest_index = sample.index;
      if (sample.index == 0) return;
      if (sample.size == 0) return;
      items.push({
        index:sample.index,
        max:sample.size,
        current:sample.size
      });
    }
    else {
      for (var i = 0; i < items.length; i++) {
        if (items[i].index == sample.index) {
          items[i].current = sample.size;
        }
      }
    }
    if (items.length > 100) {
      items = items.slice(-100);
    }
  });
});

var ws = websocket('ws://'+location.host);
ws.pipe(stream);
ws.on('data', function (d) {
  console.log(String(d));
})

ws.socket.onopen = function () {
  var xhr = new XMLHttpRequest();
  xhr.open('get', '/sample');
  xhr.send();
}


//  Properties
var mqtt = require('mqtt');
var client  = mqtt.connect({host:'localhost', port: 1883, username: 'super', password: 'super'});


var express = require('express');
var socket_io = require('socket.io');
var http = require('http');

var app = express();
app.use(express.static('public'));

var server = http.Server(app);
var io = socket_io(server);
var dev = {
 production: false
};


//When we connect to the MQTT broker (Mosquitto), then subscribe
// noiseLevel topic
client.on('connect', function () {
  client.subscribe('noiseLevel');
  //client.publish('test', 'Hello mqtt');
});

client.on('message', function (topic, message) {
  // message is Buffer
  //console.log(message.toString());
  //broadcast to any listening SocketIo clients
  io.emit('noiseLevel', message.toString());
  //client.end();
});

// SocketIO handle
io.on('connection', function (socket) {

    console.log('Someone connected');
    //  noise level handles

    //update
});



//  Start server
if (dev.production === true) {
  console.log('Server listening at ' + process.env.IP + ' port:'+ process.env.PORT +'...Ctrl+C to exit.');
  server.listen(process.env.PORT || process.env.IP);
} else {
  console.log('Server listening at http://localhost:8080...Ctrl+C to exit.');
  server.listen(8080);

}

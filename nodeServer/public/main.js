
//  Create main function object
var pipeDown = function(){
    // SocketIO client listener
    var socket = io();

    //get noise level label
    //var noiseLevel = $('#noiseLevel');

    //handle for listening to noise level published by LED controller
    socket.on('noiseLevel', function (level){
		//  Split string by comma to get location
		  var msg = level.split(",");
		  var pipeId = msg[0];
		  var pipeLevel = msg[1];
		
		var noiseLevel = $('#noiseLevel' + pipeId);
		
        noiseLevel.html('Noise Level: ' + pipeLevel);
        //console.log('Noise Level: ' + level);
    });


};

$(document).ready(function() {
    pipeDown();
});

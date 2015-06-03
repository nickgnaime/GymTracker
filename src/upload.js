Pebble.addEventListener('ready',
  function(e) {
    console.log('JavaScript app ready and running!');
  }
);

var player = 0;
var exercise = 0;
var month = 0;
var day = 0;
var rep = [0, 0, 0, 0, 0, 0];
var weight = [0, 0, 0, 0, 0, 0];



function send_data() {
  var req = new XMLHttpRequest();
  req.open('GET', 'http://www.kropix.de/gymtracker/gymtracker_upload.php?p='+player+'&e='+exercise+'&m='+month+'&d='+day+'&r0='+rep[0]+'&r1='+rep[1]+'&r2='+rep[2]+'&r3='+rep[3]+'&r4='+rep[4]+'&r5='+rep[5]+'&w0='+weight[0]+'&w1='+weight[1]+'&w2='+weight[2]+'&w3='+weight[3]+'&w4='+weight[4]+'&w5='+weight[5], true);
    req.onload = function(e) {
      if (req.readyState == 4 && req.status == 200) {
        if(req.status == 200) {
          console.log('Request successfull executed');
        } else { console.log('Error (req.status=' + req.status + ')'); }
      }
    };
    req.send(null);
}

Pebble.addEventListener('appmessage',
  function(e) {
    player = JSON.stringify(e.payload.K_PLAYER);
    exercise = JSON.stringify(e.payload.K_EXERCISE);
    month = JSON.stringify(e.payload.K_MONTH);
    day = JSON.stringify(e.payload.K_DAY);
    rep[0] = JSON.stringify(e.payload.K_R0);
    rep[1] = JSON.stringify(e.payload.K_R1);
    rep[2] = JSON.stringify(e.payload.K_R2);
    rep[3] = JSON.stringify(e.payload.K_R3);
    rep[4] = JSON.stringify(e.payload.K_R4);
    rep[5] = JSON.stringify(e.payload.K_R5);
    weight[0] = JSON.stringify(e.payload.K_W0);
    weight[1] = JSON.stringify(e.payload.K_W1);
    weight[2] = JSON.stringify(e.payload.K_W2);
    weight[3] = JSON.stringify(e.payload.K_W3);
    weight[4] = JSON.stringify(e.payload.K_W4);
    weight[5] = JSON.stringify(e.payload.K_W5);
    console.log('payload: ' + e.payload);
    
    console.log('String Received: ' + player + exercise + month + day + rep[0] + rep[1] + rep[2] + rep[3] + rep[4] + rep[5] + weight[0] + weight[1] + weight[2] + weight[3] + weight[4] + weight[5] );
    send_data();

    player = JSON.stringify(e.payload.K_PLAYER);
    exercise = JSON.stringify(e.payload.K_EXERCISE);
    month = JSON.stringify(e.payload.K_MONTH);
    day = JSON.stringify(e.payload.K_DAY);
    rep[0] = JSON.stringify(e.payload.K_R0);
    rep[1] = JSON.stringify(e.payload.K_R1);
    rep[2] = JSON.stringify(e.payload.K_R2);
    rep[3] = JSON.stringify(e.payload.K_R3);
    rep[4] = JSON.stringify(e.payload.K_R4);
    rep[5] = JSON.stringify(e.payload.K_R5);
    weight[0] = JSON.stringify(e.payload.K_W0);
    weight[1] = JSON.stringify(e.payload.K_W1);
    weight[2] = JSON.stringify(e.payload.K_W2);
    weight[3] = JSON.stringify(e.payload.K_W3);
    weight[4] = JSON.stringify(e.payload.K_W4);
    weight[5] = JSON.stringify(e.payload.K_W5);
    
    console.log('String Received: ' + player + exercise + month + day + rep[0] + rep[1] + rep[2] + rep[3] + rep[4] + rep[5] + weight[0] + weight[1] + weight[2] + weight[3] + weight[4] + weight[5] );
    send_data();
  }
);


/*
req.open('GET', 'http://www.kropix.de/gymtracker/gymtracker_upload.php?p='+player+'&e='+exercise+'&m='+month+'&d='+day+'&r0='+rep[0]+'&r1='+rep[1]+'&r2='+rep[2]+'&r3='+rep[3]+'&r4='+rep[4]+'&r5='+rep[5]+'&w0='+weight[0]+'&w1='+weight[1]+'&w2='+weight[2]+'&w3='+weight[3]+'&w4='+weight[4]+'&w5='+weight[5], true);
req.onload = function(e) {
  if (req.readyState == 4 && req.status == 200) {
    if(req.status == 200) {
      console.log('Request executed');
    } else { console.log('Error'); }
  }
};
req.send(null);
*/
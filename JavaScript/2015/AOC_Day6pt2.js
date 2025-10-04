var puzzData = document.getElementsByTagName('pre')[0].textContent;

// this is a test.
//var puzzData = 'turn on 0,0 through 999,999\ntoggle 0,0 through 999,0 \n'

var lights = new Array();
// Initializes the lights
for (var i = 0; i < 1000; ++i) {
  lights.push(new Array());
  for(var j = 0; j < 1000; ++j) {
    lights[i].push(0);
  }
}

function turnLights( On,  pntA,  pntB) {
  for(var i = pntA.x; i <= pntB.x; ++i) {
    for(var j = pntA.y; j <= pntB.y; ++j) {
      if (On) {
        ++lights[i][j];
      } else if(lights[i][j] !== 0) {
        --lights[i][j];
      }
    }
  }
}

function toggleLights( pntA,  pntB) {
  for(var i = pntA.x; i <= pntB.x; ++i) {
    for(var j = pntA.y; j <= pntB.y; ++j) {
      // lights[i][j] = !(lights[i][j]);
      lights[i][j] += 2;
    }
  }
}

function doWork(inputString) {
  var parsedStr = inputString.match(/[^ ]+/g);
  var pointA = new Object();
  var pointB = new Object();
  pointA = { x: 0, y: 0 };
  pointB = { x: 0, y: 0 };
  if (parsedStr[0] == 'turn') {
    pointA.x = parseInt(inputString.match(/\d+/g)[0]);
    pointA.y = parseInt(inputString.match(/\d+/g)[1]);
    pointB.x = parseInt(inputString.match(/\d+/g)[2]);
    pointB.y = parseInt(inputString.match(/\d+/g)[3]);
    switch (parsedStr[1]) {
      case "on":
        turnLights(true, pointA, pointB);
        break;
      case "off":
        turnLights(false, pointA, pointB);
        break;
      default:
        console.log('unexpected command ' + parsedStr[1]);
        console.log('inputString: ' + inputString);
        console.log(parsedStr);
    }
  } else if (parsedStr.length = 4) {
    pointA.x = parseInt(inputString.match(/\d+/g)[0]);
    pointA.y = parseInt(inputString.match(/\d+/g)[1]);
    pointB.x = parseInt(inputString.match(/\d+/g)[2]);
    pointB.y = parseInt(inputString.match(/\d+/g)[3]);
    switch (parsedStr[0]) {
      case "toggle":
        toggleLights(pointA, pointB);
        break;
      default:
        console.log('unexpected command ' + parsedStr[0]);
        console.log('inputString: ' + inputString);
        console.log(parsedStr);
    }
  }
}

var instructions = puzzData.match(/.+/gm);

//instructions.forEach(function(x) { console.log(x); });
instructions.forEach(function(x) { doWork(x); });

function howMuchBrightness() {
  var sum = 0;
  for (var i = 0; i < 1000; ++i) {
    for(var j = 0; j < 1000; ++j) {
      sum += lights[i][j];
    }
  }
  return(sum);
}
console.log('brightness : ' + howMuchBrightness());

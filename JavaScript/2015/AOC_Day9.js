var puzzData = document.getElementsByTagName('pre')[0].textContent;
//puzzData = "London to Dublin = 464\nLondon to Belfast = 518\nDublin to Belfast = 141\n"
var lines = puzzData.match(/.+/gm);


var places = new Array();

function disectPlaces(str) {
  if (typeof str == "string") {
    var words = str.match(/\w+/g);
    addDistinctPlace(words[0]);
    addDistinctPlace(words[2]);
    addAdjPair(words[0], words[2], words[3]);
  }
}

function addDistinctPlace(str) {
  if(typeof str == "string") {
    if (places.indexOf(str) === -1) {
      places.push(str);
    }
  }
}

var adjancies = new Map();
function addAdjPair(v1, v2, dist) {
  var pair = new Object();
  pair.a = v1;
  pair.b = v2;
  pair.dist = parseInt(dist);
  var pair2 = new Object();
  pair2.a = v2;
  pair2.b = v1;
  pair2.dist = parseInt(dist);
  if (adjancies.has(v1)) {
    var savedList = adjancies.get(v1);
    savedList.push(pair);
  } else {
    var adjlist = new Array();
    adjlist.push(pair);
    adjancies.set(v1, adjlist);
  }
  if (adjancies.has(v2)) {
    var savedList = adjancies.get(v2);
    savedList.push(pair2);
  } else {
    var adjlist = new Array();
    adjlist.push(pair2);
    adjancies.set(v2, adjlist);
  }
}

lines.forEach(function(x) { disectPlaces(x); });
// var vertices = new Map();
var vertices = new Array();
function createVertex(strName) {
  var v = new Object();
  v.name = strName;
  v.adjlist = adjancies.get(strName);
  v.known = false;
  v.distance = 1 << 29;
  v.path = {};
  // vertices.set(vertices.size, v);
  vertices.push(v)
}

function isUnknown (elem, index, arr) {
  return (!elem.known);
}

function isThisIt(arr, name) {
  for(var i = 0; i < arr.length; ++i) {
    if (arr[i].name == name) {
      return (i);
    }
  }
}

function findMinUnknownV(arr) {
  var min = 1 << 29;
  var minIndex = -1;
  for (var i = 0; i < arr.length; ++i) {
    if ((arr[i].distance < min) && (!arr[i].known)) {
      min = arr[i].distance;
      minIndex = i;
    }
  }
  return(minIndex);
}

places.forEach(
  function(x) {
    createVertex(x);
  }
);

function initialize() {
  vertices = new Array();
  places.forEach( function(x) { createVertex(x); } );
}

function TSP_example() {
  var results = new Array();
  var path = new Array();
  for (let i of adjancies) {
    path[0] = i[0];
    var totalDistance = 0;
    for (let j of adjancies) {
      if (path.indexOf(j[0]) == -1) {
        path[1] = j[0];
        totalDistance += getDistanceFromAdjList(path[0], path[1]);
        for (let k of adjancies) {
          if (path.indexOf(k[0]) == -1) {
            path[2] = k[0];
            totalDistance += getDistanceFromAdjList(path[1], path[2]);
            //console.log(printPathLine(path) + ' = ' + totalDistance.toString());
            var result = new Object();
            result.path = printPathLine(path);
            result.totalDistance = totalDistance;
            results.push(result);
            totalDistance -= getDistanceFromAdjList(path[1], path[2]);
          }
        }
        totalDistance -= getDistanceFromAdjList(path[0], path[1]);
        path.pop();
      }
    }
    path.pop();
  }
  return(results);
}

function TSP() {
  var results = new Array();
  var path = new Array();
  for (let i of adjancies) {
    path[0] = i[0];
    var totalDistance = 0;
    for (let j of adjancies) {
      if (path.indexOf(j[0]) == -1) {
        path[1] = j[0];
        totalDistance += getDistanceFromAdjList(path[0], path[1]);
        for (let k of adjancies) {
          if (path.indexOf(k[0]) == -1) {
            path[2] = k[0];
            totalDistance += getDistanceFromAdjList(path[1], path[2]);
            for (let l of adjancies) {
              if (path.indexOf(l[0]) == -1) {
                path[3] = l[0];
                totalDistance += getDistanceFromAdjList(path[2], path[3]);
                for (let m of adjancies) {
                  if (path.indexOf(m[0]) == -1) {
                    path[4] = m[0];
                    totalDistance += getDistanceFromAdjList(path[3], path[4]);
                    for (let n of adjancies) {
                      if (path.indexOf(n[0]) == -1) {
                        path[5] = n[0];
                        totalDistance += getDistanceFromAdjList(path[4], path[5]);
                        for (let o of adjancies) {
                          if (path.indexOf(o[0]) == -1) {
                            path[6] = o[0];
                            totalDistance += getDistanceFromAdjList(path[5], path[6]);
                            for (let p of adjancies) {
                              if (path.indexOf(p[0]) == -1) {
                                path[7] = p[0];
                                totalDistance += getDistanceFromAdjList(path[6], path[7]);
                                var result = new Object();
                                result.path = printPathLine(path);
                                result.totalDistance = totalDistance;
                                results.push(result);
                                totalDistance -= getDistanceFromAdjList(path[6], path[7]);
                              }
                            }
                            totalDistance -= getDistanceFromAdjList(path[5], path[6]);
                            path.pop();
                          }
                        }
                        totalDistance -= getDistanceFromAdjList(path[4], path[5]);
                        path.pop();
                      }
                    }
                    totalDistance -= getDistanceFromAdjList(path[3], path[4]);
                    path.pop();
                  }
                }
                totalDistance -= getDistanceFromAdjList(path[2], path[3]);
                path.pop();
              }
            }
            totalDistance -= getDistanceFromAdjList(path[1], path[2]);
            path.pop();
          }
        }
        totalDistance -= getDistanceFromAdjList(path[0], path[1]);
        path.pop();
      }
    }
    path.pop();
  }
  var min = 1000;
  var max = 0;
  results.forEach( function(x) { if(x.totalDistance < min) { min = x.totalDistance; } } );
  console.log('min is: ' + min);
  results.forEach( function(x) { if(x.totalDistance > max) { max = x.totalDistance; } } );
  console.log('max is: ' + max);
  return(results);
}

function getDistanceFromAdjList(distTo, distFrom) {
  var lst = adjancies.get(distFrom);
  for (var ii = 0; ii < lst.length; ++ii) {
    if (lst[ii].b == distTo) {
      return (lst[ii].dist);
    }
  }
  s = 0/0;
  return (-1);
}

function printPathLine(pathArr) {
  var output = "";
  pathArr.forEach(function(x) { output += x + ' to ' } );
  output = output.replace(/ to $/g, "");
  return (output);
}

function dijkstra(src) {
  src.distance = 0;
  for (;;) {
    if (vertices.some(isUnknown)) {
      console.log('findMinUnknownV: ' + findMinUnknownV(vertices).toString());
      var v = vertices[findMinUnknownV(vertices)];
      v.known = true;
      console.log(v);
      // var aList =  // adjancies.get(v.name);
      for (wpair in v.adjlist) {
        var w = vertices[isThisIt(vertices, v.adjlist[wpair].b)];
        console.log(v);
        console.log(w);
        if(!w.known) {
          var cvw = v.adjlist[wpair].dist;
          console.log(cvw);
          console.log(v.distance + cvw < w.distance);
          if(v.distance + cvw < w.distance) {
            w.distance = v.distance + cvw;
            w.path = v;
          }
        }
      }
    } else {
      break;
    }
  }
}

function printPath(v) {
  if( v.path.name != null ) {
    printPath(v.path.name);
    console.log(' to ')
  }
  console.log(v.name);
}

function dijkstra2(places, srcPlace) {
  var vertexSet = new Array();
  var distance = new Array();
  var previousNode = new Array();
  for (var i = 0; i < places.length; ++i) {
    distance[i] = Number.POSITIVE_INFINITY;
    previousNode[i] = Number.NaN;
    vertexSet.push(places[i]);
  }
  var srcPlaceIndex = distance.indexOf(srcPlace);
  distance[srcPlaceIndex] = 0;
  for(;;) {
    if (vertexSet.length != 0) {
      var u = vertexSet[srcPlaceIndex]
      vertexSet
    }
  }
}

console.log("loaded.");
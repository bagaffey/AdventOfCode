/* You need to run this on http://adventofcode.com/day/2/input
   This should grab all of the text content and put it into
   a string variable to store all the input. */
var puzzData = document.getElementsByTagName('pre')[0].textContent
//var puzzData = "2x3x4\n1x1x10\n"; // this is test data

var puzzArr = puzzData.match(/\d+x\d+x\d+\n/g);

var totalSurfaceArea = 0;
var totalRibbonFeet = 0;
var i = 0;
for (i; i < puzzArr.length; i++) {
  var present = puzzArr[i].match(/\d+/g);
  var l = parseInt(present[0]);
  var w = parseInt(present[1]);
  var h = parseInt(present[2]);
  var sidesSA = new Array(3);
  sidesSA[0] = l * w;
  sidesSA[1] = w * h;
  sidesSA[2] = h * l;
  sidesSA.s1 = sidesSA[0];
  sidesSA.s2 = sidesSA[1];
  sidesSA.s3 = sidesSA[2];
  var surfaceArea = 2 * sidesSA.s1 + 2 * sidesSA.s2  + 2 * sidesSA.s3;
  surfaceArea += sidesSA.sort(function(a,b){return a - b;})[0];
  totalSurfaceArea += surfaceArea;
  var sides = new Array(3);
  sides[0] = l;
  sides[1] = w;
  sides[2] = h;
  sides.s1 = sides[0];
  sides.s2 = sides[1];
  sides.s3 = sides[2];
  sides.sort(function(a,b){return a - b;});
  var shortPerimeter = (2 * sides[0]) + (2 * sides[1]);
  var volume = sides.s1 * sides.s2 * sides.s3;
  var ribbonFeet = volume + shortPerimeter;
  totalRibbonFeet += ribbonFeet;
  //console.log('For present ' + (i+1) + ', ribbon need: ' + ribbonFeet);
  //console.log('volume: ' + volume);
  //console.log('shortPerimeter: ' + shortPerimeter);
}

console.log('total surface area is ' + totalSurfaceArea + ' sq ft.');
console.log('total ribbon needed: ' + totalRibbonFeet);
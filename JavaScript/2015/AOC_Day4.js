
if (!js) {
  var js = document.createElement("script");
  js.type = "text/javascript";
  js.src = "https://raw.githubusercontent.com/blueimp/JavaScript-MD5/master/js/md5.js";
}

document.body.appendChild(js);

var Part1Soln = 0;
var secreyKey = "iwrupvqb";

// Part 1
for (;;) {
  ++Part1Soln;
  var result = md5(secreyKey + Part1Soln);
  if (result.startsWith("00000"))
    break;
}

console.log('the answer is ' + Part1Soln);

// Part 2
var Part2Soln = 0;
for (;;) {
  ++Part2Soln;
  var result = md5(secreyKey + Part2Soln);
  if (result.startsWith("000000"))
    break;
}

console.log('the answer is ' + Part2Soln);

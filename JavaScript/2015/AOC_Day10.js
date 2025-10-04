var puzzleInput = "1321131112";

function lookSay(input, nRounds) {
  var countChar = null
  var count = 0;
  for(var i = 0; i < nRounds; ++i) {
    var nextInput = "";
    for (var j = 0; j < input.length; ++j) {
      //console.log('countChar:' + countChar);
      //console.log('charAt():' + input.charAt(j));
      //console.log('nextInput:' + nextInput);
      if (!countChar) {
        countChar = input.charAt(j);
        ++count;
      } else if(countChar != input.charAt(j)) {
        nextInput += count.toString() + countChar;
        countChar = input.charAt(j);
        count = 1;
      } else {
        ++count;
      }
    }
    //console.log('countChar:' + countChar);
    //console.log('charAt():' + input.charAt(i));
    //console.log('nextInput:' + nextInput);
    nextInput += count.toString() + countChar;
    input = nextInput;
    nextInput = "";
    count = 0;
    countChar = null;
  }
  return(input);
}

console.log('loaded.');

var Part1Solution = lookSay(puzzleInput, 40).length;

console.log(`Part 1 solution is ${Part1Solution}`);

var Part2Solution = lookSay(puzzleInput, 50).length;

console.log(`Part 2 solution is ${Part2Solution}`);
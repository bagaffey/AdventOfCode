
var puzzData = document.getElementsByTagName('pre')[0].textContent;
/*
/^((?!ab|cd|pq|xy).)+$/gm
/^((?!ab|cd|pq|xy)\w)+$/gm // gets the same results as above. */

/* at least 3 vowels
/^[a-z]*[aeiou][a-z]*[aeiou][a-z]*[aeiou][a-z]*$/gm
*/

/* 2 consecutive letters
/^[a-z]*([a-z]){1}\1[a-z]*$/gm
/^[a-z]*([a-z])\1[a-z]*$/gm // same result */

/* Part 2 rules

Finds at least 2 occurrences of 2 letters side by side in the str.
*/
// /[a-z]*([a-z][a-z])[a-z]*\1[a-z]*/gm
/*
finds the same letter that has a single letter in between.
/^[a-z]*([a-z])[a-z]\1[a-z]*$/gm
*/

// Remove all results without a letter that appears at least once.
var puzzDataArray = puzzData.match(/^[a-z]*([a-z])\1[a-z]*$/gm);

function convertToBigStr(arr) {
  var index = 0;
  var result = "";
  for (index; index < arr.length; ++index) {
    result += arr[index].toString() + '\n';
  }
  return(result);
}

var puzzData2 = convertToBigStr(puzzDataArray);

var puzzDataArray2 = puzzData2.match(/^[a-z]*[aeiou][a-z]*[aeiou][a-z]*[aeiou][a-z]*$/gm);

var puzzData3 = convertToBigStr(puzzDataArray2);

var puzzDataArray3 = puzzData3.match(/^((?!ab|cd|pq|xy)\w)+$/gm);

console.log('result is ' + puzzDataArray3.length);

var p2puzzDataArray = puzzData.match(/[a-z]*([a-z][a-z])[a-z]*\1[a-z]*/gm);
var p2puzzData2 = convertToBigStr(p2puzzDataArray);
var p2puzzDataArray2 = p2puzzData2.match(/^[a-z]*([a-z])[a-z]\1[a-z]*$/gm);

console.log('part 2 result is ' + p2puzzDataArray2.length);


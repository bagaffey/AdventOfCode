var puzzData = document.getElementsByTagName('pre') [0].textContent;
// [A-Z]{1}[a-z]+   // people's names
var puzzInput = puzzData.match(/.+/gm);
var people = new Array();

function readData(s) {
  if (typeof s == 'string') {
    var words = s.match(/\w+/g);
    addDistinctPerson(words[0]);
    addDistinctPerson(words[10]);
    addPair(words[0], words[10], (words[2] == 'gain'), words[3]);
  }
}

function addDistinctPerson(s) {
  if (typeof s == 'string') {
    if (people.indexOf(s) == - 1) people.push(s);
  }
}

var happyPairs = new Map();
function addPair(person1, person2, isPositive, happyUnits) {
  var sign = isPositive ? 1 : - 1;
  var pair = {
    partner: person2,
    HU: happyUnits * sign
  }
  if (happyPairs.has(person1)) {
    var savedInfo = happyPairs.get(person1);
    savedInfo.push(pair);
  } else {
    var infoList = new Array();
    infoList.push(pair);
    happyPairs.set(person1, infoList);
  }
}

function addMe() {
  for(var i = 0; i < people.length; ++i) {
    addPair("Me", people[i], true, 0);
    addPair(people[i], "Me", true, 0);
  }
  addDistinctPerson("Me");
}

function calcu() {
  var results = new Array();
  var seated = new Array();
  var totalHappiness;
//  for (let i of happyPairs) {
//    seated[0] = i[0];
//    var totalHappiness = 0;
//    for (let j of happyPairs) {
//    }
//  }
//  path.pop();
  doWork(seated, totalHappiness, results);
  var min = 1000;
  var max = -1000;
  results.forEach( (x) => { if(x > max) { max = x; } } );
  console.log(`max is: ${max}`);
  return (results);
}

function doWork(seated, totalHappiness, results) {
  for (let i of happyPairs) {
    if (seated.indexOf(i[0]) == -1) {
      seated[seated.length] = i[0];
      //totalHappiness += getHappyUnits(i[1], i[0]);
      if (seated.length == people.length) {
        var totalHappiness = calculateHappiness(seated);
        // seated.toString() // seating list
        results.push(totalHappiness);
      } else {
        doWork(seated, totalHappiness, results);
      }
      //totalHappiness -= getHappyUnits(i[1], i[0]);
      seated.pop();
    }
  }
}

function calculateHappiness(ppl) {
  var result = 0;
  for (var j = 0; j < ppl.length; ++j) {
    var nextPerson = (j == (ppl.length - 1)) ? 0 : j + 1;
    var prevPerson = (j == 0) ? (ppl.length - 1) : j - 1;
    result += getHappyUnits(happyPairs.get(ppl[j]), ppl[nextPerson]);
    result += getHappyUnits(happyPairs.get(ppl[j]), ppl[prevPerson]);
  }
  return (result);
}

function getHappyUnits(lst, p) {
  for (var i0 = 0; i0 < lst.length; ++i0) {
    if (lst[i0].partner == p) {
      return (lst[i0].HU);
    }
  }
  s = 0/0;
  return('');
}

puzzInput.forEach((x) => { readData(x); });
console.log('loaded.');


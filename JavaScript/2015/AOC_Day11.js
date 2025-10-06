var puzzInput = "cqjxjnds";
puzzInput = "cqjxxyzz" // part 2

function incrementDriver(str, negIndex) {
    if (negIndex > str.length) { return (str); }
    var carryInfo = new Object();
    carryInfo.doICarry = false;
    var newChar = incrementChar(str.charAt(str.length - negIndex), carryInfo);
    str = `${str.substring(0, str.length - negIndex) + newChar + str.substring(str.length - (negIndex - 1))}`;
    if (carryInfo.doICarry) {
        str = incrementDriver(str, negIndex + 1);
    }
    return(str);
}

function incrementChar(c, carryObj) {
  carryObj.doICarry = false;
  var code = (c.charCodeAt(0) + 1);
  switch (String.fromCharCode(code)) {
    case "i":
    case "o":
    case "l":
      code = (c.charCodeAt(0) + 2);
      break;
    default:
  }
  if (code > 122) {
    code = 97;
    carryObj.doICarry = true;
  }
  return (String.fromCharCode(code));
}

function doesHaveDoubleChar(str) {
    var matches = str.match(/([a-z])\1/g);
    if (matches) {
        return (matches.length == 2);
    } else {
        return (false);
    }
}

function doesHaveStraight(str) {
    var matches = str.match(/abc|bcd|cde|def|efg|fgh|pqr|qrs|rst|stu|tuv|uvw|vwx|wxy|xyz/g);
    if (matches) {
        return (matches.length >= 1);
    } else {
            return (false);
    }
}

function init() {
  for(;;) {
        //console.log(puzzInput);
    puzzInput = incrementDriver(puzzInput, 1)
        if (doesHaveDoubleChar(puzzInput) && doesHaveStraight(puzzInput)) {
            console.log(`result is ${puzzInput}.`)
            break;
        }
  }
}

console.log('loaded.');

var abc = "abcdefghijklmnopqrstuvwxyz";
function produceString(s) {
    var result = "";
    for (var i = 0; i < s.length - 2; ++i) {
        if (result == "") {
            result += s[i] + s[i + 1] + s[i + 2];
        } else {
            result += `|${s[i] + s[i + 1] + s[i + 2]}`;
        }
    }
    return (result);
}
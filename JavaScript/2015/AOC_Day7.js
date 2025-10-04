var puzzData = document.getElementsByTagName('pre')[0].textContent;

var puzzLines = puzzData.match(/.+/gm);
var declarations = puzzData.match(/^\d+ -> [a-z]+$/gm);

var Wires = new Object();
//Wires.wires = new Array();
var listWires = new Array();
Wires.addProperty = function (propName) {
  if (!this.hasOwnProperty(propName)) {
    this[propName] = null;
  }
}
var tempVariables = puzzData.match(/[a-z]+/gm);
tempVariables.forEach(function(x) { Wires.addProperty(x) } );
function setupVar(codeString) {
  var tokens = codeString.split(/ -> /gm);
  var assigned = tokens[1];
  var expression = tokens[0];
  if (Wires[assigned]) {
    Wires[assigned] += '^' + expression;
    console.log('check this ' + assigned);
  } else {
    //Wires[assigned] = expression;
    Wires[assigned] = expression.replace(/[a-z]+/g, 'Wires.$&');
  }
}

var wOperators = puzzData.replace(/AND/g, '&').replace(/OR/g, '|').replace(/RSHIFT/g, '>>').replace(/LSHIFT/g, '<<').replace(/NOT/g, '((1 << 16) - 1) & ~');

var loc = wOperators.match(/.+/gm);
loc.forEach(function(X){setupVar(X)});

function fEval(expr) {
  if (typeof expr == "string") {
    var Vars = expr.match(/[a-z]+/g);
    if (Vars) {
      if (Vars.length > 1) {
        var results = new Array();
      } else {
        return (fEval(Wires[Vars[0]]));
      }
    } else {
      return(expr);
    }
  } else {
    return(expr)
  }
}

function fullEval(expr) {
  if (typeof expr == "string") {
    var Vars = expr.match(/[a-z]+/g);
    if (Vars) {
      var results = new Array();
      for(var i = 0; i < Vars.length; ++i) {
        Wires[Vars[i]] = fEval(Var[i]);
        results.push(parseInt(Wires[Vars[i]]));
        expr = expr.replace(Vars[i], results[i]);
      }
    }
  }
  return(eval(expr));
}

function replr(weers, weersJSON) {
  // here's an example to locate the wire B in a string. regex.
  // /( b$)|(^b )|( b )/gm
  for(signal in weers) {
    if (typeof weers[signal] === "function") continue;
    if (weers[signal].indexOf('Wires') == -1) {
      weers[signal] = ((1 << 16) - 1) & eval(weers[signal]);
      // weers[signal] = ((1 << 16) - 1) & parseInt(eval(weers[signal]));
      var searchRegExp = new RegExp("Wires\\." + signal + "([ \"])", "g");
      weersJSON = weersJSON.replace(searchRegExp, weers[signal] + '$1');
    }
  }
  return(weersJSON);
}

Wires.b = "46065";
var wiresJSON = JSON.stringify(Wires);
function driver() {
  for(;;) {
    if (Wires.a.indexOf('Wires') == -1) {
      console.log('a is : ' + Wires.a);
      break;
    } else {
      wiresJSON = replr(Wires, wiresJSON);
      Wires = JSON.parse(wiresJSON);
    }
  }
}

function tEval(propName) {
  Wires[propName] = eval('((1 << 16) - 1) & parseInt(eval(Wires.' + propName + '))');
  return(Wires[propName]);
}

console.log('done.');


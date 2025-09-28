var puzzData = document.getElementsByTagName('pre')[0].textContent;

var houses = new Array();
var distinctHouses = new Array();
var currentSantaX = 0;
var currentSantaY = 0;
var currentRoboSX = 0;
var currentRoboSY = 0;
var isSantaTurn = true;

houses.push(currentSantaX + ',' + currentSantaY);

distinctHouses.push(currentSantaX + ',' + currentSantaY);

var i = 0;
for (i; i < puzzData.length; ++i) {

  var direction = puzzData.charAt(i);

  switch (direction) {
    case 'v':
      if (isSantaTurn) 
        --currentSantaY;
      else 
        --currentRoboSY
      break;
    case '>':
      if (isSantaTurn) 
        ++currentSantaX;
      else 
        ++currentRoboSX;
      break;
    case '<':
      if (isSantaTurn) 
        --currentSantaX;
      else 
        --currentRoboSX;
      break;
    case '^':
      if (isSantaTurn) 
        ++currentSantaY;
      else 
        ++currentRoboSY;
      break;
    default:
      console.log('Unexpected char: ' + direction);
  }

  if (isSantaTurn) {
    houses.push(currentSantaX + ',' + currentSantaY);
    if (distinctHouses.indexOf(currentSantaX + ',' + currentSantaY) == -1) 
      distinctHouses.push(currentSantaX + ',' + currentSantaY);
    isSantaTurn = false; // Comment out this line Santa is not using Robo Santa.
  } else {
    houses.push(currentRoboSX + ',' + currentRoboSY);
    if (distinctHouses.indexOf(currentRoboSX + ',' + currentRoboSY) == -1) {
    distinctHouses.push(currentRoboSX + ',' + currentRoboSY);
    }
    isSantaTurn = true;
  }
}
console.log('done!');
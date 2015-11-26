headerStr <-
"<!DOCTYPE html>
<html>

<head>
<style>
h1 {
border-bottom: 3px solid #cc9900;
color: #996600;
font-size: 30px;
}

h2 {
border-bottom: 2px solid #cc9900;
color: #996600;
font-size: 20px;
}

table, th, td  {
border: 2px;
border-color: white;
border-collapse: separate;
border-radius: 4px;
border-spacing: 5px;
border-style: solid;

padding: 10px;
}

th {
color: black;
}

td {
color: white;
}
</style>

<title>
IDClass
</title>

<script src='https://code.jquery.com/jquery-2.1.4.min.js'></script>
</head>

<body>
<h1>IDClass</h1>

<h2>Relative importance of variables in classes</h2>

<div id='IDTable' style='overflow-x: auto;'></div>

<h2>Parameters (click)</h2>
<p id='IDParamK'>Class:</p>
<p id='IDParamVar'>Variable:</p>
<p id='IDParamVarType'>Type:</p>
<p id='IDParamParameters'>Parameters:</p>

<h2>Parameters (hover, not available on mobile devices)</h2>
<p id='IDParamKHover'>Class:</p>
<p id='IDParamVarHover'>Variable:</p>
<p id='IDParamVarTypeHover'>Type:</p>
<p id='IDParamParametersHover'>Parameters:</p>

<script>"

footerStr <-
"
function styleColor(val){
var h = Math.floor(val * 120. + 240.)
return 'style=\"background-color:hsl(' + h + ', 100%, 50%);\"'
}

function createTableIDData() {
var i
var out = '<table id=\"IDTable\">'

out += '<tr><th></th>'
for (j = 0; j < IDData.nbVar; ++j) {
out += '<th>' + IDData.varName[j] + '</th>'
}
out += '</tr>'

for (k = 0; k < IDData.nbClass; ++k) {
out += '<tr>'
out += '<th style=\"white-space: nowrap;\">k: ' + (k + 1) + ', pi: ' + IDData.pi[k] + '</th>'

for (j = 0; j < IDData.nbVar; ++j) {
out += '<td ' + styleColor(IDData.e[k][j]) + '>' + IDData.e[k][j] + '</td>'
}

out += '</tr>'
}

out += '</table>'
$('#IDTable').html(out)
}

createTableIDData()

var firstSel = true
var currElem

function clearCurrElement() {
currElem.css('border-color', 'white')
}

function onElemSelection(elem) {
var k = elem.parent().parent().children().index(elem.parent()) - 1
var j = elem.parent().children().index(elem) - 1

console.log('k: ' + k + ', j: ' + j)

$('#IDParamK').text('Class: ' + (k + 1))
$('#IDParamVar').text('Variable: ' + IDData.varName[j])
$('#IDParamVarType').text('Type: ' + IDData.varType[j])
$('#IDParamParameters').text('Parameters: ' + JSON.stringify(IDData.p[k][j]))

elem.css('border-color', 'black')
}

function onElemSelectionHover(elem) {
var k = elem.parent().parent().children().index(elem.parent()) - 1
var j = elem.parent().children().index(elem) - 1

console.log('k: ' + k + ', j: ' + j)

$('#IDParamKHover').text('Class: ' + (k + 1))
$('#IDParamVarHover').text('Variable: ' + IDData.varName[j])
$('#IDParamVarTypeHover').text('Type: ' + IDData.varType[j])
$('#IDParamParametersHover').text('Parameters: ' + JSON.stringify(IDData.p[k][j]))
}

$('td').hover(
function(){ // hover-in
onElemSelectionHover($(this))
}
);

$('td').click(
function(){ // hover-in
if (firstSel == false){
clearCurrElement()
}
else {
firstSel = false
}

currElem = $(this)
onElemSelection(currElem)
}
);
</script>

</body>

</html>
"
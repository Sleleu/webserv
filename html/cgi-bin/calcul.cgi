#!/bin/bash

function error_html()
{
	echo "Content-type: text/html; charset=UTF-8"
	echo ""
	echo "<html>"
	echo "	<head>"
	echo "		<title>ERROR</title>"
	echo "	</head>"
	echo "	<body>"
	echo "		<p>Error \"$1\": $2</p>"
	echo "	</body>"
	echo "<html>"
}

if [[ -z "$var1" ]]
then
	error_html "value1" "not set" 
	exit 1
fi
if [[ -z "$var2" ]]
then
	error_html "value2" "not set" 
	exit 1
fi
if [[ -z "$operand" ]]
then
	error_html "operand" "not set" 
	exit 1
fi
if [[ ! "$var1" = ?([-+])+([0-9]) ]]
then
	error_html "value1" "not accepted" 
	exit 2
fi
if [[ ! "$var2" = ?([-+])+([0-9]) ]]
then
	error_html "value2" "not accepted" 
	exit 2
fi
if [[ ! "$operand" = ?([-+/*%]) ]] || [[ ${#operand} > 1 ]]
then
	error_html "operand" "not accepted" 
	exit 2
fi

result=$(("$var1" "$operand" "$var2"))

echo "Content-type: text/html; charset=UTF-8"
echo ""
echo "<html>"
echo "	<head>"
echo "		<title>Sucess</title>"
echo "	</head>"
echo "	<body>"
echo "		<p>Le resultat du calcul est $result</p>"
echo "	</body>"
echo "<html>"

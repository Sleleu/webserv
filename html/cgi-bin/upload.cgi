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
	echo "		<p>Error while uploading file \"$1\": $2</p>"
	echo "	</body>"
	echo "<html>"
}

if [[ "$UPLOAD" != "on" ]]
then
	error_html "$1" "upload isn't allowed"
	exit 4
fi
last_char=${UPLOAD_PATH: -1}
if [[ -z "$UPLOAD_PATH" ]]
then
	error_html "$1" "UPLOAD_PATH not specified"
	exit 2
fi
if [[ -z "$1" ]]
then
	error_html "$1" "is a empty string"
	exit 2
fi
if [[ ! "$last_char" == "/" ]]
then
	UPLOAD_PATH="$UPLOAD_PATH/"
fi
path=$UPLOAD_PATH$1

if [[ ! -e "$UPLOAD_PATH" ]]
then
	error_html "$1" "UPLOAD_PATH \"$UPLOAD_PATH\" does not exist"
	exit 6
fi
if [[ -d "$path" ]]
then
	error_html "$1" "already exist and is a directory"
	exit 7
fi
if [[ -e "$path" ]]
then
	error_html "$1" "already exist and is a file"
	exit 1
fi

echo "$CONTENT" > $path

echo "Content-type: text/html; charset=UTF-8"
echo ""
echo "<html>"
echo "	<head>"
echo "		<title>Sucess</title>"
echo "	</head>"
echo "	<body>"
echo "		<p>File File:\"$1\" well uploaded !</p>"
echo "	</body>"
echo "<html>"

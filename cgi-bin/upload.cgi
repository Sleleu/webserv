!#/bin/bash

function error_html()
{
	echo "<html>"
	echo "	<head>"
	echo "		<title>ERROR</title>"
	echo "	</head>"
	echo "	<body>"
	echo "		<p>Error while uploading file "$1": $2</p>
	echo "	</body>"
	echo "<html>"
}

#potentiel problem selon si path est ecrit avec ou san /

path=$UPLOAD_PATH$1

if [ $UPLOAD != "ON" ]
then
	error_html $1 "upload isn't allowed" 
	return 4
elif [ !$UPLOAD_PATH -e ]
then
	error_html $1 'UPLOAD_FILE "$UPLOAD_PATH" doesn't exist' 
	return 6
elif [ !$UPLOAD_PATH -d ]
then
	error_html $1 'UPLOAD_FILE "$UPLOAD_PATH" isn't a directory' 
	return 5
elif [ "$path" -d ]
then
	error_html $1 "already exist and is a directory" 
	return 1
elif [ -z $1 ]
then
	error_html $1 "is a empty string"
	return 2
elif [ "$path" -e ]
then
	error_html $1 "already exist"
	return 3
else
	echo "$CONTENT" > $PATH_INFO

echo "<html>"
echo "	<head>"
echo "		<title>ERROR</title>"
echo "	</head>"
echo "	<body>"
echo "		<p>File "$1"</p>
echo "	</body>"
echo "<html>"

#!/bin/bash

echo "<html>"
echo "<head>"
echo "<title>Environment Variables</title>"
echo "</head>"
echo "<body>"
echo "<h1>Environment Variables</h1>"
echo "<table>"
env | sort | while read line; do
    echo "<tr><td>${line%=*}</td><td>${line#*=}</td></tr>"
done
echo "</table>"
echo "</body>"
echo "</html>"
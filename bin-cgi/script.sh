#!/bin/sh

if [[ $REQUEST_METHOD != "POST" ]]; then
	echo "No POST request" ; exit
fi

if [[ $CONTENT_TYPE != "application/x-www-form-urlencoded" ]]; then
	echo "Wrong encoded type" ; exit
else
	echo "Content-Type: text/html"
	echo
fi

echo "<html> <head> <title> CGI script </title> </head> <body>"

while read line
do
  echo "$line" >> file
done < "${1:-/dev/stdin}"

pairs=('cat "file"')

for i in "${pairs[@]}"; do
	echo $i
done
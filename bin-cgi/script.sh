#!/bin/bash

if [[ $REQUEST_METHOD != "POST" ]]; then
	echo "No POST request" ; exit
fi

if [[ $CONTENT_TYPE != "application/x-www-form-urlencoded" ]]; then
	echo "Wrong encoded type" ; exit
fi

# recupFile empty and fill it line by line from stdin
echo "" > recupFile

while read line; do
  echo "$line" >> recupFile
done < "${1:-/dev/stdin}"

# lines from stdin wrote into recupFile copy into string
str=$(cat 'recupFile')

# separer les 'espaces' definis par '&'
readarray -d '&' -t pairs < <(printf '%s' "$str")

# on declare 'data' etre un dictionaire
declare -A data

# separer les paires de valeurs grace a '=' (key=value)
for i in "${pairs[@]}"; do
	readarray -d '=' -t result < <(printf '%s' "$i")
	data[${result[0]}]="${result[1]}"
done

echo "" > fileCGI

echo "HTTP/1.1 200 OK" >> fileCGI
echo "Content-Type: text/html" >> fileCGI
echo "" >> fileCGI

echo "<html> <head> <title> CGI script </title> </head> <body>" >> fileCGI

echo "<h1>${data[@]}</h1>" >> fileCGI

echo "</body> </html>" >> fileCGI
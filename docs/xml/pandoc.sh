#!/bin/sh

pandoc="$1"
input="$2"
output="$3"

set -e

pandoc --wrap=none -t docbook -o "$output" "$input"
sed -i \
  -e 's/<programlisting/<informalexample>\0/g' \
  -e 's|/programlisting>|\0</informalexample>|g' \
  -e 's|<link xlink:href="\(.*\)">\([^<]*\)</link>|<ulink role="online-location" url="\1">\2</ulink>|' \
  "$output"

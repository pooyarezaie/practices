#!/usr/bin/env bash

rm src/main/java/Antlr/*.java
rm src/main/java/Antlr/*.tokens
java -jar /usr/local/lib/antlr-4.5.3-complete.jar src/main/java/Antlr/X.g4


for filename in src/main/java/Antlr/*.java; do
    echo 'package Antlr;' | cat - "$filename" > temp && mv temp "$filename"
done


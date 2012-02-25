#!/bin/bash

for file in *.h *.cpp; do
	sed -ie 's/Filename:.*/Filename:  '$file'/' $file;
done

rm *.he *.cppe

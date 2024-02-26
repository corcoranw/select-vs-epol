#!/bin/bash

server=$1

./$server > output.txt;
cat output.txt | wc -l

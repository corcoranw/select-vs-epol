#!/bin/bash

server=$1

ss -ntp4 state established | grep ./$server

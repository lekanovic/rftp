#!/usr/bin/bash

./connect_test.py $1
./speed_test.py $1
./put-get-test.py $1


#!/usr/bin/bash

EXPECTED_ARGS=2

ip=$1
port=$2

if [ $# -ne $EXPECTED_ARGS ]
then
	echo "STOP!! No port specified!!"
	exit 1
fi

for i in `seq 1 4`
do
./connect_test.py $ip $port
./speed_test.py $ip $port
./put-get-test.py $ip $port
./download_test.py $ip $port "1_dir"
done

echo "All md5sum should be the same.."

echo $(md5sum "1_dir/big.bin")


echo "Clean up all.."
rm -rf *_dir

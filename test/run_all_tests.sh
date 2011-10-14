#!/usr/bin/bash

EXPECTED_ARGS=1

if [ $# -ne $EXPECTED_ARGS ]
then
	echo "STOP!! No port specified!!"
	exit 1
fi

for i in `seq 1 4`
do
./connect_test.py $1
./speed_test.py $1
./put-get-test.py $1
done

./download_test.py 1 #Create and upload big file

for i in `seq 1 10`
do
	./download_test.py $1 $i"_dir" &
	sleep 1
done

echo "sleeping wait for the job to finish..."
sleep 50

./download_test.py 2 #Delete remote file

echo "All md5sum should be the same.."
for i in `seq 1 10`
do
	echo $(md5sum $i"_dir/big.bin")
done

echo "Clean up all.."
rm -rf *_dir

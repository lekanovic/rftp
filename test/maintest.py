#!/usr/bin/env python
# -*- coding: utf-8 -*-

from connectest import ConnectTest
from speedtest import SpeedTest
import sys
import time


def main(ip, port):
    test = ConnectTest(ip, port)
    test.createFile()
    test.connect()
    test.storCmd()
    test.retrCmd()
    test.checkHash()
    test.deleCmd()
    test.disconnect()

    test = SpeedTest(ip, port)
    test.createFile()
    test.connect()

    filesize = test.fileSize()
    time_start = time.time()
    test.storCmd()
    delta = (time.time() - time_start)
    data = "time to upload file %f sec, speed %d MB/s" % ((delta, filesize / delta))
    print(data)

    time_start = time.time()
    test.retrCmd()
    delta = (time.time() - time_start)
    data = "time to upload file %f sec, speed %d MB/s" % ((delta, filesize / delta))
    print(data)

    test.deleCmd()
    test.disconnect()

if __name__ == "__main__":
    if len(sys.argv) == 3:
        ip = sys.argv[1]
        port = sys.argv[2]
    else:
        ip = "0.0.0.0"
        port = 21
    main(ip, port)
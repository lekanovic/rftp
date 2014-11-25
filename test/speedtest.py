#!/usr/bin/env python
# -*- coding: utf-8 -*-

from connectest import ConnectTest
import os


class SpeedTest(ConnectTest):

    def __init__(self, ip, port):
        super(SpeedTest, self).__init__(ip, port)

    def fileSize(self):
        return os.path.getsize(self.filename)

'''
print("*** START SPEED TEST ***")

if len(sys.argv) == 3:
    ip = sys.argv[1]
    port = sys.argv[2]
else:
    ip = "0.0.0.0"
    port = 21

s = SpeedTest(ip, port)
s.createFile()
s.connect()

filesize = s.fileSize()
time_start = time.time()
s.storCmd()
delta = (time.time() - time_start)
data = "time to upload file %f sec, speed %d MB/s" % ((delta, filesize / delta))
print(data)

time_start = time.time()
s.retrCmd()
delta = (time.time() - time_start)
data = "time to upload file %f sec, speed %d MB/s" % ((delta, filesize / delta))
print(data)

s.deleCmd()
s.disconnect()
'''

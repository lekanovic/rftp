#!/usr/bin/env python
# -*- coding: utf-8 -*-

from basetest import BaseTest
import subprocess
import md5
import sys


class ConnectTest(BaseTest):

    def __init__(self, ip, port):
        super(ConnectTest, self).__init__(ip, port)
        self.hashvalue = 0
        self.filename = "big.bin"

    def createFile(self):
        print(("Creating %s file" % self.filename))
        cmd = "dd if=/dev/urandom of=%s bs=1M count=100" % self.filename
        subprocess.call(cmd, shell=True)
        print("Done..")

        hash_obj = md5.new()
        hash_obj.update(open(self.filename).read())

        self.hashvalue = hash_obj.hexdigest()
        print((self.hashvalue))

    def checkHash(self):
        hash_obj = md5.new()
        hash_obj.update(open(self.filename).read())
        new_value = hash_obj.hexdigest()

        if self.hashvalue != new_value:
            print(("hash_value %s" % self.hashvalue))
            print(("new_value %s" % new_value))
            self.print_result(False)
        else:
            self.print_result(True)

    def storeCmd(self):
        try:
            f = open(self.filename, 'rb')
            self.ftp.storbinary('STOR ' + self.filename, f)
            self.print_result(True)
        except Exception as e:
            print(e)
            self.print_result(False)
        cmd = "rm %s" % self.filename
        subprocess.call(cmd, shell=True)

    def retrCmd(self):
        try:
            self.ftp.retrbinary('RETR ' + self.filename,
                open(self.filename, 'wb').write)
            self.print_result(True)
        except Exception as e:
            print(e)
            self.print_result(False)

    def deleCmd(self):
        compare = "250 " + self.filename + " has been deleted"
        result = self.ftp.sendcmd('DELE ' + self.filename)

        if result != compare:
            self.print_result(False)
        else:
            self.print_result(True)

print("*** START CONNECT TEST ***")

if len(sys.argv) == 3:
    ip = sys.argv[1]
    port = sys.argv[2]
else:
    ip = "127.0.0.1"
    port = 7000

c = ConnectTest(ip, port)
c.createFile()
c.connect()
c.storeCmd()
c.retrCmd()
c.checkHash()
c.deleCmd()
c.disconnect()

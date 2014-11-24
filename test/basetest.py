# -*- coding: utf-8 -*-

import ftplib


class BaseTest(object):

    def __init__(self,
                ip="127.0.0.1",
                port=7000,
                user="tester",
                passwd="celeron"):
        self.ip = ip
        self.port = port
        self.ftp = ftplib.FTP()
        self.username = user
        self.passwd = passwd

    def connect(self):
        self.ftp.connect(self.ip, self.port)
        self.ftp.login(self.username, self.passwd)
        self.ftp.sendcmd('TYPE I')

    def disconnect(self):
        self.ftp.sendcmd('QUIT')
        self.ftp.close()

    def print_result(self, passed=False):
        if passed:
            print("\033[01;34mTEST PASSED\033[0m")
        else:
            print("\033[01;31mTEST FAILED\033[0m")


test = BaseTest("0.0.0.0", 21)
test.connect()
test.disconnect()
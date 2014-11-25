#!/usr/bin/env python
# -*- coding: utf-8 -*-

from basetest import BaseTest


class PutgetTest(BaseTest):

    def __init__(self, ip, port):
        super(PutgetTest, self).__init__(ip, port)
        self.list = []
        self.list.append(("MKD test", "257 test directory created"))
        self.list.append(("CWD test", "200 Working directory changed"))
        self.list.append(("CWD ..", "200 Working directory changed"))
        self.list.append(("RMD test",
        "250 Requested file action okay, completed"))

    def listTest(self):
        self.ftp.retrlines("LIST")

    def pwdTest(self):
        if self.ftp.sendcmd("PWD").find("257 "):
            self.print_result(False)
        else:
            self.print_result(True)

    def cmdTest(self):
        for item in self.list:
            if self.ftp.sendcmd(item[0]) != item[1]:
                self.print_result(False)
            else:
                self.print_result(True)

    def retrTest(self):
        filename = "testfile.txt"
        try:
            self.ftp.retrbinary("RETR " + filename,
            open(filename, "wb").write)
            self.print_result(True)
        except Exception as e:
            print(e)
            self.print_result(False)

    def storTest(self):
        filename = "testfile.txt"
        try:
            f = open(filename, "rb")
            self.ftp.storbinary("STOR " + filename, f)
            self.print_result(True)
        except Exception as e:
            print(e)
            self.print_result(False)

    def mkdTest(self):
        cmd = "MKD radde"
        result = "257 radde directory created"
        if self.ftp.sendcmd(cmd) != result:
            self.print_result(False)
        else:
            self.print_result(True)

    def rmdTest(self):
        cmd = "RMD radde"
        result = "250 Requested file action okay, completed"
        if self.ftp.sendcmd(cmd) != result:
            self.print_result(False)
        else:
            self.print_result(True)

    def asciiTest(self):
        cmd = "TYPE A"
        result = "200 Opening ASCII mode data connection"
        if self.ftp.sendcmd(cmd) != result:
            self.print_result(False)
        else:
            self.print_result(True)

    def binarymodeTest(self):
        cmd = "TYPE I"
        result = "200 Opening binary mode data connection"
        if self.ftp.sendcmd(cmd) != result:
            self.print_result(False)
        else:
            self.print_result(True)

    def deleTest(self):
        f = "testfile.txt"
        cmd = "DELE " + f
        result = "250 %s has been deleted" % (f)

        if self.ftp.sendcmd(cmd) != result:
            self.print_result(False)
        else:
            self.print_result(True)

'''
p = PutgetTest("0.0.0.0", 21)
p.connect()
p.listTest()
p.pwdTest()
p.cmdTest()
p.storTest()
p.retrTest()
p.mkdTest()
p.rmdTest()
p.asciiTest()
p.binarymodeTest()
p.deleTest()

p.disconnect()
'''
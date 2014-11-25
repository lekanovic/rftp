#!/usr/bin/env python
# -*- coding: utf-8 -*-

from basetest import BaseTest
import ftplib


class RenamefolderTest(BaseTest):

    def __init__(self, ip, port):
        super(RenamefolderTest, self).__init__(ip, port)

    def mkdTest(self):
        cmd = "MKD TestingFolder"
        result = "257 TestingFolder directory created"
        if self.ftp.sendcmd(cmd) != result:
            self.print_result(False)
        else:
            self.print_result(True)

    def renameTest(self):
        cmd = "RNFR TestingFolder"
        result = "350 Requested file action pending further information"
        if self.ftp.sendcmd(cmd) != result:
            self.print_result(False)
        else:
            self.print_result(True)

        cmd = "RNTO NewName"
        result = "250 Requested file action okay, completed"
        if self.ftp.sendcmd(cmd) != result:
            self.print_result(False)
        else:
            self.print_result(True)

    def cleanfolderTest(self):
        cmd = "RMD NewName"
        result = "250 Requested file action okay, completed"
        if self.ftp.sendcmd(cmd) != result:
            self.print_result(False)
        else:
            self.print_result(True)
        cmd = "CWD NewName"
        try:
            self.ftp.sendcmd(cmd) != "550 This is not an directory"
        except ftplib.error_perm:
            self.print_result(True)
            return
        self.print_result(False)

'''
r = RenamefolderTest("0.0.0.0", 21)
r.connect()
r.mkdTest()
r.renameTest()
r.cleanfolderTest()

r.disconnect()
'''
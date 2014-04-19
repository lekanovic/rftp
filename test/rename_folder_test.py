#!/usr/bin/env python
import ftplib
import sys

#http://docs.python.org/library/ftplib.html

ftp = ftplib.FTP()
ip='127.0.1.1'
port=7000

def print_test_failed():
	print "\033[01;31mTEST FAILED\033[0m"

def print_test_passed():
	print "\033[01;34mTEST PASSED\033[0m"


def connect_to_ftp():
	ftp.connect(ip,port)     #specify port number when connection
	ftp.login('tester','celeron')
	ftp.sendcmd('TYPE I')

def mkd_cmd_test():
	if ftp.sendcmd('MKD TestingFolder') != "257 TestingFolder directory created":
		print_test_failed()
	else:
		print_test_passed()

def rename_cmd_test():
	if ftp.sendcmd('RNFR TestingFolder') != "350 Requested file action pending further information":
		print_test_failed()
	else:
		print_test_passed()

	if ftp.sendcmd('RNTO NewName') != "250 Requested file action okay, completed":
		print_test_failed()
	else:
		print_test_passed()

def clean_folders():
	if ftp.sendcmd('RMD NewName') != "250 Requested file action okay, completed":
		print_test_failed()
	else:
		print_test_passed()

	try:
		ftp.sendcmd('CWD NewName') != "550 This is not an directory"
	except ftplib.error_perm:
		print_test_passed()
		return
	print_test_failed()

def main():
	print "*** START RENAME FOLDER TEST ***"
	connect_to_ftp()
	mkd_cmd_test()
	rename_cmd_test()
	clean_folders()

if __name__ == "__main__":
	if len(sys.argv) == 3:
		ip = sys.argv[1]
		port = sys.argv[2]
	else:
		port = 7000
	main()


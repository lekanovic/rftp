#!/usr/bin/env python
import ftplib
import sys
import time

#http://docs.python.org/library/ftplib.html

ftp = ftplib.FTP()
port = 7000

def print_test_failed():
	print "\033[01;31mTEST FAILED\033[0m"

def print_test_passed():
	print "\033[01;34mTEST PASSED\033[0m"

def connect_to_ftp():
	ftp.connect('localhost',port)     #specify port number when connection
	ftp.login('radovan','XXX')
	ftp.sendcmd('TYPE I')

def stor_cmd_test():
	filename = 'big.bin'
	try:
		f = open(filename,'rb')
		ftp.storbinary('STOR ' + filename, f)
		print_test_passed()
	except Exception,e:
		print e
		print_test_failed()

def retr_cmd_test():
	filename = 'big.bin'
	try:
		ftp.retrbinary('RETR ' + filename, open(filename, 'wb').write)
		print_test_passed()
	except Exception,e:
		print e
		print_test_failed()

def dele_cmd_test():
	f='big.bin'
	if ftp.sendcmd('DELE ' + f) != "250 " + f + " has been deleted":
		print_test_failed()
	else:
		print_test_passed() 

def close_connection():
	ftp.sendcmd('QUIT')
	ftp.close()

def main():
	while True:
		print "*** START NEW TEST ROUND ***"
		connect_to_ftp()
		stor_cmd_test()
		retr_cmd_test()
		dele_cmd_test()
		close_connection()
		time.sleep(2)

if __name__ == "__main__":
	if len(sys.argv) == 2:
		port = sys.argv[1]
	else:
		port = 7000
	main()





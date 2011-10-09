#!/usr/bin/env python
import ftplib
import sys

#http://docs.python.org/library/ftplib.html

ftp = ftplib.FTP()

def print_test_failed():
	print "\033[01;31mTEST FAILED\033[0m"

def print_test_passed():
	print "\033[01;34mTEST PASSED\033[0m"

def connect_to_ftp():
	ftp.connect('localhost',7000)     #specify port number when connection
	ftp.login('radovan','XXX')
	ftp.set_pasv(False)

def close_connection():
	ftp.close()

def list_cmd_test():
	ftp.retrlines('LIST')

def pwd_cmd_test():
	if ftp.sendcmd('PWD') != "226 Transfer complete":
		print_test_failed()
	else:
		print_test_passed()

def cwd_cmd_test():
	if ftp.sendcmd('CWD test') != "200 Working directory changed":
		print_test_failed()
	else:
		print_test_passed()

	if ftp.sendcmd('CWD ..') != "200 Working directory changed":
		print_test_failed()
	else:
		print_test_passed()

def retr_cmd_test(filename):
	# fetch a text file
	outfile = sys.stdout
	# use a lambda to add newlines to the lines read from the server
	try:
		ftp.retrlines("RETR " + filename, lambda s, w=outfile.write: w(s+"\n"))
	except :
		print_test_failed()
def stor_cmd_test(filename):
	#print ftp.retrlines('STOR testfile.txt')
	ftp.storlines("STOR " + filename, open(filename))

def mkd_cmd_test():
	if ftp.sendcmd('MKD radde') != "257 radde directory created":
		print_test_failed()
	else:
		print_test_passed()

def rmd_cmd_test():
	if ftp.sendcmd('RMD radde') != "250 Requested file action okay, completed":
		print_test_failed()
	else:
		print_test_passed()
def main(argv):
	connect_to_ftp()

	list_cmd_test()
	pwd_cmd_test()
	cwd_cmd_test()
	#stor_cmd_test("testfile.txt")
	retr_cmd_test("testfile.txt")
	mkd_cmd_test()
	rmd_cmd_test()
	close_connection()

if __name__ == "__main__":
	main(sys.argv[1:])






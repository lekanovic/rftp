#!/usr/bin/env python
import ftplib
import sys
import time
import subprocess
import md5

#http://docs.python.org/library/ftplib.html

ftp = ftplib.FTP()
port = 7000
hash_value=0

def print_test_failed():
	print "\033[01;31mTEST FAILED\033[0m"

def print_test_passed():
	print "\033[01;34mTEST PASSED\033[0m"

def connect_to_ftp():
	ftp.connect('localhost',port)     #specify port number when connection
	ftp.login('radovan','XXX')
	ftp.sendcmd('TYPE I')

def close_connection():
	ftp.sendcmd('QUIT')
	ftp.close()

def retr_cmd_test(p):
	filename = 'big.bin'
	print p
	subprocess.call('mkdir ' + p, shell=True)
	try:
		ftp.retrbinary('RETR ' + filename, open(p + '/' + filename, 'wb').write)
		print_test_passed()
	except Exception,e:
		print e
		print_test_failed()
def create_big_file():
	global hash_value
	print "Creating big.bin file"
	subprocess.call("dd if=/dev/urandom of=big.bin bs=1M count=500", shell=True)
	print "Done.."

def stor_cmd_test():
	filename = 'big.bin'
	try:
		f = open(filename,'rb')
		ftp.storbinary('STOR ' + filename, f)
		print_test_passed()
	except Exception,e:
		print e
		print_test_failed()
	subprocess.call("rm big.bin", shell=True) #remove local file

def dele_cmd_test():
	f='big.bin'
	if ftp.sendcmd('DELE ' + f) != "250 " + f + " has been deleted":
		print_test_failed()
	else:
		print_test_passed()

def main(p):
	print "*** Download TEST started ***"
	connect_to_ftp()
	retr_cmd_test(p)
	close_connection()
	print "*** Download TEST done ***"

if __name__ == "__main__":
	if len(sys.argv) == 3:
		port = sys.argv[1]
		dir_path = sys.argv[2]
	elif len(sys.argv) == 2:
		if sys.argv[1] == '1':
			connect_to_ftp()
			create_big_file()
			stor_cmd_test()
			close_connection()
		elif sys.argv[1] == '2':
			connect_to_ftp()
			dele_cmd_test()
			close_connection()
		exit(1)
	else:
		port = 7000
	main(dir_path)

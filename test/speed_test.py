#!/usr/bin/env python
import ftplib
import sys
import time
import subprocess
import md5

ftp = ftplib.FTP()
port=7000


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

def create_big_file():
	global hash_value
	print "Creating big.bin file"
	subprocess.call("dd if=/dev/urandom of=big.bin bs=1M count=500", shell=True)
	print "Done.."

	hash = md5.new()
	hash.update(open('big.bin').read())

	hash_value = hash.hexdigest()
	print hash_value

def delete_big_file():
	subprocess.call("rm big.bin", shell=True) #remove local file

def main():
	create_big_file()
	connect_to_ftp()
	print "*** START SPEED TEST ***"
	start = time.time()
	stor_cmd_test()
	print 'time to upload file %f sec' % ((time.time() - start))

	start = time.time()
	retr_cmd_test()
	print 'time to dwnld file %f sec' % ((time.time() - start))

	dele_cmd_test()
	delete_big_file()

	close_connection()

if __name__ == "__main__":
	if len(sys.argv) == 2:
		port = sys.argv[1]
	else:
		port = 7000
	main()




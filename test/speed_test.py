#!/usr/bin/env python
import ftplib
import sys
import time
import subprocess
import md5
import os

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

def get_file_size(filename):
	return os.path.getsize('big.bin')

def main():
	create_big_file()
	file_size = get_file_size('big.bin')/1000000
	connect_to_ftp()
	print "*** START SPEED TEST ***"
	start = time.time()
	stor_cmd_test()
	delta = (time.time() - start)
	print 'time to upload file %f sec, sped %d MB/s' % (delta,file_size/delta)

	start = time.time()
	retr_cmd_test()
	delta = (time.time() - start)
	print 'time to dwnld file %f sec, speed %d MB/s' % (delta,file_size/delta)


	dele_cmd_test()
	delete_big_file()

	close_connection()

if __name__ == "__main__":
	if len(sys.argv) == 3:
		ip = sys.argv[1]
		port = sys.argv[2]
	else:
		ip = "10.97.0.112"
		port = 21
	main()




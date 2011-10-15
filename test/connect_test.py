#!/usr/bin/env python
import ftplib
import sys
import time
import subprocess
import md5

#http://docs.python.org/library/ftplib.html

ftp = ftplib.FTP()
ip='127.0.1.1'
port = 7000
hash_value=0

def print_test_failed():
	print "\033[01;31mTEST FAILED\033[0m"

def print_test_passed():
	print "\033[01;34mTEST PASSED\033[0m"

def connect_to_ftp():
	ftp.connect(ip,port)     #specify port number when connection
	ftp.login('tester','celeron')
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
	subprocess.call("rm big.bin", shell=True) #remove local file

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

def create_big_file():
	global hash_value
	print "Creating big.bin file"
	subprocess.call("dd if=/dev/urandom of=big.bin bs=1M count=100", shell=True)
	print "Done.."

	hash = md5.new()
	hash.update(open('big.bin').read())

	hash_value = hash.hexdigest()
	print hash_value

def check_hash():
	global hash_value
	hash = md5.new()
	hash.update(open('big.bin').read())
	new_value = hash.hexdigest()

	if hash_value != new_value:
		print "hash_value %s" % hash_value
		print "new_value %s" % new_value
		print_test_failed()
	else:
		print_test_passed()

def main():
	create_big_file()

	for i in range(0,5):
		print "*** START NEW TEST ROUND ***"
		connect_to_ftp()
		stor_cmd_test()
		retr_cmd_test()
		check_hash()
		dele_cmd_test()
		close_connection()
		time.sleep(2)

if __name__ == "__main__":
	if len(sys.argv) == 2:
		port = sys.argv[1]
	else:
		port = 7000
	main()





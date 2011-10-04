#!/usr/bin/python

import socket
import time
import sys
import thread
#152 = 0x98
#76 =  0x4C
# 0x984c = 38988

def setup_server(a,b):
	s = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
	s.bind(("127.0.0.1",38988))
	s.listen(1)
	conn, addr = s.accept()
	print 'Connected by\n',addr
	while 1:
		data = conn.recv(1024)
		if not data: break
		print data

def main(argv):
	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	s.connect(('localhost', 7000))

	data = s.recv(1024)

	s.send("USER radovan\r\n");
	data = s.recv(1024)

	if data != "331 Anonymous access allowed\r\n":
		print "Test failed"
		exit(1)

	s.send("PASS xxx\r\n")
	data = s.recv(1024)

	if data != "230 Welcome to server\r\n":
		print "Test failed"

	thread.start_new_thread(setup_server,(0,0))
	time.sleep(3)

	s.send("PORT 127,0,0,1,152,76\r\n")

	data = s.recv(1024)
	print data

	if data != "200 PORT command successful\r\n":
		print "Test failed"
		exit(1)

	s.send("LIST\r\n")
	data = s.recv(1024)
	print data
	while 1:
		data = s.recv(1024)
		if data == "226 Transfer complete\r\n":
			print data
			break
		time.sleep(1)
	s.send("QUIT\r\n")
	data = s.recv(1024)
	if data != "221 Goodbye\r\n":
		print "Test failed"
		exit(1)

	print "TEST PASSED!!!"

if __name__ == "__main__":
	main(sys.argv[1:])





#!/usr/bin/python

import socket
import time
import sys

def main(argv):
	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	s.connect(('localhost', 7000))

	data = s.recv(1024)
	if data == "**Hi dude**":
		print "Server answer"
	print data

	while True:
		s.send(argv[0])
		data = s.recv(1024)
		if data != argv[0]:
			print "!! inte lika"
		time.sleep(1)

if __name__ == "__main__":
	main(sys.argv[1:])

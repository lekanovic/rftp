#!/usr/bin/env python
import ftplib
import sys

def main(argv):
	ftp = ftplib.FTP()
	ftp.connect('localhost',7000)     #specify port number when connection
	ftp.login('radovan','XXX')
	ftp.retrlines('LIST')

if __name__ == "__main__":
	main(sys.argv[1:])

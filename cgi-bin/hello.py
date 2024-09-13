#! /usr/bin/python3

import cgitb
import os
import cgi

#cgitb.enable()

form = cgi.FieldStorage()
name = form.getvalue('name', 'world')

print("<!DOCTYPE html>")
print("<html>")

print("\t<head>")
print('\t<meta charset="utf-8">')
print("\t<title>Hello</title>")
print("\t</head>")

print("\t<body>")
print(f"\t\t<h2>Hello, {name}!</h2>")
print("\t</body>")

print("</html>")
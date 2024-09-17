#! /usr/bin/python3

import cgitb
import os
import cgi

#cgitb.enable()

form = cgi.FieldStorage()
name = form.getvalue('name', 'world')

print(f"<h2>Hello, {name}!</h2>")
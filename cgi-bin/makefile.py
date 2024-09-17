import cgi
import os

#!/usr/bin/env python

# Get user input from form
form = cgi.FieldStorage()
text = form.getvalue('message')

# Create a new file and write user input to it
UPLOAD_DIR = "uploads/"
if not os.path.exists(UPLOAD_DIR):
	os.makedirs(UPLOAD_DIR)

filename = os.path.join(UPLOAD_DIR, 'message.txt')
with open(filename, 'w') as file:
	file.write(text)

print('<html>')
print('<body>')
print('<h1>File created successfully!</h1>')
print('</body>')
print('</html>')
import cgi
import os

#!/usr/bin/env python3


# Set the upload directory
UPLOAD_DIR = "../uploads"
if not os.path.exists(UPLOAD_DIR):
	os.makedirs(UPLOAD_DIR)

# Create the form object
form = cgi.FieldStorage()

# Check if a file was uploaded
if "file" in form:
	# Get the file object
	file = form["file"]

	# Check if the file was successfully uploaded
	if file.filename:
		# Create a unique filename
		filename = os.path.join(UPLOAD_DIR, file.filename)

		# Save the file to the upload directory
		with open(filename, "wb") as f:
			f.write(file.file.read())

		# Print a success message
		print("Content-Type: text/html")
		print()
		print("File uploaded successfully.")
	else:
		# Print an error message
		print("Content-Type: text/html")
		print()
		print("No file selected.")
else:
	# Print an error message
	print("Content-Type: text/html")
	print()
	print("No file uploaded.")
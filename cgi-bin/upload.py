import cgi
import os

SIZE_LIMIT = 150000

# Set the upload directory
UPLOAD_DIR = "uploads/"
if not os.path.exists(UPLOAD_DIR):
	os.makedirs(UPLOAD_DIR)


# Create the form object
form = cgi.FieldStorage()

# Get the file object
file = form["file"]

# Check if the file was successfully uploaded
if file.filename:
	# Create a unique filename
	filename = os.path.join(UPLOAD_DIR, file.filename)

	# Check file size
	if int(os.environ['CONTENT_LENGTH']) < SIZE_LIMIT:
		# Save the file to the upload directory
		with open(filename, "wb") as f:
			f.write(file.file.read())

		# Print a success message
		print("File uploaded successfully.")

	else:
		print(f"File size ({os.environ['CONTENT_LENGTH']} bytes) exceeds the limit ({SIZE_LIMIT} bytes)")
else:
	print("No file selected.")
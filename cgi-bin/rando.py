import cgi
import random

#!/usr/bin/env python3


# Create instance of FieldStorage
form = cgi.FieldStorage()

# Define a function to safely convert a value to an integer
def safe_int(value, default):
	try:
		return int(value)
	except:
		return default

# Get user input
start = safe_int(form.getvalue('start', 0), 0)
end = safe_int(form.getvalue('end', 100), 100)

# Ensure start is less than end
if start > end:
	start, end = end, start

# Generate random number
random_number = random.randint(start, end)

# Print the random number as the CGI response
print(f"The random number between {start} and {end} is: {random_number}")
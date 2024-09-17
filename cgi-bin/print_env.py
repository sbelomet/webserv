import os


for key, value in os.environ.items():
	print(f"<p>{key}: {value}</p>")
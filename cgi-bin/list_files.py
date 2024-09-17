import os
import json

UPLOAD_DIR = "uploads/"
files = os.listdir(UPLOAD_DIR)
print(json.dumps(files))
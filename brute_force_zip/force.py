import requests
import os
import string
import itertools
import zipfile
from dotenv import load_dotenv
load_dotenv()

access_token = os.getenv("ACCESS_TOKEN")

request_url = f"https://hackattic.com/challenges/brute_force_zip/problem?access_token={access_token}"

response = requests.get(request_url)

zip_url = response.json()["zip_url"]

response = requests.get(zip_url, stream=True)

with open('downloaded_file.zip', 'wb') as f:
    f.write(response.content)

# charset = string.ascii_lowercase + string.digits

# zip_file = zipfile.ZipFile('downloaded_file.zip')

# def brute_force():
#     for length in range(4, 7):
#         for candidate in itertools.product(charset, repeat=length):
#             password = ''.join(candidate)
#             try:
#                 zip_file.extractall(path="./extracted_content",pwd=password.encode('ascii'))
#                 return password
#             except (RuntimeError, zipfile.BadZipFile):
#                 continue

# global_password = brute_force()

# print(f"Password found: {global_password}")

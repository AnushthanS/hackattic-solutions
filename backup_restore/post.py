import requests
import json
import os
from dotenv import load_dotenv
load_dotenv()

access_token = os.getenv("ACCESS_TOKEN")

submission_url = f"https://hackattic.com/challenges/backup_restore/solve?access_token={access_token}"

with open('ssns.json', 'r') as f:
    data = json.load(f)

response = requests.post(url = submission_url, json = data)

print(response.json())
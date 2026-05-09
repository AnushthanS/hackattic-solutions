import requests
import json
import os
from dotenv import load_dotenv
load_dotenv()

access_token = os.getenv("ACCESS_TOKEN")

problem_url = f"https://hackattic.com/challenges/backup_restore/problem?access_token={access_token}"

response = requests.get(url=problem_url)

with open('dump.json', 'w') as f:
    json.dump(response.json(), f, indent=4)
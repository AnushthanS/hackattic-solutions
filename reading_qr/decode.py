import requests
import os
import cv2
import numpy as np
from dotenv import load_dotenv
load_dotenv()

access_token = os.getenv('ACCESS_TOKEN')

problem_url = f"https://hackattic.com/challenges/reading_qr/problem?access_token={access_token}"

response = requests.get(problem_url)

image_url = response.json().get('image_url')

response = requests.get(image_url, stream=True)

# with open("downloaded_qr.png", "wb") as f:
#     f.write(response.content)

image_array = np.frombuffer(response.content, np.uint8)

image = cv2.imdecode(image_array, cv2.IMREAD_COLOR)

detector = cv2.QRCodeDetector()
decoded_text, points, _ = detector.detectAndDecode(image)

solution = {
    "code": decoded_text
}

solution_url = f"https://hackattic.com/challenges/reading_qr/solve?access_token={access_token}"

response = requests.post(solution_url, json=solution)

print(response.json())
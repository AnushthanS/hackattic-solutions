import base64
import struct
import requests
import os
from dotenv import load_dotenv
load_dotenv()

access_token = os.getenv('ACCESS_TOKEN')

problem_url = f"https://hackattic.com/challenges/help_me_unpack/problem?access_token={access_token}"

response = requests.get(problem_url)

data = response.json()

decoded_data = base64.b64decode(data['bytes'])

print("Length of bytes:", len(decoded_data))

#    < : little-endian
#    i : signed int (4 bytes)
#    I : unsigned int (4 bytes)
#    h : short (2 bytes)
#    2x : skip 2 bytes
#    f : float (4 bytes)
#    d : double (8 bytes)
format_string = "<iIh2xfd"
unpacked_values = struct.unpack(format_string, decoded_data[:24])

format_string = ">d"
big_endian_double = struct.unpack(format_string, decoded_data[24:])[0]

solution = {
    "int": unpacked_values[0],
    "uint": unpacked_values[1],
    "short": unpacked_values[2],
    "float": unpacked_values[3],
    "double": unpacked_values[4],
    "big_endian_double": big_endian_double
}

solution_url = f"https://hackattic.com/challenges/help_me_unpack/solve?access_token={access_token}"

response = requests.post(solution_url, json=solution)

print("Response status code:", response.status_code)
print("Response JSON:", response.json())
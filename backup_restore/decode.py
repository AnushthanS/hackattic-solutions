import base64
import gzip
import json

with open("dump.json", "r") as f:
    json_data = json.load(f)

compressed_dump = base64.b64decode(json_data['dump'])
decompressed_dump = gzip.decompress(compressed_dump)

with open("dump.pg", "wb") as f:
    f.write(decompressed_dump)
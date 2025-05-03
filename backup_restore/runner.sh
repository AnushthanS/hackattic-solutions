#!/bin/zsh

source ../pyvenv/bin/activate

docker run --rm --name postgres-11-container -e POSTGRES_PASSWORD=password -p 5432:5432 -d postgres:11

python3 get.py
python3 decode.py

sleep 3 #wait for the container, change this accordingly

docker cp dump.pg postgres-11-container:/

docker exec postgres-11-container psql -U postgres -c "DROP DATABASE IF EXISTS mydb;"
docker exec postgres-11-container psql -U postgres -c "CREATE DATABASE mydb;"

docker exec postgres-11-container psql -U postgres -d mydb -f /dump.pg

docker exec postgres-11-container psql -U postgres -d mydb -t -A -c "SELECT json_build_object('alive_ssns', array_agg(ssn)) FROM criminal_records WHERE status = 'alive';" > ssns.json

python3 post.py
rm dump*
rm ssns*

docker stop postgres-11-container
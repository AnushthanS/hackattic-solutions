#!/bin/zsh

PORT=5000

echo "Set PORT to $PORT"

echo "Pulling registry image from hub.docker"

podman pull docker.io/registry

echo "Spinning up registry container"

podman run -d --rm -p $PORT:$PORT --name registry registry

SLEEP=30
echo "Sleep for $SLEEP seconds, allow registry to set up"
sleep $SLEEP

curl localhost:5000/v2/

echo "Starting ngrok on $PORT"
ngrok http $PORT

#!/bin/bash

# Load project name from config
source ./project.conf

echo "Cleaning build directory..."
./cleanup.sh
echo "Cleanup done."
echo "----------------------------------"
echo ""

echo "Running inside Docker container..."
echo "---> Make sure Docker Daemon is running <---"
docker run -it --rm --name stm32_container -v "$(pwd):${DOCKER_INPUT_PATH}" "${DOCKER_IMAGE_NAME}"


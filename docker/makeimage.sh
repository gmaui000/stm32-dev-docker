#!/bin/bash
# Load project name from config
source ../project.conf

docker build . -t "${DOCKER_IMAGE_NAME}"

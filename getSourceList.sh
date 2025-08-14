#!/bin/bash

# Load project name from config
source ./project.conf

# Run ./getSourceList.sh in terminal to generate an updated list of source for CMake

# Clear the file
> "$CMAKE_SRC_FILE_PATH"

# Print find result
if [ -n "$APP_PATH" ]; then
	APP_SRC="$(find "$APP_PATH" -type f -name '*.c' -o -name '*.cpp' -o -name '*.s')"
	printf "$APP_SRC\n" >> "$CMAKE_SRC_FILE_PATH"
fi

if [ -n "$CORE_PATH" ]; then
	CORE_SRC="$(find "$CORE_PATH" -type f -name '*.c' -o -name '*.cpp' -o -name '*.s')"
	printf "$CORE_SRC\n" >> "$CMAKE_SRC_FILE_PATH"
fi

if [ -n "$SRC_DRIVERS_PATH" ]; then
	DRIVERS_SRC="$(find "$SRC_DRIVERS_PATH" -type f -name '*.c')"
	printf "$DRIVERS_SRC\n" >> "$CMAKE_SRC_FILE_PATH"
fi

if [ -n "$FATFS_PATH" ]; then
	FATFS_SRC="$(find "$FATFS_PATH" -type f -name '*.c')"
	printf "$FATFS_SRC\n" >> "$CMAKE_SRC_FILE_PATH"
fi

if [ -n "$MIDWARE_PATH" ]; then
	MIDWARE_SRC="$(find "$MIDWARE_PATH" -type f -name '*.c')"
	printf "$MIDWARE_SRC\n" >> "$CMAKE_SRC_FILE_PATH"
fi

if [ -n "$USB_PATH" ]; then
	USB_SRC="$(find "$USB_PATH" -type f -name '*.c')"
	printf "$USB_SRC\n" >> "$CMAKE_SRC_FILE_PATH"
fi

# sed command works different for MacOS and Linux 
if [[ "$OSTYPE" == "darwin"* ]]; then
	# ========== FOR MACOS ==========
	# Remove Empty Line
	sed -i '' '/./!d' "$CMAKE_SRC_FILE_PATH"

	# Adding ${PROJ_PATH}/ to beginning of each line
	sed -i '' 's|^|${PROJ_PATH}/|' "$CMAKE_SRC_FILE_PATH"

	# Struct cmake format (Do not add tab space to line)
	sed -i '' '1i\
set(source_list ${source_list}' "$CMAKE_SRC_FILE_PATH"
	sed -i '' -e '$a\'$'\n'')' "$CMAKE_SRC_FILE_PATH"
else
	# ========== FOR LINUX  ==========
	# Remove Empty Line
	sed -i '/./!d' "$CMAKE_SRC_FILE_PATH"

	# Adding ${PROJ_PATH}/ to beginning of each line
	sed -i 's|^|${PROJ_PATH}/|' "$CMAKE_SRC_FILE_PATH"

	# Struct cmake format
	sed -i '1i set(source_list ${source_list}' "$CMAKE_SRC_FILE_PATH"
	sed -i '$a)' "$CMAKE_SRC_FILE_PATH"
fi
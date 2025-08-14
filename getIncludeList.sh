#!/bin/bash

# Load project name from config
source ./project.conf

# Run ./getIncludeList.sh in terminal to generate an updated list of source for CMake

# Clear the file
> "$CMAKE_INC_FILE_PATH"

if [ -n "$APP_PATH" ]; then
	APP_INC="$(find "$APP_PATH" -type d -name 'Inc')"
	printf "$APP_INC\n" >>"$CMAKE_INC_FILE_PATH"
fi

if [ -n "$DEP_LIB_PATH" ]; then
	DEP_LIB_INC="$(find "$DEP_LIB_PATH" -type d)"
	printf "$DEP_LIB_INC\n" >>"$CMAKE_INC_FILE_PATH"
fi

if [ -n "$CORE_PATH" ]; then
	CORE_INC="$(find "$CORE_PATH" -type d -name 'Inc' -o -name 'Include')"
	printf "$CORE_INC\n" >>"$CMAKE_INC_FILE_PATH"
fi

if [ -n "$INC_DRIVERS_PATH" ]; then
	DRIVERS_INC="$(find "$INC_DRIVERS_PATH" -type d -name 'Inc' -o -name 'Include' -o -name 'Legacy')"
	printf "$DRIVERS_INC\n" >>"$CMAKE_INC_FILE_PATH"
fi

if [ -n "$FATFS_PATH" ]; then
	FATFS_INC="$(find "$FATFS_PATH" -type d)"
	printf "$FATFS_INC\n" >>"$CMAKE_INC_FILE_PATH"
fi

if [ -n "$RTOS_PATH" ]; then
	RTOS_INC="$(find "$MIDWARE_PATH" -type d -name 'include' -o -name 'CMSIS_RTOS' -o -name 'ARM_CM4F')"
	printf "$RTOS_INC\n" >>"$CMAKE_INC_FILE_PATH"
fi

if [ -n "$MIDWARE_PATH" ]; then
	MIDWARE_INC="$(find "$MIDWARE_PATH" -type d -name 'Inc' -o -name 'src')"
	printf "$MIDWARE_INC\n" >>"$CMAKE_INC_FILE_PATH"
fi

if [ -n "$USB_PATH" ]; then
	USB_INC="$(find "$USB_PATH" -type d)"
	printf "$USB_INC\n" >>"$CMAKE_INC_FILE_PATH"
fi

# sed command works different for MacOS and Linux 
if [[ "$OSTYPE" == "darwin"* ]]; then
	# ========== FOR MACOS ==========
	# Remove Empty Line
	sed -i '' '/./!d' "$CMAKE_INC_FILE_PATH"

	# Adding ${PROJ_PATH}/ to beginning of each line
	sed -i '' 's|^|${PROJ_PATH}/|' "$CMAKE_INC_FILE_PATH"

	# Struct cmake format (Do not add tab space to line - 68)
	sed -i '' '1s|^|set(include_list ${include_list}\n|; $s|$|\n)|' "$CMAKE_INC_FILE_PATH"
else
	# ========== FOR LINUX  ==========
	# Remove Empty Line
	sed -i '/./!d' "$CMAKE_INC_FILE_PATH"

	# Adding ${PROJ_PATH}/ to beginning of each line
	sed -i 's|^|${PROJ_PATH}/|' "$CMAKE_INC_FILE_PATH"

	# Struct cmake format
	sed -i '1s|^|set(include_list ${include_list}\n|; $s|$|\n)|' "$CMAKE_INC_FILE_PATH"
fi

# Only add closing ')' if not already present
if [[ "$(tail -n 1 "$CMAKE_INC_FILE_PATH")" != ")" ]]; then
    echo ")" >> "$CMAKE_INC_FILE_PATH"
fi
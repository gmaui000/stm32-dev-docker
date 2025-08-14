# STM32 CMake + Docker Project

A modern STM32 firmware build system for the STM32F4 MCU or can be modified of any STM32xx MCU, using **CMake** and **Docker** for portability and toolchain isolation.


## Docker-Based Build System

No need to install ARM GCC or CMake on your host system. All builds run inside a Docker container, ensuring a clean, consistent environment.

## Prerequisites

Before building or flashing, make sure you have:

- ✅ [Docker installed and running](#build-docker-image)
- ✅ OpenOCD installed (for flashing firmware)  
- ✅ USB access set up (especially for WSL users)
- ✅ [Set MCU parameters and definitions](#mcu-specific-file)

See [Embedded Development Environment Setup Guide](#embedded-development-environment-setup-guide)
 below for platform-specific setup.

## MCU specific file

Each MCU has their own ARM compiler flags. This template is defined for STM32F4. The flags can be changed in the following module.

> STM32_PARA.cmake

```makefile
set(CPU_PARAMETERS ${CPU_PARAMETERS}
    -mthumb
    -mcpu=cortex-m4
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard
)

set(compiler_define ${compiler_define}
    "STM32F429xx"
    #"USE_HAL_DRIVER" # uncomment and modify entire cmake template for HAL
)

```

> **General rule for settings would be as per table below:**

| STM32 Family | -mcpu           | -mfpu         | -mfloat-abi |
| ------------ | --------------- | ------------- | ----------- |
| STM32F0      | `cortex-m0`     | `Not used`    | `soft`      |
| STM32F1      | `cortex-m3`     | `Not used`    | `soft`      |
| STM32F2      | `cortex-m3`     | `Not used`    | `soft`      |
| STM32F3      | `cortex-m4`     | `fpv4-sp-d16` | `hard`      |
| STM32F4      | `cortex-m4`     | `fpv4-sp-d16` | `hard`      |
| STM32F7 SP   | `cortex-m7`     | `fpv5-sp-d16` | `hard`      |
| STM32F7 DP   | `cortex-m7`     | `fpv5-d16`    | `hard`      |
| STM32G0      | `cortex-m0plus` | `Not used`    | `soft`      |
| STM32C0      | `cortex-m0plus` | `Not used`    | `soft`      |
| STM32G4      | `cortex-m4`     | `fpv4-sp-d16` | `hard`      |
| STM32H7      | `cortex-m7`     | `fpv5-d16`    | `hard`      |
| STM32L0      | `cortex-m0plus` | `Not used`    | `soft`      |
| STM32L1      | `cortex-m3`     | `Not used`    | `soft`      |
| STM32L4      | `cortex-m4`     | `fpv4-sp-d16` | `hard`      |
| STM32L5      | `cortex-m33`    | `fpv5-sp-d16` | `hard`      |
| STM32U5      | `cortex-m33`    | `fpv5-sp-d16` | `hard`      |
| STM32WB      | `cortex-m4`     | `fpv4-sp-d16` | `hard`      |
| STM32WL CM4  | `cortex-m4`     | `Not used`    | `soft`      |
| STM32WL CM0  | `cortex-m0plus` | `Not used`    | `soft`      |

&nbsp;

## Build Docker Image

Before you build your firmware, make sure to build the Docker image.  
You only need to do this once unless you change the image configuration.

### Step 1: Modify `project.conf` (Optional)

To change the Docker image tag, edit:

```bash
DOCKER_IMAGE_NAME=stm32-compiler
```
Make sure the same value is updated in your Dockerfile:
```bash
ARG DOCKER_IMAGE_NAME=stm32-compiler
```

### Step 2:  Build the Docker Image
Inside the /Docker folder:
```bash
./makeimage.sh
```

## Quick Start

### One-Liner Clone & Build

```bash
docker run --rm -v "$(pwd)":/app <docker-image> -r <your-project-git-url>
```

&nbsp;

# Embedded Development Environment Setup Guide

This guide covers setup for **Windows (WSL)**, **macOS**, and **Linux** for embedded development with OpenOCD and USB device support.


## 🪟 Windows (WSL) Setup

### 1. Install WSL (Ubuntu)

#### a. Enable Required Windows Features

Go to **Control Panel → Programs → Turn Windows features on or off**, and enable:

* Virtual Machine Platform
* Windows Subsystem for Linux

Alternatively, via PowerShell (Admin):

```powershell
dism.exe /online /enable-feature /featurename:VirtualMachinePlatform /all /norestart
dism.exe /online /enable-feature /featurename:Microsoft-Windows-Subsystem-Linux /all /norestart
```

#### b. Install WSL

```powershell
wsl --install
```

This installs:

* WSL 2
* Ubuntu (default)
* Required kernel updates

> 📌 Reboot after install.
> To check install: `wsl --status`

---

### 2. Install Docker for Windows

Download & install Docker:
[https://www.docker.com/products/docker-desktop/](https://www.docker.com/products/docker-desktop/)

Enable WSL 2 integration in Docker settings:

* **Settings → Resources → WSL Integration** → Enable for your Ubuntu distro

> ✅ Confirm in WSL: `docker --version`

---

### 3. Install OpenOCD (Inside WSL)

```bash
sudo apt update
sudo apt install openocd
```

> ✅ Confirm installation: `openocd --version`

---

### 4. Enable USB Access in WSL

Microsoft Guide:
[https://learn.microsoft.com/en-us/windows/wsl/connect-usb](https://learn.microsoft.com/en-us/windows/wsl/connect-usb)

#### a. Install `usbipd-win` (on Windows)

```powershell
winget install --interactive --exact dorssel.usbipd-win
```

#### b. Bind USB device

```powershell
usbipd list
usbipd bind --busid <busid>
usbipd attach --wsl --busid <busid>
```

#### c. Install tools inside WSL

```bash
sudo apt install linux-tools-$(uname -r) linux-cloud-tools-$(uname -r)
```

#### d. Restart WSL

```powershell
wsl --shutdown
```

Then relaunch Ubuntu.

#### e. Confirm device in WSL

```bash
lsusb
```

Should list your ST-Link/J-Link/CMSIS-DAP.

#### f. Detach USB device

```powershell
usbipd detach --busid <busid>
```


## 🍎 macOS Setup

### 1. Install Homebrew (if not installed)

```bash
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

### 2. Install required packages

```bash
brew install openocd usbutils
```

✅ Confirm installation:
```bash
openocd --version
lsusb
```
## 🐧 Linux Setup (Native Ubuntu/Debian)

### 1. Install required packages

```bash
sudo apt update
sudo apt install openocd usbutils
```

✅ Confirm installation:
```bash
openocd --version
lsusb
```
---

You're now ready to use OpenOCD and interface with dev boards on your platform of choice.
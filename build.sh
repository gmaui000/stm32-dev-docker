#!/bin/bash

# STM32 Docker Build Script

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Function to show usage
show_usage() {
    echo -e "${YELLOW}Usage: $0 [OPTIONS]${NC}"
    echo "Options:"
    echo "  -b, --build     Build the project (default)"
    echo "  -f, --flash     Build and flash to device"
    echo "  -c, --clean     Clean build directory"
    echo "  -s, --server    Start OpenOCD debug server"
    echo "  -g, --gdb       Start GDB client in container"
    echo "  -e, --erase     Erase flash memory"
    echo "  -r, --reset     Reset target device"
    echo "  -i, --info      Show device information"
    echo "  -h, --help      Show this help message"
    echo ""
    echo -e "${BLUE}Examples:${NC}"
    echo "  $0              # Build only"
    echo "  $0 --flash      # Build and flash"
    echo "  $0 --clean      # Clean build directory"
    echo "  $0 --server     # Start OpenOCD debug server"
    echo "  $0 --gdb        # Start GDB client in container"
    echo "  $0 --erase      # Erase flash memory"
    echo "  $0 --reset      # Reset target device"
    echo "  $0 --info       # Show device information"
}

# Default action
ACTION="build"

# Parse command line arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        -b|--build)
            ACTION="build"
            shift
            ;;
        -f|--flash)
            ACTION="flash"
            shift
            ;;
        -c|--clean)
            ACTION="clean"
            shift
            ;;
        -s|--server)
            ACTION="server"
            shift
            ;;
        -g|--gdb)
            ACTION="gdb"
            shift
            ;;
        -e|--erase)
            ACTION="erase"
            shift
            ;;
        -r|--reset)
            ACTION="reset"
            shift
            ;;
        -i|--info)
            ACTION="info"
            shift
            ;;
        -h|--help)
            show_usage
            exit 0
            ;;
        *)
            echo -e "${RED}Unknown option: $1${NC}"
            show_usage
            exit 1
            ;;
    esac
done

echo -e "${YELLOW}STM32 Docker Build Environment${NC}"
echo "=================================="

# Check if Docker is installed
if ! command -v docker &> /dev/null; then
    echo -e "${RED}Error: Docker is not installed. Please install Docker first.${NC}"
    exit 1
fi

# Build the Docker image
echo -e "${YELLOW}Building Docker image...${NC}"
docker build -t stm32-dev-docker .

if [ $? -ne 0 ]; then
    echo -e "${RED}Failed to build Docker image${NC}"
    exit 1
fi

echo -e "${GREEN}Docker image built successfully${NC}"

# Perform the requested action
case $ACTION in
    "build")
        # Run the build in Docker container
        echo -e "${YELLOW}Building STM32 project...${NC}"
        docker run --rm -v $(pwd):/workspace stm32-dev-docker make all
        
        if [ $? -eq 0 ]; then
            echo -e "${GREEN}Build completed successfully!${NC}"
            echo -e "${GREEN}Output files are in the 'build' directory:${NC}"
            ls -la build/
        else
            echo -e "${RED}Build failed${NC}"
            exit 1
        fi
        ;;
        
    "flash")
        # Build and flash
        echo -e "${YELLOW}Building STM32 project...${NC}"
        docker run --rm -v $(pwd):/workspace stm32-dev-docker make all
        
        if [ $? -eq 0 ]; then
            echo -e "${GREEN}Build completed successfully!${NC}"
            echo -e "${YELLOW}Flashing to device...${NC}"
            
            # Use Docker container for OpenOCD flashing
            echo -e "${BLUE}Using Docker container for OpenOCD flashing...${NC}"
            if [ -e "/dev/bus/usb" ]; then
                docker run --rm --privileged -v /dev/bus/usb:/dev/bus/usb -v $(pwd):/workspace stm32-dev-docker \
                    openocd -f interface/stlink-v2.cfg -f target/stm32f1x.cfg -c "program /workspace/build/time_sync.bin verify reset exit 0x08000000"
                
                if [ $? -eq 0 ]; then
                    echo -e "${GREEN}Flash completed successfully using Docker OpenOCD!${NC}"
                    echo -e "${GREEN}Device is now running the program.${NC}"
                else
                    echo -e "${YELLOW}Docker OpenOCD flash failed, trying alternative method...${NC}"
                    
                    # Alternative method: Use Docker container for st-flash
                    echo -e "${BLUE}Using Docker container for st-flash...${NC}"
                    docker run --rm --privileged -v /dev/bus/usb:/dev/bus/usb -v $(pwd):/workspace stm32-dev-docker \
                        st-flash write /workspace/build/time_sync.bin 0x8000000
                    
                    if [ $? -eq 0 ]; then
                        echo -e "${GREEN}Flash completed successfully using Docker st-flash!${NC}"
                        echo -e "${GREEN}Device is now running the program.${NC}"
                    else
                        echo -e "${RED}All flash methods failed${NC}"
                        echo -e "${YELLOW}Troubleshooting steps:${NC}"
                        echo "  1. Check ST-Link connection and device power"
                        echo "  2. Try running with sudo: sudo ./build.sh --flash"
                        echo "  3. Ensure no other programs are using the ST-Link"
                        echo ""
                        echo -e "${BLUE}Manual flashing options:${NC}"
                        echo "  - Use STM32CubeProgrammer GUI tool"
                        echo "  - Use J-Flash if you have J-Link"
                        echo "  - Use serial bootloader with stm32flash"
                        exit 1
                    fi
                fi
            else
                echo -e "${RED}Cannot access USB devices. Please run with sudo or check permissions.${NC}"
                echo -e "${YELLOW}You can try:${NC}"
                echo "  sudo ./build.sh --flash"
                exit 1
            fi
        else
            echo -e "${RED}Build failed, cannot flash${NC}"
            exit 1
        fi
        ;;
        
    "clean")
        # Clean build directory
        echo -e "${YELLOW}Cleaning build directory...${NC}"
        docker run --rm -v $(pwd):/workspace stm32-dev-docker make clean
    
        if [ $? -eq 0 ]; then
            echo -e "${GREEN}Clean completed successfully!${NC}"
        else
            echo -e "${RED}Clean failed${NC}"
            exit 1
        fi
        ;;
        
    "server")
        # Start OpenOCD debug server in Docker container
        echo -e "${YELLOW}Starting STM32 Debug Server...${NC}"
        echo -e "${BLUE}OpenOCD will start a GDB server on port 3333${NC}"
        echo -e "${BLUE}Available ports:${NC}"
        echo -e "${BLUE}  - GDB: 3333${NC}"
        echo -e "${BLUE}  - Telnet: 4444${NC}"
        echo -e "${BLUE}  - TCL: 6666${NC}"
        echo ""
        echo -e "${YELLOW}Usage instructions:${NC}"
        echo -e "${BLUE}  • Press Ctrl+C once to gracefully stop the server${NC}"
        echo -e "${BLUE}  • Server will exit immediately when stopped${NC}"
        echo -e "${BLUE}  • Manual control - no automatic timeout${NC}"
        echo ""
        echo -e "${YELLOW}To connect with GDB client:${NC}"
        echo -e "${BLUE}  ./build.sh --gdb${NC}"
        echo -e "${BLUE}  or${NC}"
        echo -e "${BLUE}  docker run -it --rm --name stm32-runner -v \$(pwd):/workspace stm32-dev-docker gdb-multiarch /workspace/build/time_sync.elf${NC}"
        echo ""
        if [ -e "/dev/bus/usb" ]; then
            # Copy the debug server script to container and run it
            docker run --init --rm --privileged --name stm32-runner -v /dev/bus/usb:/dev/bus/usb -v $(pwd):/workspace stm32-dev-docker \
                bash -c "cp /workspace/debug_server.sh /tmp/ && chmod +x /tmp/debug_server.sh && cd /tmp && ./debug_server.sh"
        else
            echo -e "${RED}Cannot access USB devices. Please run with sudo or check permissions.${NC}"
            echo -e "${YELLOW}You can try:${NC}"
            echo "  sudo ./build.sh --server"
            exit 1
        fi
        ;;
        
    "gdb")
        # Start GDB client in Docker container with auto-breakpoint at main
        echo -e "${YELLOW}Starting GDB client in Docker container...${NC}"
        echo -e "${BLUE}Connecting to OpenOCD server on localhost:3333${NC}"
        echo -e "${BLUE}Auto-breakpoint will be set at main() function${NC}"
        echo ""
        echo -e "${YELLOW}GDB commands (auto-executed):${NC}"
        echo -e "${BLUE}  target remote localhost:3333${NC}"
        echo -e "${BLUE}  monitor reset halt${NC}"
        echo -e "${BLUE}  load${NC}"
        echo -e "${BLUE}  break main${NC}"
        echo -e "${BLUE}  continue${NC}"
        echo ""
        echo -e "${BLUE}Make sure OpenOCD server is running first:${NC}"
        echo -e "${BLUE}  ./build.sh --server${NC}"
        echo ""
        docker exec -it stm32-runner gdb-multiarch -ex "target remote localhost:3333" -ex "monitor reset halt" -ex "load" -ex "break main" -ex "continue" /workspace/build/time_sync.elf
        ;;
        
    "erase")
        # Erase flash memory using Docker container
        echo -e "${YELLOW}Erasing flash memory using Docker container...${NC}"
        if [ -e "/dev/bus/usb" ]; then
            docker run --rm --privileged -v /dev/bus/usb:/dev/bus/usb -v $(pwd):/workspace stm32-dev-docker \
                openocd -f interface/stlink-v2.cfg -f target/stm32f1x.cfg -c "init; reset halt; stm32f1x mass_erase 0; exit"
            
            if [ $? -eq 0 ]; then
                echo -e "${GREEN}Flash memory erased successfully!${NC}"
            else
                echo -e "${RED}Flash erase failed${NC}"
                exit 1
            fi
        else
            echo -e "${RED}Cannot access USB devices. Please run with sudo or check permissions.${NC}"
            echo -e "${YELLOW}You can try:${NC}"
            echo "  sudo ./build.sh --erase"
            exit 1
        fi
        ;;
        
    "reset")
        # Reset target device using Docker container
        echo -e "${YELLOW}Resetting target device using Docker container...${NC}"
        if [ -e "/dev/bus/usb" ]; then
            docker run --rm --privileged -v /dev/bus/usb:/dev/bus/usb -v $(pwd):/workspace stm32-dev-docker \
                openocd -f interface/stlink-v2.cfg -f target/stm32f1x.cfg -c "init; reset; exit"
            
            if [ $? -eq 0 ]; then
                echo -e "${GREEN}Device reset successfully!${NC}"
            else
                echo -e "${RED}Device reset failed${NC}"
                exit 1
            fi
        else
            echo -e "${RED}Cannot access USB devices. Please run with sudo or check permissions.${NC}"
            echo -e "${YELLOW}You can try:${NC}"
            echo "  sudo ./build.sh --reset"
            exit 1
        fi
        ;;
        
    "info")
        # Show device information using Docker container
        echo -e "${YELLOW}Reading device information using Docker container...${NC}"
        if [ -e "/dev/bus/usb" ]; then
            docker run --rm --privileged -v /dev/bus/usb:/dev/bus/usb -v $(pwd):/workspace stm32-dev-docker \
                openocd -f interface/stlink-v2.cfg -f target/stm32f1x.cfg -c "init; flash banks; exit" 2>/dev/null | grep -E "(STM32|Flash|Size|Device)"
            
            if [ $? -eq 0 ]; then
                echo -e "${GREEN}Device information retrieved successfully!${NC}"
            else
                echo -e "${RED}Failed to read device information${NC}"
                echo -e "${YELLOW}Trying alternative method...${NC}"
                docker run --rm --privileged -v /dev/bus/usb:/dev/bus/usb stm32-dev-docker st-info --probe
            fi
        else
            echo -e "${RED}Cannot access USB devices. Please run with sudo or check permissions.${NC}"
            echo -e "${YELLOW}You can try:${NC}"
            echo "  sudo ./build.sh --info"
            exit 1
        fi
        ;;
esac

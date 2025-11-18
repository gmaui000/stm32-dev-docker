#!/bin/bash

# STM32 Debug Server with reliable exit functionality and auto-breakpoint

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

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
echo -e "${BLUE}  • No automatic timeout - manual control only${NC}"
echo -e "${BLUE}  • Auto-breakpoint set at main() function${NC}"
echo ""
echo -e "${YELLOW}To connect with GDB client:${NC}"
echo -e "${BLUE}  ./build.sh --gdb${NC}"
echo -e "${BLUE}  GDB will automatically break at main() function${NC}"
echo ""

# Function to start OpenOCD with proper signal handling
start_openocd() {
    local openocd_pid
    
    echo -e "${YELLOW}Starting OpenOCD...${NC}"
    
    # Start OpenOCD in foreground with proper signal handling
    openocd -f interface/stlink-v2.cfg -f target/stm32f1x.cfg &
    openocd_pid=$!
    
    echo -e "${GREEN}OpenOCD started with PID: $openocd_pid${NC}"
    echo -e "${BLUE}Server is ready for GDB connections${NC}"
    echo -e "${BLUE}Press Ctrl+C to stop the server${NC}"
    
    # Wait for OpenOCD process and handle signals
    wait $openocd_pid
    local exit_code=$?
    
    echo -e "${YELLOW}OpenOCD process ended with exit code: $exit_code${NC}"
    echo -e "${GREEN}Debug server shutdown complete${NC}"
}

# Main execution
if [ -e "/dev/bus/usb" ]; then
    # Set up signal handlers for clean shutdown
    trap 'echo -e "${YELLOW}Received shutdown signal, stopping OpenOCD...${NC}"; kill $openocd_pid 2>/dev/null; exit 0' SIGTERM SIGINT
    
    start_openocd
else
    echo -e "${RED}Cannot access USB devices. Please check permissions or run with sudo.${NC}"
    exit 1
fi

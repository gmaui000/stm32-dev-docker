# STM32 Timer Sync Project - Docker Build Environment

这个项目提供了一个Docker构建环境，用于在Ubuntu系统上构建STM32F103C8的PWM定时器同步项目，无需安装Keil MDK开发环境。

## 项目概述

这是一个基于STM32F103C8的PWM定时器同步项目，包含：
- 两个PWM定时器（TIM2和TIM3）
- LED控制
- 按键输入
- 串口通信
- 系统延时功能

## 构建环境要求

- Docker (已安装并运行)
- Ubuntu/Linux系统

## 快速开始

### 方法1：使用构建脚本（推荐）

```bash
# 给构建脚本执行权限
chmod +x build.sh

# 运行构建脚本
./build.sh
```

### 方法2：手动构建

```bash
# 构建Docker镜像
docker build -t stm32-dev-docker .

# 在Docker容器中构建项目
docker run --rm -v $(pwd):/workspace stm32-dev-docker make all
```

## 构建输出

构建完成后，在 `build/` 目录中会生成以下文件：
- `time_sync.elf` - ELF格式的可执行文件
- `time_sync.hex` - Intel HEX格式文件（用于烧录）
- `time_sync.bin` - 二进制格式文件（用于烧录）
- `time_sync.map` - 内存映射文件

## 清理构建

```bash
# 清理构建文件
docker run --rm -v $(pwd):/workspace stm32-dev-docker make clean
```

## 烧录到设备

如果需要烧录到STM32设备，可以使用ST-Link工具：

```bash
# 安装stlink-tools
sudo apt-get install stlink-tools

# 烧录二进制文件
st-flash write build/time_sync.bin 0x8000000
```

## 项目结构

```
.
├── Dockerfile              # Docker构建环境配置
├── Makefile                # 项目构建配置
├── STM32F103C8_FLASH.ld    # 链接器脚本
├── build.sh                # 构建脚本
├── README_DOCKER.md        # 本文档
├── CORE/                   # 核心文件
├── HARDWARE/               # 硬件驱动
├── SYSTEM/                 # 系统文件
├── STM32F10x_FWLib/        # STM32固件库
└── USER/                   # 用户代码
```

## 技术细节

- **MCU**: STM32F103C8 (Cortex-M3)
- **编译器**: arm-none-eabi-gcc
- **工具链**: GNU Arm Embedded Toolchain
- **内存配置**: 64KB Flash, 20KB RAM

## 故障排除

1. **Docker权限问题**
   ```bash
   # 将用户添加到docker组
   sudo usermod -aG docker $USER
   # 重新登录或重启系统
   ```

2. **构建失败**
   - 检查Docker是否正常运行
   - 确保所有源文件存在且完整
   - 查看构建输出中的错误信息

3. **内存不足**
   - 确保系统有足够的内存运行Docker
   - 可以尝试清理Docker缓存：`docker system prune`

## 许可证

本项目基于STM32标准外设库和相关开源工具构建。

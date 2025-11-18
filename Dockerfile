FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

# Change source
RUN sed -i s@archive.ubuntu.com@mirrors.tuna.tsinghua.edu.cn@g /etc/apt/sources.list
RUN sed -i s@security.ubuntu.com@mirrors.tuna.tsinghua.edu.cn@g /etc/apt/sources.list


ARG HOST_UID=1000
ARG HOST_GID=1000
ARG USER_NAME=developer

RUN apt-get update && \
    apt-get install -y \
    build-essential \
    git \
    wget \
    gcc-arm-none-eabi \
    gdb-multiarch \
    openocd \
    stlink-tools && \
    groupadd -g ${HOST_GID} ${USER_NAME} && \
    useradd -u ${HOST_UID} -g ${USER_NAME} -m -s /bin/bash ${USER_NAME} && \
    rm -rf /var/lib/apt/lists/*

# COPY ./deps/gcc-arm-none-eabi-6-2017-q2-update-linux.tar.bz2 /opt/
# RUN cd /opt && \
#     tar -xjf gcc-arm-none-eabi-6-2017-q2-update-linux.tar.bz2

# ENV PATH="/opt/gcc-arm-none-eabi-6-2017-q2-update/bin:${PATH}"

USER ${USER_NAME}

WORKDIR /workspace

CMD ["bash"]

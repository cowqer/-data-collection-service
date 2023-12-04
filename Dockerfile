# 使用基础镜像，这里以 Ubuntu 20.04 为例
FROM ubuntu:20.04

# 安装交叉编译工具链和其他依赖项
RUN apt-get update && \
    apt-get install -y zlib1g-dev \
    apt-get install -y \
    aarch64-linux-gnu-gcc \
    aarch64-linux-gnu-g++ \
    cmake

WORKDIR /app

# 复制项目文件到容器中
COPY . /app
ENV FORCE_UNSAFE_CONFIGURE=1


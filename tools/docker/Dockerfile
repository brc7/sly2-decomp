# Sly 2 decomp toolchain image: reproduces the upstream Linux/WSL environment on any Docker host
# (macOS on Apple Silicon or Intel, Windows, Linux).  Compilers are NOT baked in; tools/setup_all.sh
# downloads them into tools/compilers/ of the bind-mounted checkout (gitignored), so they persist.
#
# Everything here is x86: wibo-i686 and the Sony SDK compilers are 32-bit i386 Linux binaries.
# Platform is chosen by `docker build --platform linux/amd64` in tools/dev.sh.
FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive
RUN dpkg --add-architecture i386 \
 && apt-get update \
 && apt-get install -y --no-install-recommends \
      python3 python3-pip git curl ca-certificates xz-utils file binutils \
      libc6:i386 \
 && rm -rf /var/lib/apt/lists/*

# rabbitizer: matchtool.py; spimdisasm + splat: optional asm/ regeneration
RUN pip3 install --no-cache-dir --break-system-packages rabbitizer spimdisasm "splat64[mips]"

# m2c: only for tools/show.py drafts and raw m2c .c sources (m2c_macros.h)
RUN git clone --depth 1 https://github.com/matt-kempster/m2c /opt/m2c && ln -s /opt/m2c /root/m2c
ENV SLY2_M2C_INC=/opt/m2c

WORKDIR /sly2

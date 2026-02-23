# 🏗️ base: build environment
# 🐢 Slow layer — installs Emojicode + build deps. Cached aggressively by Docker/GHA.
# emojicodec is x86_64-only; pin the platform so this works on ARM hosts too (via QEMU).
FROM --platform=linux/amd64 ubuntu:22.04 AS base
ARG DEBIAN_FRONTEND=noninteractive
# libtinfo5: emojicodec links against libtinfo.so.5 (Ubuntu 22.04 ships v6 by default)
RUN apt-get update && apt-get install -y \
    curl clang libcurl4-openssl-dev make rsync libtinfo5 \
    && rm -rf /var/lib/apt/lists/*
# 🍇 Install Emojicode 1.0 beta 2
RUN curl -fsSL \
    https://github.com/emojicode/emojicode/releases/download/v1.0-beta.2/Emojicode-1.0-beta.2-Linux-x86_64.tar.gz \
    -o /tmp/emojicode.tar.gz \
    && tar xzf /tmp/emojicode.tar.gz -C /tmp \
    && cd /tmp/Emojicode-1.0-beta.2-Linux-x86_64 \
    && echo "y" | ./install.sh \
    && rm -rf /tmp/emojicode*

# 🔨 builder: compile the agent
# Invalidated on source changes. Also used by CI to run `make test`.
FROM base AS builder
WORKDIR /app
COPY . .
RUN make build

# 🚀 runtime: minimal distributable image
# Usage: docker run -it -e OPENROUTER_API_KEY=... -v $(pwd):/workdir <image>
FROM --platform=linux/amd64 ubuntu:22.04 AS runtime
RUN apt-get update && apt-get install -y libcurl4 libtinfo5 && rm -rf /var/lib/apt/lists/*
WORKDIR /workdir
COPY --from=builder /app/out/main /usr/local/bin/emojicode-agent
COPY workdir/ /workdir/
ENTRYPOINT ["emojicode-agent"]

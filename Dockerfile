# Dockerfile for compiling Smart House Firmware using PlatformIO
FROM python:3.10-slim

# Install system dependencies & git
RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    git \
    curl \
    && rm -rf /var/lib/apt/lists/*

# Install PlatformIO Core CLI
RUN pip install --no-cache-dir platformio

# Set working directory inside container
WORKDIR /workspace

# Pre-install PlatformIO platforms and packages for esp32dev to speed up builds
RUN pio pkg install --global --platform espressif32

# Default command: build the firmware using production environment
CMD ["pio", "run", "-e", "esp32-prod"]

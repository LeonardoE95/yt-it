#!/usr/bin/env sh

# Downloads ImGui sources to integrate in your program.
# 
# Usage: ./bootstrap.sh

# create folder containing imgui assets 
mkdir -p ./src/imgui

# Copy ImGui assets
rm -rf /tmp/imgui
git clone --depth 1 https://github.com/ocornut/imgui /tmp/imgui
cp /tmp/imgui/*.cpp ./src/imgui
cp /tmp/imgui/*.h ./src/imgui

# Copy ImGui-SFML assets
rm -rf /tmp/imgui-sfml
git clone --depth 1 https://github.com/SFML/imgui-sfml.git /tmp/imgui-sfml
cp /tmp/imgui-sfml/*.cpp  ./src/imgui
cp /tmp/imgui-sfml/*.h  ./src/imgui

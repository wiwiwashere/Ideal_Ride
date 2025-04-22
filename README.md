# Ideal_Ride

## Overview
This project lets you select a subset of rides at a designated theme park and computes an efficient walking route using two MST heuristics (Prim’s & Kruskal’s).  
It also includes a Python script to generate a synthetic dataset of 100,000 “rides” if you just want to test the algorithm.

---

### Installing dependencies

1. nlohmann/json
option 1 : Package: (MSYS2) pacman -S mingw-w64-x86_64-nlohmann-json
option 2 : Download the single header
  Go to https://github.com/nlohmann/json/releases/latest
  Save json.hpp into include/nlohmann/json.hpp in this repo
  CMake will automatically pick it up if you add include/ to your include paths

2. libcurl
MSYS2: installed above via pacman -S mingw-w64-x86_64-curl
Other platforms: install via your package manager, e.g.
  Ubuntu: sudo apt install libcurl4-openssl-dev
  macOS (Homebrew): brew install curl

### Building C++ Backend
```bash
# from repo root
mkdir -p build && cd build

# configure (assuming you have nlohmann/json in your environment)
cmake ..

# build
cmake --build . -- -j$(nproc)
```

### Generate a Large-Scale Dataset (Optional)
If you need ≥100 000 data points:
  change directory to the directory where this project is located in your computer 
```bash
py generate_random.py
```
It should create a file named "random_rides_100000.csv" in the same directory and you could choose to use it directly or add to the existing "Rides.csv" file

### Running 
1. Prepare your CSV in the same folder as your executable
  one way you could do this is drag the "Rides.csv" inside a sub-directory called "data" and drag it to the same folder as your executable
2. Run
```bash
./ride_planner
```
3. Follow the instructions and check out the suggested path!

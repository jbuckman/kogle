set -e
clang++ -O3 -shared -std=c++17 -fPIC $(python3 -m pybind11 --includes) -undefined dynamic_lookup cpp/binding.cpp -o cpp/binding$(python3-config --extension-suffix)

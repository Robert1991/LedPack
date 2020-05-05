Remove-Item ./build -Recurse -Force

mkdir build
cmake -G "MinGW Makefiles" -S . -B ./build

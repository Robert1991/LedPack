param([switch]$test=$true)

Remove-Item ./build -Recurse -Force

mkdir build
cmake -G "MinGW Makefiles" -S . -B ./build

mingw32-make.exe -C build/

if ($test) {
	./build/test/unit_tests.exe
}
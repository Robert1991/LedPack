param([switch]$test = $true)

Remove-Item ./build -Recurse -Force

mkdir build
cmake -G "MinGW Makefiles" -S . -B ./build

mingw32-make.exe -C build/

if ($test) {
	Set-Location ./build/test/
	ctest.exe
}
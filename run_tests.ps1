param([switch]$rebuild=$true)

if ($rebuild) {
	mingw32-make.exe -C build/
}

./build/test/unit_tests.exe
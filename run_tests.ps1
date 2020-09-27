param([switch]$rebuild = $true)

if ($rebuild) {
	mingw32-make.exe -C build/
}

Set-Location ./build/test/
ctest.exe
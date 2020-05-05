param([switch]$verify=$false)

$ARDUINO_IDE_PATH = "C:\Program Files (x86)\Arduino"
$LED_PACK_INO_PATH = "./src/LedPack/LedPack.ino"
$COM_PORT = "COM5"
$BOARD_CONFIG = "arduino:avr:uno"
$BUILD_PATH = "./arduinoBuild"

if ($verify) {
    $command = "verify"
} else {
    $command = "upload"
}

$deploymentProcess = Start-Process -FilePath "$ARDUINO_IDE_PATH\arduino.exe" `
                                   -windowstyle Hidden `
                                   -ArgumentList "--board $BOARD_CONFIG --port $COM_PORT --pref build.path=$BUILD_PATH --$command $LED_PACK_INO_PATH --verbose-build" `
                                   -PassThru -Wait

if ($deploymentProcess.ExitCode -eq 0) {
    Write-Host "Successfully build and deployed led pack"
} elseif ($deploymentProcess.ExitCode -eq 1) {
    Write-Host "Led Pack build failed..."
} elseif ($deploymentProcess.ExitCode -eq 2) {
    Write-Host "Sketch not found!"
} elseif ($deploymentProcess.ExitCode -eq 3) {
    Write-Host "Invalid call!"
}

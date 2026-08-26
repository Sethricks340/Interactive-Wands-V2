from pathlib import Path
import shutil

folder = Path(__file__).parent

source = folder.parent / ".pio" / "build" / "seeed_xiao_esp32s3" / "firmware.bin"
destination = folder / "firmware.bin"
version_file = folder / "version.txt"

# Copy firmware
shutil.copy2(source, destination)

# Increment version
version = int(version_file.read_text().strip()) + 1
version_file.write_text(str(version))

print(f"Firmware copied.")
print(f"Version updated to {version}.")
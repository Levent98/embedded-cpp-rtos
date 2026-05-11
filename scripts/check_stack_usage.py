from pathlib import Path
import sys

MAX_STACK_BYTES = 1024
failed = False

for su_file in Path("build").rglob("*.su"):
    for line in su_file.read_text(errors="ignore").splitlines():
        parts = line.split()
        if len(parts) < 2:
            continue

        try:
            usage = int(parts[1])
        except ValueError:
            continue

        if usage > MAX_STACK_BYTES:
            print(f"STACK LIMIT EXCEEDED: {usage} bytes -> {line}")
            failed = True

if failed:
    sys.exit(1)

print("Stack usage check passed.")
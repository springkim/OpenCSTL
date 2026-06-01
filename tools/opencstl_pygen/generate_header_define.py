import hashlib
import json
import os
import pyperclip


def save_kv(filename: str, k: str, v: str):
    data = None
    if os.path.exists(filename) == False:
        data = {}
    else:
        with open(filename, "r", encoding="utf-8") as f:
            data = json.load(f)
    data[k] = v
    with open(filename, "w", encoding="utf-8") as f:
        json.dump(data, f, ensure_ascii=False, indent=4)


s = "OPENCSTL_VECTOR_H"
digest = hashlib.sha256(s.encode("utf-8")).hexdigest().upper()

header_guard = 'HG_' + digest + '_H'
print(header_guard)

pyperclip.copy(header_guard)
print("Copied to clipboard")

save_kv("header_guard.json", header_guard, s)

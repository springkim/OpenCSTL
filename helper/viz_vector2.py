import os
from tqdm import tqdm

caps, caps2, caps3 = [], [], []
test_size = 100000000

files = {
    "../cmake-build-release-llvm/vector01.txt": caps,
    "../cmake-build-release-llvm/vector02.txt": caps2,
    "../cmake-build-release-llvm/vector03.txt": caps3,
}

for path, buf in files.items():
    with open(path, "rt", encoding="utf-8") as f:
        with tqdm(total=100000000, desc=os.path.basename(path)) as pbar:
            while True:
                line = f.readline()
                if not line:
                    break
                size, capacity = map(int, line.split(':'))
                buf.append(capacity)
                pbar.update(1)
                if len(buf) >= test_size:
                    break

import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
import numpy as np
import math

plt.rcParams.update({
    "font.family":   "DejaVu Sans",
    "font.size":     11,
    "axes.titlesize": 13,
    "axes.labelsize": 11,
    "legend.fontsize": 10,
    "lines.linewidth": 1.8,
})

data = {
    "OpenCSTL": caps,
    "C++ STL":  caps2,
    "M*LIB":    caps3,
}
COLORS = {
    "OpenCSTL": "#2196F3",
    "C++ STL":  "#FF9800",
    "M*LIB":    "#4CAF50",
}

n = min(len(caps), len(caps2), len(caps3))
x = np.logspace(0, np.log10(n), n).astype(int)

fig, ax = plt.subplots(figsize=(11, 6))

for label, y in data.items():
    ax.plot(x, np.array(y[:n]), label=label, color=COLORS[label], linewidth=1.0)

ax.set_xscale("log")
ax.set_yscale("log")

all_vals = [v for y in data.values() for v in y[:n] if v > 0]
exp_min = int(math.floor(math.log10(max(1, min(all_vals)))))
exp_max = int(math.ceil(math.log10(max(all_vals))))
y_ticks = [10**e for e in range(exp_min, exp_max + 1)]
ax.set_yticks(y_ticks)
ax.yaxis.set_major_formatter(
    ticker.FuncFormatter(
        lambda val, pos: f'$10^{{{int(round(math.log10(val)))}}}$' if val > 0 else '0'
    )
)

ax.set_title("Vector Memory Capacity Comparison", fontsize=13, pad=10)
ax.set_xlabel("size")
ax.set_ylabel("capacity")
ax.legend(loc="upper left", framealpha=0.85, edgecolor="#cccccc")
ax.grid(True, which="major", linestyle="--", alpha=0.5)
ax.grid(True, which="minor", linestyle=":",  alpha=0.25)
ax.spines[["top", "right"]].set_visible(False)
ax.xaxis.set_major_formatter(ticker.LogFormatterMathtext())

plt.tight_layout()
plt.show()
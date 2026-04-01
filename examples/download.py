import urllib.request
import os
import sys

def download_file(url: str, output_path: str) -> None:
    print(f"Downloading: {url}")
    print(f"Saving to  : {output_path}")

    def progress(block_num, block_size, total_size):
        if total_size > 0:
            downloaded = block_num * block_size
            percent = min(downloaded / total_size * 100, 100)
            bar = int(percent / 2)
            sys.stdout.write(f"\r[{'?' * bar:<50}] {percent:5.1f}%")
            sys.stdout.flush()

    urllib.request.urlretrieve(url, output_path, reporthook=progress)
    print("\nDone.")

if __name__ == "__main__":
    url = "https://github.com/user-attachments/files/26328104/words_random.txt"
    filename = url.split("/")[-1]
    output_path = os.path.join(os.getcwd(), filename)

    try:
        download_file(url, output_path)
    except Exception as e:
        print(f"\nError: {e}", file=sys.stderr)
        sys.exit(1)

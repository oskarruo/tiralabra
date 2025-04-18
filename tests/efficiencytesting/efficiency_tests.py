import os
import requests
import unidecode
import time
import platform
import subprocess
import re
import matplotlib.pyplot as plt


# The function to fetch books from Project Gutenberg and convert them to ASCII
def fetch_books():
    # Check if the books directory exists, if not, create it
    os.makedirs("books", exist_ok=True)
    urls = {
        1: "https://www.gutenberg.org/cache/epub/61985/pg61985.txt",
        2: "https://www.gutenberg.org/cache/epub/53143/pg53143.txt",
        3: "https://www.gutenberg.org/cache/epub/79/pg79.txt",
        4: "https://www.gutenberg.org/cache/epub/52074/pg52074.txt",
        5: "https://www.gutenberg.org/cache/epub/39/pg39.txt",
    }
    # Iterate through the URLs and download the books if they don't exist
    for i in urls:
        if not os.path.exists(f"books/{i}.txt") and not os.path.exists(
            f"books/{i}_ascii.txt"
        ):
            resp = requests.get(urls[i])
            if resp.status_code == 200:
                with open(f"books/{i}.txt", "wb") as f:
                    f.write(resp.content)
            time.sleep(1)
    # Convert the downloaded books to ASCII format
    for i in urls:
        if not os.path.exists(f"books/{i}_ascii.txt"):
            with open(f"books/{i}.txt", "r", encoding="utf-8") as f:
                text = f.read()
                ascii_text = unidecode.unidecode(text)
                with open(f"books/{i}_ascii.txt", "w", encoding="utf-8") as f:
                    f.write(ascii_text)
            os.remove(f"books/{i}.txt")


# The function to copy the executable from the build directory to the current directory
def copy_executable(executable):
    try:
        executable_path = os.path.join("..", "..", "build", "src", executable)
    except FileNotFoundError:
        raise FileNotFoundError(
            f"Executable not found at {executable_path}. Did you build the project?"
        )

    if platform.system() == "Windows":
        os.system(f'copy "{executable_path}" .')
    else:
        os.system(f'cp "{executable_path}" .')


# Runs the efficiency tests for LZ78 and Huffman compression algorithms and returns the results
def run_efficiency_tests():
    # Check if the executable exists, if not, copy it from the build directory
    executable = "main.exe" if platform.system() == "Windows" else "./main"
    if not os.path.exists(executable):
        copy_executable(executable)

    # Initialize a dictionary to store the results
    compression_results = {"lz78": [], "huffman": []}

    # Iterate through the books and run the compression and decompression tests
    for a in ["lz78", "huffman"]:
        for i in range(1, 6):
            results = []
            input_file = f"books/{i}_ascii.txt"
            compress_cmd = (
                [executable, "-a", "lz78", input_file]
                if a == "lz78"
                else [executable, "-a", "huffman", input_file]
            )

            compress_result = subprocess.run(
                compress_cmd, capture_output=True, text=True
            )
            compression_time_match = re.search(
                r"Compression time: ([\d.]+) seconds", compress_result.stdout
            )
            compression_percent_match = re.search(
                r"Compressed to ([\d.]+)% of original size", compress_result.stdout
            )

            decompress_result = subprocess.run(
                [executable, f"{i}_ascii_compressed.bin"],
                capture_output=True,
                text=True,
            )
            decompression_time_match = re.search(
                r"Decompression time: ([\d.]+) seconds", decompress_result.stdout
            )

            if (
                compression_time_match
                and compression_percent_match
                and decompression_time_match
            ):
                compression_time = float(compression_time_match.group(1))
                compression_percent = float(compression_percent_match.group(1))
                decompression_time = float(decompression_time_match.group(1))
                results.extend(
                    [compression_time, compression_percent, decompression_time]
                )
                compression_results[a].append(results)
                os.remove(f"{i}_ascii_compressed.bin")
                os.remove(f"{i}_ascii_compressed_decompressed.txt")

    return compression_results["lz78"], compression_results["huffman"]


# The function to print the results in a markdown table format
def print_markdown_table(title, results):
    # Print the title and header
    print(f"\n### {title}\n")
    print("| File Number | Compression time | Compressed size | Decompression time |")
    print("|-------------|------------------|------------------|-------------------|")

    # Iterate through the results and print each row
    for i, (compression_time, compression_percent, decompression_time) in enumerate(
        results, 1
    ):
        original_path = f"books/{i}_ascii.txt"
        original_size = os.path.getsize(original_path)
        compressed_size = int(original_size * (compression_percent / 100))
        compressed_size_spaces = f"{compressed_size:,}".replace(",", "\u00a0")
        print(
            f"| {i} | {compression_time:.6f} s | {compressed_size_spaces} bytes ({compression_percent:.4f}% of original) | {decompression_time:.6f} s |"
        )


# The function to plot the results
def plot_results(lz, huffman):
    file_sizes = [os.path.getsize(f"books/{i}_ascii.txt") for i in range(1, 6)]

    lz_compression_times = [result[0] for result in lz]
    huffman_compression_times = [result[0] for result in huffman]

    lz_decompression_times = [result[2] for result in lz]
    huffman_decompression_times = [result[2] for result in huffman]

    lz_percentages = [result[1] for result in lz]
    huffman_percentages = [result[1] for result in huffman]

    plt.figure(figsize=(12, 8))

    # Plot the compression times
    plt.subplot(1, 3, 1)
    plt.plot(file_sizes, lz_compression_times, label="LZ78", marker="o")
    plt.plot(file_sizes, huffman_compression_times, label="Huffman", marker="o")
    plt.title("Compression Time")
    plt.xlabel("Original File Size (bytes)")
    plt.ylabel("Time (s)")
    plt.legend()
    plt.grid(True)

    # Plot the decompression times
    plt.subplot(1, 3, 2)
    plt.plot(file_sizes, lz_decompression_times, label="LZ78", marker="o")
    plt.plot(file_sizes, huffman_decompression_times, label="Huffman", marker="o")
    plt.title("Decompression Time")
    plt.xlabel("Original File Size (bytes)")
    plt.ylabel("Time (s)")
    plt.legend()
    plt.grid(True)

    # Plot the compressed size percentages
    plt.subplot(1, 3, 3)
    plt.plot(file_sizes, lz_percentages, label="LZ78", marker="o")
    plt.plot(file_sizes, huffman_percentages, label="Huffman", marker="o")
    plt.title("Compressed Size (% of Original)")
    plt.xlabel("Original File Size (bytes)")
    plt.ylabel("Size (%)")
    plt.legend()
    plt.grid(True)

    plt.tight_layout()
    plt.show()


# The main function to run the tests and display results
def main():
    fetch_books()
    lz, huffman = run_efficiency_tests()
    print_markdown_table("Results: LZ78", lz)
    print_markdown_table("Results: Huffman", huffman)
    plot_results(lz, huffman)


if __name__ == "__main__":
    main()

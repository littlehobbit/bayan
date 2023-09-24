# bayan 
[![Test](https://github.com/littlehobbit/bayan/actions/workflows/test.yml/badge.svg)](https://github.com/littlehobbit/bayan/actions/workflows/test.yml) [![codecov](https://codecov.io/gh/littlehobbit/bayan/graph/badge.svg?token=54DF2KOSP4)](https://codecov.io/gh/littlehobbit/bayan) 

## Table of Contents
- [Introduction](#introduction)
- [Usage](#usage)
  - [Options](#options)
  - [Examples](#examples)
- [License](#license)

## Introduction
Bayan is a utility for finding duplicate files in a given set of directories. It compares files based on their content, allowing you to identify and manage duplicate files efficiently.

## Usage

### Options
- `-h, --help`: Print the help message and exit.
- `-s, --scan`: List of paths for scanning (at least one required).
- `-i, --ignore`: List of paths to ignore during scanning.
- `-l, --level`: Level of scanning (default is 0).
- `-m, --size`: Minimum size of a file in bytes (default is 1).
- `-f, --filemask`: Mask of files to compare (optional).
- `-b, --block-size`: Read block size (default is 512 bytes).
- `-a, --algorithm`: Hash algorithm for file comparison. SHA256, Crc32 and MD5 can be used (default is md5).

### Examples
1. Scan the current directory for duplicate files using the default options:
   ```
   bayan -s .
   ```

2. Scan multiple directories and specify a minimum file size of 10,000 bytes:
   ```
   bayan -s /path/to/directory1 /path/to/directory2 -m 10000
   ```

3. Ignore specific directories during scanning:
   ```
   bayan -s /path/to/directory -i /path/to/directory/ignore1 /path/to/directory/ignore2
   ```

4. Change the hashing algorithm to SHA-256 for file comparison:
   ```
   bayan -s /path/to/directory -a sha256
   ```

5. Specify a file mask to compare only specific file types (e.g., `\w+.txt`` files):
   ```
   bayan -s /path/to/directory -f "\w+.txt"
   ```

6. Adjust the read block size to 1024 bytes:
   ```
   bayan -s /path/to/directory -b 1024
   ```

7. Perform a deeper scan (increased level of scanning):
   ```
   bayan -s /path/to/directory -l 2
   ```

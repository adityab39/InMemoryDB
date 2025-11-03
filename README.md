# InMemoryDB – A C++-Based Key-Value Store with TTL Support

This is a lightweight, terminal-based key-value store written in modern C++.

## Features

- `SET key value` – Store a key-value pair
- `GET key` – Retrieve a value
- `DEL key` – Delete a key
- `EXPIRE key seconds` – Auto-expire a key after X seconds
- `KEYS` – List all current keys
- `EXIT` – Exit the CLI

## Build Instructions

```bash
mkdir build && cd build
cmake ..
make
./InMemoryDB
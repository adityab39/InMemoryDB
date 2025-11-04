# InMemoryDB – C++ In-Memory Key-Value Store

A lightweight C++-based key-value store that mimics basic Redis functionality. It supports TTL-based auto-expiry, LRU eviction, versioned key history, and CLI command support — all built using C++ STL and multithreading.

---

## Features

- LRU Cache (capacity: 100 keys)
- TTL (Time-To-Live) auto-expiry for keys using `std::priority_queue`
- Versioned Key History – every `SET` stores previous values
- Interactive CLI with simple commands
- Multithreaded TTL watcher using condition variables
- Thread-safe design using `std::mutex`

---

## How It Works

- Keys are stored in an in-memory unordered_map
- When `EXPIRE` is called, TTL is pushed to a min-heap (priority queue)
- A background thread (`ttlWatcher`) removes expired keys based on priority
- A list + hashmap combo maintains Least Recently Used (LRU) eviction
- Every value set is saved in a history map per key

---

## Sample Commands

```bash
SET name Aditya            # Store key-value pair
GET name                   # Retrieve value
EXPIRE name 5              # Auto-delete after 5 seconds
HISTORY name               # View all previous values
DEL name                   # Manually delete
KEYS                       # List all active keys
EXIT                       # Quit program
# lite-memkv

![C++](https://img.shields.io/badge/C++-17-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![Build](https://img.shields.io/badge/Build-Passing-brightgreen?style=for-the-badge)
![License](https://img.shields.io/badge/License-MIT-blue.svg?style=for-the-badge)
![Architecture](https://img.shields.io/badge/Architecture-LRU%20%7C%20AOF%20Engine-purple?style=for-the-badge)

A high-performance, embedded, in-memory Key-Value storage engine written in modern C++17.

Designed for low-latency caching and systems-level memory management, **lite-memkv** couples an O(1) LRU eviction scheme with crash-resilient Append-Only File (AOF) persistence and a Redis-style interactive terminal interface.


A high-performance, embedded, in-memory Key-Value storage engine written in modern C++17. 



Designed for low-latency caching and systems-level memory management, \*\*lite-memkv\*\* couples an O(1) LRU eviction scheme with crash-resilient Append-Only File (AOF) persistence and a Redis-style interactive terminal interface.



\---



\##  Key Highlights



\* \*\*Guaranteed O(1) Operations:\*\* Combines a hash map (`std::unordered\_map`) for fast indexing with an intrusive doubly linked list (`std::list`) to track access recency.

\* \*\*Bounded Memory (LRU Eviction):\*\* Drops least recently accessed records when memory reaches capacity limits, preventing out-of-memory errors.

\* \*\*Crash Resilience (AOF Persistence):\*\* Commits write instructions sequentially to a persistent append-only log (`data.aof`). Engine state auto-reconstructs upon restart.

\* \*\*Thread Safety:\*\* Implements thread synchronization barriers via mutual exclusion locks (`std::mutex`) across all mutate/read pathways.

\* \*\*Standalone REPL Interface:\*\* Integrated CLI prompt mimicking core Redis semantics out of the box with zero third-party dependencies.



\---



\##  System Architecture



```text

&#x20;      \[ Interactive CLI Client / REPL ]

&#x20;                      │

&#x20;                      ▼

&#x20;        \[ Cache Core (LRUCache) ]

&#x20;    ┌───────────────────────────────────┐

&#x20;    │  Hash Index (std::unordered\_map)  │

&#x20;    │                 │                 │

&#x20;    │                 ▼                 │

&#x20;    │  Access Queue (std::list)         │

&#x20;    └─────────────────┬─────────────────┘

&#x20;                      │

&#x20;            ┌─────────┴─────────┐

&#x20;            ▼                   ▼

&#x20;   \[ Fast Memory Read ]   \[ Mutate Operation ]

&#x20;                                │

&#x20;                                ▼

&#x20;                    \[ AOF Persistence Log ]

&#x20;                    (Appends to data.aof)

```


&#x20;CLI Terminal Preview

&#x20; \_ \_ \_                            \_           

&#x20;| (\_) | |\_ \_\_\_ \_\_\_\_\_ \_\_\_ \_\_\_\_\_   | |\_\_\_ \_ \_   

&#x20;| | |  \_| -\_|\_\_\_|     | -\_|     |\_| |\_ / | |  

&#x20;|\_|\_|\_| |\_\_\_|   |\_|\_|\_|\_\_\_|\_|\_|\_|\_\_\_|\_\_\_|\_\_\_| 

&#x20;High-Performance In-Memory Key-Value Engine   



\[+] Engine ready. Restored 0 records from disk.



lite-memkv> SET token secret\_key\_99

OK

lite-memkv> GET token

"secret\_key\_99"

lite-memkv> SIZE

(integer) 1

lite-memkv> KEYS

1\) "token"

lite-memkv> DEL token

(integer) 1

lite-memkv> GET token

(nil)



Build and Execution

Prerequisites

Clang or GCC compiler with C++17 support



POSIX or Windows terminal environment



Build Command (Direct Compilation)



\# Clone the repository

git clone \[https://github.com/dorukhanevren/lite-memkv.git](https://github.com/dorukhanevren/lite-memkv.git)

cd lite-memkv



\# Compile all source files

g++ -std=c++17 -O3 -pthread src/\*.cpp -Iinclude -o lite-memkv



\# Run the binary

./lite-memkv

(On Windows PowerShell / CMD, run lite-memkv.exe after building).



Command Reference



SET <key> <value>: Adds or updates the key with the provided value and writes to AOF.



GET <key>: Fetches the value and moves the key to the head of the LRU queue.



DEL <key>: Deletes the key from both memory and disk sequence.



KEYS: Returns an indexed list of resident keys.



SIZE: Displays the total number of items currently in cache.



FLUSH: Clears all in-memory records and wipes memory state.



EXIT: Safely closes open file handles and exits the process.





&#x20;License

This project is open-source and distributed under the MIT License.










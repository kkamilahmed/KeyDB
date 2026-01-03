# CacheGuard

**CacheGuard** is a Redis-inspired, multi-threaded in-memory key-value store written in **C++**.  
It is designed to explore systems-level concepts such as low-level networking, concurrency, memory management, and security.

The system supports concurrent TCP clients, TTL-based key expiration, LRU eviction, and includes a lightweight Network Intrusion Detection System (NIDS) to mitigate DoS-style attacks.

---

## Features

- **Multi-threaded TCP Server**  
  Handles multiple concurrent clients using the Windows Sockets API and detached worker threads.

- **In-Memory Key-Value Store**  
  Supports core commands: `GET`, `SET`, and `PING`.

- **TTL-Based Key Expiration**  
  Automatically expires keys after a configurable time-to-live.

- **LRU Eviction Policy**  
  Evicts least-recently-used keys when memory limits are reached.

- **DoS Detection & Prevention**  
  Built-in NIDS using IP-based connection tracking and rate limiting.

- **Persistent Storage**  
  Serializes in-memory state to disk for crash recovery and restart persistence.

- **Thread-Safe Design**  
  Uses mutex-protected shared data structures for safe concurrent access.

---

## System Architecture

```mermaid
flowchart TD
    Client[Clients TCP]
    Server[TCP_Server_Listener]
    Handler[Client_Handler_Thread]
    Parser[Parser]
    Executor[Executor]
    Storage[Storage KV_TTL_LRU]
    NIDS[NIDS Rate_Limiting]
    Disk[Disk_Persistence]

    Client --> Server
    Server --> Handler
    Handler --> Parser
    Parser --> Executor
    Executor --> Storage
    Executor --> NIDS
    Storage --> Disk

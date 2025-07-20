# Custom Memory Allocator

A complete memory allocation system implemented in C with assembly-backed heap management. This allocator provides `malloc()`/`free()` functionality with custom header management and block reusing capabilities.

## Project Overview

This project implements a fully functional memory allocator that manages a 128MB heap space. It features custom memory headers, efficient block finding algorithms, memory reusing, and comprehensive debugging tools.

## ✨ Features

- **✅ Complete Allocation System**: Both allocation (`alloc()`) and deallocation (`free_it()`)
- **✅ Memory Block Reusing**: Freed memory can be allocated again efficiently  
- **✅ Assembly-Backed Heap**: 128MB heap space defined in assembly for precise control
- **✅ Packed Headers**: 32-bit headers with 30-bit size field and allocation flags
- **✅ Memory Layout Visualization**: `show()` function displays current heap state
- **✅ Error Handling**: Built-in error codes for memory exhaustion and double-free detection
- **✅ Type Safety**: Custom type definitions for different word sizes
- **✅ Debug Output**: Detailed logging of allocation and deallocation operations

## Architecture

### Memory Layout
```
Heap: [Header1][Data1][Header2][Data2][Header3][Data3]...
      ^4 bytes ^allocated   ^4 bytes ^allocated
```

### Header Structure (32-bit packed)
```c
struct s_header {
    word w: 30;           // Size in words (30 bits)
    bool alloced: 1;      // Allocation status (1 bit)  
    bool reserved: 1;     // Reserved for future use (1 bit)
};
```

### Example Output
```
Memspace: 0xd4b87030
--- After all allocations ---
Alloc 1 = 2 alloced words
Alloc 2 = 500 alloced words  
Alloc 3 = 1 alloced words
--- After freeing x2 ---
Alloc 1 = 2 alloced words
Alloc 2 = 500 free words      ← Available for reuse
Alloc 3 = 1 alloced words
--- After allocating x4 ---
REUSING: Block of 500 words for 390 word allocation (wasting 110 words)
Alloc 1 = 2 alloced words
Alloc 2 = 500 alloced words   ← Reused block
Alloc 3 = 1 alloced words
```

## File Structure

| File | Description |
|------|-------------|
| `alloc.h` | Header with type definitions, structures, and function prototypes |
| `alloc.c` | Main allocator implementation with allocation/deallocation logic |
| `heap.asm` | Assembly file defining 128MB heap space |
| `Makefile` | Build configuration |

## Building

### Prerequisites
- **GCC compiler** (with 64-bit support)
- **NASM assembler** (for assembly file)
- **Make** (optional)

### Quick Build
```bash
# Using Makefile
make

# Manual compilation
nasm -f elf64 heap.asm -o heap.o
gcc -Wall -O2 -std=c17 alloc.c heap.o -o alloc
```

### Windows (MinGW)
```bash
nasm -f win64 heap.asm -o heap.o
gcc -Wall -O2 -std=c17 alloc.c heap.o -o alloc.exe
```

## Usage

### Basic Example
```c
#include "alloc.h"

int main() {
    // Allocate memory
    int8 *ptr1 = alloc(100);    // 100 bytes
    int8 *ptr2 = alloc(2000);   // 2KB
    
    // View memory layout
    show();
    
    // Free memory
    free_it(ptr2);
    
    // Allocate again (will reuse freed space)
    int8 *ptr3 = alloc(1500);
    
    show();  // See the reused block
    
    return 0;
}
```

### Convenience Macros
```c
void *small = alloc(50);           // 50 bytes
void *big = alloc_k(4);           // 4KB
void *huge = alloc_m(10);         // 10MB
```

## Core Functions

### Memory Management
```c
void *alloc(int32 bytes);         // Allocate memory
bool free_it(void *address);      // Free allocated memory
```

### Debugging & Visualization  
```c
show();                           // Display heap layout
header *findBlock(word words);    // Find available block
```

## How It Works

### Allocation Process
1. **Convert bytes to words** (4-byte alignment)
2. **Find available block** using recursive search
3. **Mark block as allocated** and return user pointer
4. **User gets pointer** to memory after 4-byte header

### Deallocation Process
1. **Calculate header location** (user_ptr - 4 bytes)
2. **Validate block** (check for double-free)
3. **Zero out user data** for security
4. **Mark block as free** for future reuse

### Block Reusing Strategy
- When allocating, **freed blocks are prioritized**
- **Simple reuse**: Use entire freed block even if oversized
- **Trade-off**: Wastes some space but maintains memory structure integrity

## Configuration

### Memory Limits
```c
#define Maxwords ((1024*1024*1024/4)-1)  // ~1GB max allocation
```

### Error Codes
```c
#define ErrorNoError    0    // Success
#define ErrorNoMemory   1    // Insufficient memory  
#define ErrorUnknown    2    // Unknown error
#define ErrorDoubleFree 4    // Attempted double-free
```

### Heap Size
```asm
%define Heapsize (1024 * 1024 * 1024 / 8)  ; 128MB heap
```

## Testing

The included `main()` function demonstrates:
- **Multiple allocations** of different sizes
- **Memory deallocation** and verification
- **Block reusing** behavior
- **Memory layout visualization**

Run with:
```bash
./alloc
```

## Current Limitations

- **Simple Reuse Strategy**: Doesn't split oversized blocks (wastes space)
- **Linear Search**: O(n) complexity for block finding
- **No Coalescing**: Adjacent free blocks aren't merged
- **Fixed Heap**: 128MB compile-time constant

## Future Enhancements

- [ ] **Block Splitting**: Cut oversized blocks to exact size needed
- [ ] **Block Coalescing**: Merge adjacent free blocks automatically
- [ ] **Free List**: Maintain linked list of free blocks for faster allocation
- [ ] **Best-Fit Algorithm**: Find smallest suitable block instead of first-fit
- [ ] **Heap Growth**: Dynamic heap expansion when needed
- [ ] **Memory Debugging**: Leak detection and corruption checking

## Key Achievements

This allocator successfully demonstrates:
- ✅ **Complete memory management lifecycle**
- ✅ **Efficient header-based block tracking** 
- ✅ **Memory reuse without fragmentation issues**
- ✅ **Robust error handling and validation**
- ✅ **Clean separation of concerns** (assembly heap, C logic)

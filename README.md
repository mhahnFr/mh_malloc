# Welcome to **mh_malloc**!
This library provides an implementation of the four functions `malloc`, `realloc`,
`calloc` and `free`.

## How it works
Internally, three zones are created:
1. for small allocations
2. for medium allocations
3. for larger allocations.

Each allocation zone is optimized for its purpose.

### Small zone
The small zone uses a fixed allocation size.

### Medium zone
The medium zone creates block of memory and splits them into smaller chunks as
needed. When a chunk is given back to the allocator, the chunk is appended to
the block it came from, if possible.

### Large zone
The large zone simply requests a sufficient number of bytes from the system.

## How to use
You can either download a compiled version [here][2].  
Alternatively, you can build the library yourself:
- Clone the repository: `git clone https://github.com/mhahnFr/mh_malloc.git`
- and compile it: `cd mh_malloc && make`

The Makefile comes with an integrated benchmark, run it with `make bench`.

Then, you can simply link your code with the emitted library.

© Copyright 2023 [mhahnFr][1]

[1]: https://github.com/mhahnFr
[2]: https://github.com/mhahnFr/mh_malloc/releases

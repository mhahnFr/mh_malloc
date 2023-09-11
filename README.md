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

### Why?
This project has been created since the [LeakSanitizer][2] needed its own allocator.

© Copyright 2023 [mhahnFr][1]

[1]: https://github.com/mhahnFr
[2]: https://github.com/mhahnFr/LeakSanitizer

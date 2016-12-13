# mallocator

## Overview

mallocator is a simple program which allocates a configurable amount of memory and then goes into a sleep loop.  The main usage is for RAM stress testing.

kmallocator is a kernel module that does the same.

## Usage

`mallocator TOTAL_ALLOC_SIZE [CHUNK_SIZE=50MiB] [USEC_SLEEP_BETWEEN_CHUNKS=1000000]`

`sudo insmod kmallocator.ko total_alloc_size=SIZE_IN_MiB chunk_size=SIZE_IN_MiB millisecond_delay=MILLISECONDS`

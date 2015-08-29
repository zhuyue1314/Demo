# afl fuzz server

AFL is american fuzzy loop.

The original fuzzer always use execv() to invoke the target program
which wastes a lot of time. This demo use a forkserver which uses
fork() instead of execv(). It more faster.

## How to use this demo

```
$ cd afl_fuzz_server
$ make
$ ./fuzzer
```

# This is a demo project

* Inherited class can inherit the static methods from base class
* fork()ed process could not change the static variable of parent class

## 1. How to use it

```
$ make

$ ./static_inherited_class
===================================
	This is a demo project
===================================
BaseClass::print()

****: Inherited class can inherit the static method of base class

BaseClass::print()

$ ./static_fork
===================================
	This is a demo project
===================================
[c]		This is child process: 9666
[c]		Child process is changing the counter
[c]		counter= 111
[p]	This is parent process: 9665
[p]	counter=0

***: fork()ed process could not change the static variable of parent process

```

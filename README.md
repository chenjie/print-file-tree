# print-file-tree
A simple C program that builds a tree of a given path of the file system. It also prints a user-friendly string representation including permission info to standard output.  

## Note
hash_functions.c and hash.h are not necessarily required here. Their job is to store the hash of each file to its TreeNode, which is used to building a file tree.

## Getting Started

### Prerequisites

* GCC
* Terminal (in Unix) OR PowerShell (in Windows)

### Download source code and compile
The following instructions are presented using Terminal in macOS:
```
# Change to HOME directory
$ cd ~

# Clone this repo and 'cd' into it
$ git clone https://github.com/jellycsc/print-file-tree.git
$ cd print-file-tree/

# Compile source code
$ make
gcc -Wall -std=gnu99 -c print_ftree.c
gcc -Wall -std=gnu99 -c ftree.c
gcc -Wall -std=gnu99 -c hash_functions.c
gcc -Wall -std=gnu99 -o print_ftree print_ftree.o ftree.o hash_functions.o
```

### Usage
```
Usage: print_ftree DIRECTORY_NAME
```

### Examples
Current directory (of depth 1):
```
$ ./print_ftree .
===== . (755) =====
  ftree.c (644)
  print_ftree.o (644)
  Makefile (644)
  hash_functions.o (644)
  README.md (644)
  print_ftree (755)
  ftree.o (644)
  ftree.h (644)
  print_ftree.c (644)
  hash_functions.c (644)
  hash.h (644)
```
A deeper directory of a Java project:
```
$ ./print_ftree PhotoRenamer/
===== PhotoRenamer/ (755) =====
  ===== bin (755) =====
    ===== photoRenamer (755) =====
      PhotoRenamer$ButtonListener.class (644)
      PhotoRenamer.class (644)
      PhotoRenamer$JListListener.class (644)
      Image.class (644)
      PhotoRenamer$keyListener.class (644)
      ImageFilter.class (644)
    ===== unitTest (755) =====
      TagManagerTest.class (644)
      ImageManagerTest.class (644)
      ImageTest.class (644)
    ===== managers (755) =====
      MyLogingSystem$MyLogingSystemHander.class (644)
      TagManager.class (644)
      MyLogingSystem.class (644)
      ImageManager.class (644)
  ===== src (755) =====
    ===== photoRenamer (755) =====
      Image.java (755)
      ImageFilter.java (755)
      PhotoRenamer.java (755)
    ===== unitTest (755) =====
      TagManagerTest.java (755)
      ImageTest.java (755)
      ImageManagerTest.java (755)
    ===== managers (755) =====
      ImageManager.java (644)
      MyLogingSystem.java (644)
      TagManager.java (644)
```
You can see the indentations help to present the file structure. 

## Authors

| Name             | GitHub                                     | Email
| ---------------- | ------------------------------------------ | -------------------------
| Chenjie Ni       | [jellycsc](https://github.com/jellycsc)    | nichenjie2013@gmail.com

## Thoughts and future improvements

* This program will encounter an infinite loop when there are cycles (created by soft links) under the specified path. It takes some extra effort to deal with that case.

## Contributing to this project

1. Fork it ( https://github.com/jellycsc/print-file-tree/fork )
2. Create your feature branch (`git checkout -b my-new-feature`)
3. Commit your changes (`git commit -m 'Add some feature'`)
4. Push to your feature branch (`git push origin my-new-feature`)
5. Create a new Pull Request

Details are described [here](https://git-scm.com/book/en/v2/GitHub-Contributing-to-a-Project).

## Bug Reporting
Please log bugs under [Issues](https://github.com/jellycsc/print-file-tree/issues) tab on Github.  
OR you can shoot an email to <nichenjie2013@gmail.com>

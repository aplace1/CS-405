# CS-405
Modified projects from Secure-Coding


## XOR Encryption Artifact:

### Purpose:
Encrypts a file using a simple XOR

Produces two files:
- encrypted.txt
- decrypted.txt


### Sample compilation, execution, and output:

Compiling with:

`g++ -std=c++2a ./Encryption.cpp -o Encryption`

or

`clang++-12 -std=c++20 ./Encryption.cpp -o Encryption`

Execution and Output:

```
$ ./Encryption "TEST"

Read File: input.txt
        - Encrypted To: encrypted.txt
        - Decrypted To: decrypted.txt
```

Sample output from `decrypted.txt `

```
Header:	HEADER_NAME
Date:	Fri Oct 15 03:16:11 2021
Key:	TEST
========= BEGIN DATA =========
HEADER_NAME
"DATA":{
 Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod
 tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam,
 quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo
 consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse
 cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non
 proident, sunt in culpa qui officia deserunt mollit anim id est laborum.
}
=========  END DATA  =========
```

### NOTE:
The `encrypted.txt` file will likely contain unreadable characters.

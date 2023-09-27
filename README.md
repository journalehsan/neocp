# Introduction
This project is an improved version of the standard cp command. It adds the following new features:

Support for copying compressed files: This project can copy files that are compressed using gzip, bzip2, or zip.
Support for copying files with long names: This project can copy files with names that are longer than 255 characters.
Support for copying files with special characters: This project can copy files that contain special characters, such as spaces, tabs, or newlines.
This project is still under development and may contain bugs. Please create a new issue if you find any bugs.

Version: 1.0

Author: Ehsan Tork

GitHub link: https://github.com/journalehsan/neocp

Thank you,
Ehsan Tork
## Installation

To install and compile your project in Linux, follow these steps:

Clone the project code from GitHub:
``` git clone https://github.com/journalehsan/neocp.git ```
Navigate to the project directory:
cd neocp
Compile the project:
``` gcc -o neocp neocp.c ```
OR

clang -o neocp neocp.c
Change the output to the name neocp with the -o switch:
‍‍‍``` gcc -o neocp -o neocp neocp.c ```
OR

clang -o neocp -o neocp neocp.c
Copy the neocp file to the system's bin directory:
``` sudo cp neocp /usr/bin/ ```
Now you can use neocp as a regular command:

neocp source_file destination_file
For example:

``` neocp README.md ~/Desktop/README.md ```
This command will copy the README.md file to the Desktop directory.

Explanation:

The -o switch tells the compiler to save the output with the specified name.
The sudo switch allows you to copy files to the system's bin directory.
I hope this helps!

/* 
This code copy files and directories via cp command but add progress bar for more information
COPYRIGHT (C) 2023 Ehsan Tork
All rights reserved.
mail: journalehsan@gmail.com
GITHUB REPO: https://github.com/journalehsan/neocp
This software is free software; you can redistribute it and/or
modify it under the terms of the MIT License
MIT License (MIT)
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <termios.h>
#include <sys/select.h>

#define MAX_PATH 1024
#define MAX_BUF 1024
#define MAX_FILE 1024
#define MAX_DIR 1024
#define MAX_NAME 1024
#define MAX_SIZE 1024
#define MAX_TIME 1024
#define MAX_MODE 1024
#define MAX_UID 1024
#define MAX_GID 1024
#define MAX_LINK 1024
#define MAX_BLOCK 1024
#define MAX_INODE 1024
#define MAX_DEV 1024
#define MAX_NLINK 1024
#define MAX_BLKSIZE 1024


#define MAX(a,b) ((a)>(b)?(a):(b))

// global variable
int g_total_size = 0;
int g_total_file = 0;
int g_total_dir = 0;
int g_total_link = 0;
int g_total_block = 0;
int g_total_inode = 0;
int g_total_dev = 0;
int g_total_nlink = 0;
int g_total_blksize = 0;

// function prototype
void print_usage();
void print_error(char *msg);
void print_error_and_exit(char *msg);

void print_progress_bar(int cur, int total);

void copy_file(char *src, char *dst);
void copy_dir(char *src, char *dst);
void copy_link(char *src, char *dst);

void get_file_info(char *path, char *name, char *size, char *time, char *mode, char *uid, char *gid, char *link, char *block, char *inode, char *dev, char *nlink, char *blksize);
void get_dir_info(char *path, char *name, char *size, char *time, char *mode, char *uid, char *gid, char *link, char *block, char *inode, char *dev, char *nlink, char *blksize);
void get_link_info(char *path, char *name, char *size, char *time, char *mode, char *uid, char *gid, char *link, char *block, char *inode, char *dev, char *nlink, char *blksize);

void print_file_info(char *name, char *size, char *time, char *mode, char *uid, char *gid, char *link, char *block, char *inode, char *dev, char *nlink, char *blksize);
void print_dir_info(char *name, char *size, char *time, char *mode, char *uid, char *gid, char *link, char *block, char *inode, char *dev, char *nlink, char *blksize);
void print_link_info(char *name, char *size, char *time, char *mode, char *uid, char *gid, char *link, char *block, char *inode, char *dev, char *nlink, char *blksize);

void print_file_info_with_progress(char *name, char *size, char *time, char *mode, char *uid, char *gid, char *link, char *block, char *inode, char *dev, char *nlink, char *blksize);
void print_dir_info_with_progress(char *name, char *size, char *time, char *mode, char *uid, char *gid, char *link, char *block, char *inode, char *dev, char *nlink, char *blksize);
void print_link_info_with_progress(char *name, char *size, char *time, char *mode, char *uid, char *gid, char *link, char *block, char *inode, char *dev, char *nlink, char *blksize);


// main function
int main(int argc, char *argv[])
{
	
    // help option
    if(strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)
    {
        print_usage();
        exit(0);
    }
    //version option
     if(strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0)
    {
        printf("neocp version 1.0\n");
        //add mit license author ehsan tork journalehsa@gmail.com
       
         printf("This is free software: you are free to change and redistribute it.\n");
        printf("There is NO WARRANTY, to the extent permitted by law.\n");
        printf("Written by Ehsan Tork, journalehsan@gmail.com\n");
         // MIT License
         printf("The MIT License (MIT)\n");
        printf("\n");
        printf("Permission is hereby granted, free of charge, to any person obtaining a copy\n");
        printf("of this software and associated documentation files (the \"Software\"), to deal\n");
        printf("in the Software without restriction, including without limitation the rights\n");
        printf("to use, copy, modify, merge, publish, distribute, sublicense, and/or sell\n");
        printf("copies of the Software, and to permit persons to whom the Software is\n");
        printf("furnished to do so, subject to the following conditions:\n");
        printf("\n");
        printf("The above copyright notice and this permission notice shall be included in\n");
        printf("all copies or substantial portions of the Software.\n");
        printf("\n");
        printf("THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR\n");
        printf("IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,\n");
        printf("FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE\n");
        printf("AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER\n");
        printf("LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,\n");
        printf("OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE\n");
        printf("SOFTWARE.\n");
        printf("\n");
        exit(0);
    }

    // check argument
	if(argc != 3)
	{
		print_usage();
		exit(1);
	}

	// check source file or directory
	char src[MAX_PATH];
	strcpy(src, argv[1]);
	struct stat src_stat;
	if(lstat(src, &src_stat) == -1)
	{
		print_error_and_exit("lstat() error 5");
	}
	if(!S_ISREG(src_stat.st_mode) && !S_ISDIR(src_stat.st_mode) && !S_ISLNK(src_stat.st_mode))
	{
		print_error_and_exit("source is not file or directory");
	}

	// check destination file or directory
	char dst[MAX_PATH];
	strcpy(dst, argv[2]);
	struct stat dst_stat;
	if(lstat(dst, &dst_stat) == -1)
	{
        //touch dst
        int fd = open(dst, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if(errno != ENOENT)
        {
            print_error_and_exit("lstat() error 6");
        }
	}
	if(S_ISDIR(src_stat.st_mode) && !S_ISDIR(dst_stat.st_mode))
	{
		//if directory is exists ask for owerwrite otherwise mkdir new one
        char answer[10];
        printf("directory is exists do you want to overwrite it? (y/n): ");
        scanf("%s", answer);
        if(strcmp(answer, "y") == 0)
        {
            // remove file
            if(remove(dst) == -1)
            {
                print_error_and_exit("overwite error");
            }
        }
        else
        {
            // make directory
            if(mkdir(dst, 0755) == -1)
            {
                print_error_and_exit("directory is already exists.");
            }
        }
	}
	if(!S_ISDIR(src_stat.st_mode) && S_ISDIR(dst_stat.st_mode))
	{
		sprintf(dst, "%s/%s", dst, src);
	}

	// copy
	if(S_ISREG(src_stat.st_mode))
	{
		copy_file(src, dst);
	}
	else if(S_ISDIR(src_stat.st_mode))
	{
		copy_dir(src, dst);
	}
	else if(S_ISLNK(src_stat.st_mode))
	{
		copy_link(src, dst);
	}

	return 0;
}

// function implementation
void print_usage()
{
	printf("Usage: neocp <source> <destination>\n");
}

void print_error(char *msg)
{
	fprintf(stderr, "%s\n", msg);
}

void print_error_and_exit(char *msg)
{
	fprintf(stderr, "%s\n", msg);
	exit(1);
}

void print_progress_bar(int cur, int total)
{
	//better prgressbar in case of terminal size and modern ui
    //progressbar looklike python pip progressbar
    //get terminal size
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    int width = w.ws_col;
    //calculate progressbar size
    int progressbar_size = width - 20;
    //calculate progressbar percent
    int percent = (cur * 100) / total;
    //calculate progressbar fill size
    int fill_size = (cur * progressbar_size) / total;
    //calculate progressbar empty size
    int empty_size = progressbar_size - fill_size;
    //print progressbar
    printf("\r[");
    for(int i = 0; i < fill_size; i++)
    {
        printf("=");
    }
    for(int i = 0; i < empty_size; i++)
    {
        printf(" ");
    }
    printf("] %d%%", percent);
    fflush(stdout);
    
}

// copy_file
void copy_file(char *src, char *dst)
{
    // get file info
    char name[MAX_NAME];
    char size[MAX_SIZE];
    char time[MAX_TIME];
    char mode[MAX_MODE];
    char uid[MAX_UID];
    char gid[MAX_GID];
    char link[MAX_LINK];
    char block[MAX_BLOCK];
    char inode[MAX_INODE];
    char dev[MAX_DEV];
    char nlink[MAX_NLINK];
    char blksize[MAX_BLKSIZE];
    get_file_info(src, name, size, time, mode, uid, gid, link, block, inode, dev, nlink, blksize);

    // print file info
    print_file_info(name, size, time, mode, uid, gid, link, block, inode, dev, nlink, blksize);

    // open source file
    int src_fd = open(src, O_RDONLY);
    if(src_fd == -1)
    {
        print_error_and_exit("open() error this");
    }

    // open destination file
    int dst_fd = open(dst, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if(dst_fd == -1)
    {
        //TODO: this is not work correctly because of dst is directory and we want to copy file to it so we should append file name to dst and open again with another method
        //pritn destination path
        printf("destination path is this %s\n",dst);
       //appned source file name to destination path and open again
        char *src_file_name;
        char *new_dst;
        src_file_name = strrchr(src, '/');
        new_dst = strcat(dst,src_file_name);
        dst_fd = open(new_dst, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if(dst_fd == -1)
        {
            print_error_and_exit("open() error");
        }
    }

    // copy
    char buf[MAX_BUF];
    int read_size;
    int write_size;
    while((read_size = read(src_fd, buf, MAX_BUF)) > 0)
    {
        write_size = write(dst_fd, buf, read_size);
        if(write_size == -1)
        {
            print_error_and_exit("write() error");
        }
        g_total_size += write_size;
        print_progress_bar(g_total_size, atoi(size));
    }
    printf("\n");

    // close file
    close(src_fd);
    close(dst_fd);
}

//copy_dir
void copy_dir(char *src, char *dst)
{
    // get directory info
    char name[MAX_NAME];
    char size[MAX_SIZE];
    char time[MAX_TIME];
    char mode[MAX_MODE];
    char uid[MAX_UID];
    char gid[MAX_GID];
    char link[MAX_LINK];
    char block[MAX_BLOCK];
    char inode[MAX_INODE];
    char dev[MAX_DEV];
    char nlink[MAX_NLINK];
    char blksize[MAX_BLKSIZE];
    get_dir_info(src, name, size, time, mode, uid, gid, link, block, inode, dev, nlink, blksize);

    // print directory info
    print_dir_info(name, size, time, mode, uid, gid, link, block, inode, dev, nlink, blksize);

    // make directory
    if(mkdir(dst, 0755) == -1)
    {
        //append source directory name to destinition path and mkdir again
        char *src_dir_name;
        char *new_dst;
        src_dir_name = strrchr(src, '/');
        new_dst = strcat(dst,src_dir_name);
        if(mkdir(new_dst, 0755) == -1)
        {
            print_error_and_exit("mkdir() error");
        }
    }

    // open directory
    DIR *dir = opendir(src);
    if(dir == NULL)
    {
        print_error_and_exit("opendir() error");
    }

    // read directory
    struct dirent *entry;
    char src_path[MAX_PATH];
    char dst_path[MAX_PATH];
    while((entry = readdir(dir)) != NULL)
    {
        // ignore . and ..
        if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }

        // make path
        sprintf(src_path, "%s/%s", src, entry->d_name);
        sprintf(dst_path, "%s/%s", dst, entry->d_name);

        // get file info
        struct stat stat;
        //print source path
        printf("source path is this %s\n",src_path);
        if(lstat(src_path, &stat) == -1)
        {
            print_error_and_exit("lstat() error 1");
        }

        // copy
        if(S_ISREG(stat.st_mode))
        {
            copy_file(src_path, dst_path);
            g_total_file++;
        }
        else if(S_ISDIR(stat.st_mode))
        {
            copy_dir(src_path, dst_path);
            g_total_dir++;
        }
        else if(S_ISLNK(stat.st_mode))
        {
            copy_link(src_path, dst_path);
            g_total_link++;
        }
    }
    closedir(dir);
}

// copy_link
void copy_link(char *src, char *dst)
{
    // get link info
    char name[MAX_NAME];
    char size[MAX_SIZE];
    char time[MAX_TIME];
    char mode[MAX_MODE];
    char uid[MAX_UID];
    char gid[MAX_GID];
    char link[MAX_LINK];
    char block[MAX_BLOCK];
    char inode[MAX_INODE];
    char dev[MAX_DEV];
    char nlink[MAX_NLINK];
    char blksize[MAX_BLKSIZE];
    get_link_info(src, name, size, time, mode, uid, gid, link, block, inode, dev, nlink, blksize);

    // print link info
    print_link_info(name, size, time, mode, uid, gid, link, block, inode, dev, nlink, blksize);

    // read link
    char buf[MAX_BUF];
    int read_size = readlink(src, buf, MAX_BUF);
    if(read_size == -1)
    {
        print_error_and_exit("readlink() error");
    }
    buf[read_size] = 0;

    // make link
    if(symlink(buf, dst) == -1)
    {
        print_error_and_exit("symlink() error");
    }
}

//print_dir_info
void print_dir_info(char *name, char *size, char *time, char *mode, char *uid, char *gid, char *link, char *block, char *inode, char *dev, char *nlink, char *blksize)
{
    printf("%s\n", name);
}

//print_file_info
void print_file_info(char *name, char *size, char *time, char *mode, char *uid, char *gid, char *link, char *block, char *inode, char *dev, char *nlink, char *blksize)
{
    printf("%s\n", name);
}

//print_link_info
void print_link_info(char *name, char *size, char *time, char *mode, char *uid, char *gid, char *link, char *block, char *inode, char *dev, char *nlink, char *blksize)
{
    printf("%s\n", name);
}

//get_dir_info
void get_dir_info(char *path, char *name, char *size, char *time, char *mode, char *uid, char *gid, char *link, char *block, char *inode, char *dev, char *nlink, char *blksize)
{
    // get file info
    struct stat stat;
    if(lstat(path, &stat) == -1)
    {
        print_error_and_exit("lstat() error 2");
    }

    // get name
    strcpy(name, path);

    // get size
    sprintf(size, "%d", stat.st_size);

    // get time
    struct tm *tm = localtime(&stat.st_mtime);
    strftime(time, MAX_TIME, "%Y-%m-%d %H:%M:%S", tm);

    // get mode
    sprintf(mode, "%o", stat.st_mode);

    // get uid
    sprintf(uid, "%d", stat.st_uid);

    // get gid
    sprintf(gid, "%d", stat.st_gid);

    // get link
    sprintf(link, "%d", stat.st_nlink);

    // get block
    sprintf(block, "%d", stat.st_blocks);

    // get inode
    sprintf(inode, "%d", stat.st_ino);

    // get dev
    sprintf(dev, "%d", stat.st_dev);

    // get nlink
    sprintf(nlink, "%d", stat.st_nlink);

    // get blksize
    sprintf(blksize, "%d", stat.st_blksize);
}

//get_file_info
void get_file_info(char *path, char *name, char *size, char *time, char *mode, char *uid, char *gid, char *link, char *block, char *inode, char *dev, char *nlink, char *blksize)
{
    // get file info
    struct stat stat;
    if(lstat(path, &stat) == -1)
    {
        print_error_and_exit("lstat() error 3");
    }

    // get name
    strcpy(name, path);

    // get size
    sprintf(size, "%d", stat.st_size);

    // get time
    struct tm *tm = localtime(&stat.st_mtime);
    strftime(time, MAX_TIME, "%Y-%m-%d %H:%M:%S", tm);

    // get mode
    sprintf(mode, "%o", stat.st_mode);

    // get uid
    sprintf(uid, "%d", stat.st_uid);

    // get gid
    sprintf(gid, "%d", stat.st_gid);

    // get link
    sprintf(link, "%d", stat.st_nlink);

    // get block
    sprintf(block, "%d", stat.st_blocks);

    // get inode
    sprintf(inode, "%d", stat.st_ino);

    // get dev
    sprintf(dev, "%d", stat.st_dev);

    // get nlink
    sprintf(nlink, "%d", stat.st_nlink);

    // get blksize
    sprintf(blksize, "%d", stat.st_blksize);
}

//get_link_info
void get_link_info(char *path, char *name, char *size, char *time, char *mode, char *uid, char *gid, char *link, char *block, char *inode, char *dev, char *nlink, char *blksize)
{
    // get file info
    struct stat stat;
    if(lstat(path, &stat) == -1)
    {
        print_error_and_exit("lstat() error 4");
    }

    // get name
    strcpy(name, path);

    // get size
    sprintf(size, "%d", stat.st_size);

    // get time
    struct tm *tm = localtime(&stat.st_mtime);
    strftime(time, MAX_TIME, "%Y-%m-%d %H:%M:%S", tm);

    // get mode
    sprintf(mode, "%o", stat.st_mode);

    // get uid
    sprintf(uid, "%d", stat.st_uid);

    // get gid
    sprintf(gid, "%d", stat.st_gid);

    // get link
    sprintf(link, "%d", stat.st_nlink);

    // get block
    sprintf(block, "%d", stat.st_blocks);

    // get inode
    sprintf(inode, "%d", stat.st_ino);

    // get dev
    sprintf(dev, "%d", stat.st_dev);

    // get nlink
    sprintf(nlink, "%d", stat.st_nlink);

    // get blksize
    sprintf(blksize, "%d", stat.st_blksize);
}

//print_dir_info_with_progress
void print_dir_info_with_progress(char *name, char *size, char *time, char *mode, char *uid, char *gid, char *link, char *block, char *inode, char *dev, char *nlink, char *blksize)
{
    printf("%s\n", name);
}

//print_file_info_with_progress
void print_file_info_with_progress(char *name, char *size, char *time, char *mode, char *uid, char *gid, char *link, char *block, char *inode, char *dev, char *nlink, char *blksize)
{
    printf("%s\n", name);
}

//print_link_info_with_progress
void print_link_info_with_progress(char *name, char *size, char *time, char *mode, char *uid, char *gid, char *link, char *block, char *inode, char *dev, char *nlink, char *blksize)
{
    printf("%s\n", name);
}


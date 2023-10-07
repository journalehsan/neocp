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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <time.h>
#include <math.h>
#include <sys/time.h>


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
        printf("neocp version 1.1\n");
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
		//if exists ask for overwrite otherwise show message
        char answer[10];
        printf("source is exists do you want to overwrite it? (y/n): ");
        scanf("%s", answer);
        if(strcmp(answer, "y") == 0)
        {
            // remove file
            if(remove(src) == -1)
            {
                print_error_and_exit("overwite error");
            }
        }
        else
        {
            // show message
            print_error_and_exit("source is already exists.");
        }
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

/* Specify how wide the progress bar should be. */
#define PROGRESS_BAR_WIDTH 50

/*
 * Alternative progress bar where each block grows
 * vertically instead of horizontally.
 */
/* #define VERTICAL */

/* Various unicode character definitions. */
#define BAR_START "\u2595"
#define BAR_STOP  "\u258F"
#define PROGRESS_BLOCK     "\u2588"

#ifdef VERTICAL
static const char * subprogress_blocks[] = { " ",
                                             "\u2581",
                                             "\u2582",
                                             "\u2583",
                                             "\u2584",
                                             "\u2585",
                                             "\u2586",
                                             "\u2587"
};
#else
static const char * subprogress_blocks[] = { " ",
                                             "\u258F",
                                             "\u258E",
                                             "\u258D",
                                             "\u258C",
                                             "\u258B",
                                             "\u258A",
                                             "\u2589"
};
#endif

#define NUM_SUBBLOCKS (sizeof(subprogress_blocks) / sizeof(subprogress_blocks[0]))

/* Helper function to get the current time in usecs past the epoch. */
static uint64_t get_timestamp(void) {
    struct timeval tv;
    uint64_t stamp = 0;
    gettimeofday(&tv, NULL);
    stamp = tv.tv_sec * 1000000 + tv.tv_usec;
    return stamp;
}

/* Helper function to print a usecs value as a duration. */
static void print_timedelta(uint64_t delta) {

    uint64_t delta_secs = delta / 1000000;
    uint64_t hours    = delta_secs / 3600;
    uint64_t minutes  = (delta_secs - hours * 3600) / 60;
    uint64_t seconds  = (delta_secs - hours * 3600 - minutes * 60);
    uint64_t mseconds = (delta / 100000) % 10;

    if (hours) {
        printf("%lluh %llum %llus    ", hours, minutes, seconds);
    }
    else if (minutes) {
        printf("%llum %02llus        ", minutes, seconds);
    }
    else {
        printf("%llu.%llus           ", seconds, mseconds);
    }
}

//function for round
static double my_round(double d)
{
    //do not use math lib floor, round or etc because of some problem
    //use mathematics calculation
    double result;
    result = (int)(d * 100 + .5);
    result = (double)result / 100;
    return result;
}

/*
 * Main interface function for updating the progress bar. This
 * function doesn't print a newline, so you can call it iteratively
 * and have the progress bar grow across the screen. The caller can
 * print a newline when the're ready to go to a new line.
 *
 * percentage: a double between 0.0 and 100.0 indicating the progress.

 * start: usecs timestamp for when the task started, for calculating
 *        remaining time.
 */
void print_progress(double percentage, uint64_t start) {
    size_t i;
    size_t total_blocks = PROGRESS_BAR_WIDTH * NUM_SUBBLOCKS;
    size_t done = (size_t)my_round(percentage / 100.0 * total_blocks);
    size_t num_blocks = done / NUM_SUBBLOCKS;
    size_t num_subblocks = done % NUM_SUBBLOCKS;

    uint64_t now = get_timestamp();
    uint64_t elapsed = now - start;
//    uint64_t estimated_total = elapsed / (percentage / 100.0);
//    uint64_t remaining = estimated_total - elapsed;
//reverse percentage by 100% - percentage;
    uint64_t estimated_total = elapsed / ((100.0 - percentage) / 100.0);
    uint64_t remaining = 100 - percentage;



    printf("   Progress: %6.2f%% \t%s", percentage, BAR_START);

    for (i = 0; i < num_blocks; i++) {
        printf("%s", PROGRESS_BLOCK);
    }

    if (num_subblocks) {
        printf("%s", subprogress_blocks[num_subblocks]);
        i++;
    }

    for (; i < PROGRESS_BAR_WIDTH; i++) {
        printf(" ");
    }

    printf("%s\t", BAR_STOP);

    if (percentage < 100.0) {
        printf("NOT COPY: ");
        printf("%lu\t",remaining);
        //print 's' for second
        printf("%\t");
    }
    else {
        printf("                          ");
    }

//    printf("\n"); // Add a newline character to ensure progress bar appears on a new line
    fflush(stdout);
    printf("\033[2K\r");
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
	//print using print_progress function
    print_progress((double)cur / total * 100, get_timestamp());

    
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


#ifndef SYCALL_H
#define SYSCALL_H

//functions used for call system
char read_string(char *buf);  
char read_char();

//call system
void print_char(char c);
void print_string(char *str);
void read_sector(int sector, uchar *buf);
void write_sector(int sector, uchar *buf);

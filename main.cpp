#include <iostream>
#include <cstring>
#include <vector>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <cstdlib>
#include <iomanip>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
using namespace std;
//#define O_RDONLY 0
//#define O_WRONLY 1
//#define O_RDWR 2
struct file {
    int chmod;
    string name;
};
vector<file> list;
char file_name[20];
char write_mode[20];
char mode[20];
int fd;
void help() {
    cout << "\t\t file-tools v1.0" << endl
         //<< "仅支持txt格式文件" << endl
         << "1.创建新文件 -m [file_name]" << endl
         << "2.写文件 -w [FLAG] [file_name] \n \t     FLAG: -a 追加 -o 覆盖" << endl
         << "3.读文件 -r [file_name]" << endl
         << "4.修改文件权限 -c [FLAG] [file_name] \n \t \t  FLAG: -r 只读方式 -w 只写方式 -rw 读写方式" << endl
         << "5.查看文件权限 -l [file_name]" << endl;
         //<< "6.查看文件记录 -ls" << endl;
         //<< "0.退出 -e" << endl;
}
void make() {
    umask(000);
    fd = open(file_name, O_RDWR | O_CREAT);
    if(fd < 0){
        printf("创建失败\n");
    } else{
        printf("创建成功\n");
    }
    close(fd);
}
void write() {
    string buffer;
    printf("请输入文件内容\n");
    getline(cin, buffer);
    if(strcmp(write_mode, "-a") == 0) {
        fd = open(file_name, O_RDWR | O_APPEND);
    } else {
        fd = open(file_name, O_RDWR | O_TRUNC);
    }
    int returnnum = write(fd, buffer.c_str(), buffer.length());
    if(returnnum != -1){
        printf("写入成功！\n");
    }else {
        printf("写入失败！\n");
    }
    close(fd);
}
void read() {
    char buffer[1<<20];
    memset(buffer, 0, sizeof(buffer));
    fd = open(file_name, O_RDWR);
    lseek(fd, 0, SEEK_SET);
    int returnnum=read(fd,buffer,1024 * 1024);
    if(returnnum!=-1){
        printf("文件内容为：\n");
        printf("%s\n",buffer);
    } else {
        printf("读取失败\n");
    }
    close(fd);
}
void chmod() {
    struct stat buf;
    stat(file_name, &buf);
    if (geteuid() != buf.st_uid) {
        cout <<"无法修改文件权限, 请查看您是否是文件所有者？" << endl;
        return;
    }
    struct stat statbuf;
    int m;
    if(strcmp(mode, "-w") == 0) {
        m = S_IWUSR;
    } else if(strcmp(mode, "-r") == 0) {
        m = S_IRUSR;
    } else {
        m = S_IRUSR | S_IWUSR;
    }
    if(chmod(file_name, (statbuf.st_mode & ~S_IXGRP) | m) < 0) {
        cerr << "chmod error for " << file_name << endl;
    }
}
void lmod() {
    char* pargv[4]={"ls", "-l", const_cast<char *>(file_name), nullptr};
    execv("/bin/ls",pargv);
}
int main(int argc, char *argv[]) {
    //string str;
    //while(true) {
        //getline(cin, str);
        //vector<string> argv;
        //string s;
        //istringstream is(str);
        //while(is >> s) {
            //argv. push_back (s);
        //}
        if (strcmp(argv[1],"-m") == 0) {
            strcpy(file_name, argv[2]);
            //file_name = argv[1];
            make();
        } else if (strcmp(argv[1], "-w") == 0) {
            //write_mode = argv[1];
            strcpy(write_mode, argv[2]);
            //file_name = argv[2];
            strcpy(file_name, argv[3]);
            write();
        } else if (strcmp(argv[1], "-r") == 0) {
            //file_name = argv[1];
            strcpy(file_name, argv[2]);
            read();
        } else if (strcmp(argv[1], "-c") == 0) {
            //mode = argv[1];
            strcpy(mode, argv[2]);
            //file_name = argv[2];
            strcpy(file_name, argv[3]);
            chmod();
        } else if (strcmp(argv[1], "-l") == 0) {
            //file_name = argv[1];
            strcpy(file_name, argv[2]);
            lmod();
        } else if (strcmp(argv[1], "-h") == 0) {
            help();
        } else if (strcmp(argv[1], "-ls") == 0) {
            //file_name = argv[1];
            /*
            cout << "序号 | 权限 | 文件" << endl;
            for (int i = 0; i < list.size(); i++) {
                cout << setw(2) << i << " " << list[i].chmod << " " << list[i].name << endl;
            }
             */
        //} else if(argv[0] == "-e") {
        //    break;
        } else {
            cout << "输入错误，输入 -h 查看帮助" << endl;
        }
    //}
    return 0;
}

/*
 *  mysystem.c : system 関数のクローン
 */

#include <stdio.h>
#include <stdlib.h>    // exit, system のため
#include <unistd.h>    // fork, execXX のため
#include <sys/wait.h>  // wait のため
#include "mysystem.h"  // インタフェース

char *execpath = "/bin/sh";

// system関数のクローン
int mysystem(char *command) {
  if(command == NULL) return 1;//引数がnullなら返り値は1
  int status = 0;//待機状況
  int pid;//フォークできたか

  // ここにプログラムを書く
  if ((pid=fork())<0) {//フォークに失敗した時
    perror(execpath);
    return -1;//返り値は0
  }
  if (pid!=0) {//親プロセスについて
    while (wait(&status)!=pid);//子プロセスが終了するまで待機
  } else {
    execl(execpath, "sh", "-c", command, NULL);//引数を/bin/shに渡して実行
    perror(execpath);
    exit(127);//エラーの場合終了
  }
  return status;
}

/* 実行例
ここに実行例を書く
% make  //コンパイル
cc -Wall -std=c99 -o mysysmain mysysmain.c  mysystem.c
% ./mysysmain ls -l //引数が2この場合
使い方 : ./mysysmain コマンド文字列
% ./mysysmain "ls -l README.md" //正常に実行
mysystem:
-rw-r--r--  1 azumamanaki  staff  2795  7  4 09:44 README.md
retval = 00000000
system:
-rw-r--r--  1 azumamanaki  staff  2795  7  4 09:44 README.md
retval = 00000000
% ./mysysmain "cat a" //エラーについても同様の実行結果          
mysystem:
cat: a: No such file or directory
retval = 00000100
system:
cat: a: No such file or directory
retval = 00000100
% ./mysysmain aaa  //存在しないコマンド実行     
mysystem:
sh: aaa: command not found
retval = 00007f00
system:
sh: aaa: command not found
retval = 00007f00
*/

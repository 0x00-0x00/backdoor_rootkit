#ifndef _UTIL_H_
#define _UTIL_H_

#include <stdint.h>
//����sock fdȡ�Զ�ip��ַ
extern uint32_t getpeerip(int fd);
//���������豸��ȡip��ַ
extern uint32_t getipbyif(const char* ifname);
void getinfo();
extern char hostinfo[256]; 
#endif

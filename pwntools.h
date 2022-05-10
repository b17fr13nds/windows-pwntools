#include <iostream>
#include <cstring>
#include <windows.h>
#include <tchar.h>
#include <stdio.h> 
#include <strsafe.h>

class proc_t {
private:
	char read_byte() {
		char b;
		BOOL ret = FALSE;

		while (!ret) ret = ReadFile(stdout_read, &b, 0x1, NULL, NULL);

		return b;
	}
public:
	HANDLE stdout_write = NULL;
	HANDLE stdout_read = NULL;
	HANDLE stdin_write = NULL;
	HANDLE stdin_read = NULL;

	void send(const CHAR buf[]) {
		BOOL ret = FALSE;

		while (!ret) ret = WriteFile(stdin_write, buf, strlen(buf), NULL, NULL);

		return;
	}

	void send(char* buf, size_t len) {
		BOOL ret = FALSE;

		while (!ret) ret = WriteFile(stdin_write, buf, len, NULL, NULL);

		return;
	}

	char* recv(size_t sz) {
		HANDLE heap = GetProcessHeap();
		bool ret = FALSE;

		char* buf = (char*)HeapAlloc(heap, 0, sz + 1);
		ZeroMemory(buf, sz + 1);
		for (int i = 0; i < sz; i++) {
			buf[i] = read_byte();
		}

		return buf;
	}

	char* recvline(size_t sz) {
		HANDLE heap = GetProcessHeap();
		bool ret = FALSE;

		char* buf = (char*)HeapAlloc(heap, 0, sz + 1);
		ZeroMemory(buf, sz + 1);
		for (int i = 0; i < sz; i++) {
			buf[i] = read_byte();
			if (buf[i] == 0x0a) break;
		}

		return buf;
	}

	char* recvline(void) {
		HANDLE heap = GetProcessHeap();
		bool ret = FALSE;

		char* buf = (char*)HeapAlloc(heap, 0, 0x1000);
		ZeroMemory(buf, 0x1000);
		for (int i = 0; i < 0x1000; i++) {
			buf[i] = read_byte();
			if (buf[i] == 0x0a) break;
		}

		return buf;
	}

	char* recvuntil(const CHAR pattern[]) {
		HANDLE heap = GetProcessHeap();
		bool ret = FALSE;

		char* buf = (char*)HeapAlloc(heap, 0, 0x1000);
		ZeroMemory(buf, 0x1000);
		for (int i = 0; i < 0x1000; i++) {
			buf[i] = read_byte();
			if (strstr(buf, pattern)) break;
		}

		return buf;
	}

	char* recvuntil(const CHAR pattern[], size_t sz) {
		HANDLE heap = GetProcessHeap();
		bool ret = FALSE;
		DWORD read = 0;

		char* buf = (char*)HeapAlloc(heap, 0, sz + 1);
		ZeroMemory(buf, sz + 1);
		for (int i = 0; i < sz; i++) {
			buf[i] = read_byte();
			if (strstr(buf, pattern)) break;
		}

		return buf;
	}

};

void spawn(proc_t*, wchar_t*);
proc_t* process(const CHAR[]);
char* p64(uint64_t);
char* p32(uint32_t);
char* p16(uint16_t);
char* p8(uint8_t);

uint64_t u64(const char*);
uint32_t u32(const char*);
uint16_t u16(const char*);
uint8_t u8(const char);

#include <iostream>
#include <cstring>
#include <windows.h>
#include <tchar.h>
#include <stdio.h> 
#include <strsafe.h>
#include "pwntools.h"

proc_t *process(const CHAR buf[]) {
	SECURITY_ATTRIBUTES saAttr;

	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
	saAttr.bInheritHandle = TRUE;
	saAttr.lpSecurityDescriptor = NULL;

	proc_t *p = new proc_t;

	if (!CreatePipe(&p->stdout_read, &p->stdout_write, &saAttr, 0)) exit(-1);
	if (!SetHandleInformation(p->stdout_read, HANDLE_FLAG_INHERIT, 0)) exit(-2);
	if (!CreatePipe(&p->stdin_read, &p->stdin_write, &saAttr, 0)) exit(-3);
	if (!SetHandleInformation(p->stdin_write, HANDLE_FLAG_INHERIT, 0)) exit(-4);

	std::wstring w;
	std::copy(buf, buf + strlen(buf), back_inserter(w));
	spawn(p, (wchar_t *)w.c_str());

	return p;
}

char* p64(uint64_t x) {
	char* res = new char[10];
	res[0] = x & 0xff;
	res[1] = (x & 0xff00) >> 0x8;
	res[2] = (x & 0xff0000) >> 0x10;
	res[3] = (x & 0xff000000) >> 0x18;
	res[4] = (x & 0xff00000000) >> 0x20;
	res[5] = (x & 0xff0000000000) >> 0x28;
	res[6] = (x & 0xff000000000000) >> 0x30;
	res[7] = (x & 0xff00000000000000) >> 0x38;

	return res;
}

char* p32(uint32_t x) {
	char* res = new char[6];
	res[0] = x & 0xff;
	res[1] = (x & 0xff00) >> 0x8;
	res[2] = (x & 0xff0000) >> 0x10;
	res[3] = (x & 0xff000000) >> 0x18;

	return res;
}

char* p16(uint16_t x) {
	char* res = new char[4];
	res[0] = x & 0xff;
	res[1] = (x & 0xff00) >> 0x8;

	return res;
}

char* p8(uint8_t x) {
	char* res = new char[3];
	res[0] = x & 0xff;

	return res;
}

uint64_t u64(const char* x) {
	uint64_t res = 0;
	res += x[0];
	res += (x[1] << 0x8);
	res += (x[2] << 0x10);
	res += (x[3] << 0x18);
	res += (x[4] << 0x20);
	res += (x[5] << 0x28);
	res += (x[6] << 0x30);
	res += (x[7] << 0x38);
	return res;
}

uint32_t u32(const char* x) {
	uint32_t res = 0;
	res += x[0];
	res += (x[1] << 0x8);
	res += (x[2] << 0x10);
	res += (x[3] << 0x18);
	return res;
}

uint16_t u16(const char* x) {
	uint16_t res = 0;
	res += x[0];
	res += (x[1] << 0x8);
	return res;
}

uint8_t u8(const char x) {
	uint8_t res = 0;
	res += x;
	return res;
}

void spawn(proc_t *p, wchar_t *progname) {
	TCHAR *cmd = (TCHAR *)progname;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	si.hStdError = p->stdout_write;
	si.hStdOutput = p->stdout_write;
	si.hStdInput = p->stdin_read;
	si.dwFlags |= STARTF_USESTDHANDLES;
	ZeroMemory(&pi, sizeof(pi));

	if (!CreateProcess(NULL, cmd, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) exit(-5);

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	CloseHandle(p->stdout_write);
	CloseHandle(p->stdin_read);
	return;
}

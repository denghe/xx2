#pragma once
#include "xx_string.h"
#ifdef _WIN32
#include <strsafe.h>

namespace xx {

	// https://learn.microsoft.com/en-us/windows/win32/procthread/creating-a-child-process-with-redirected-input-and-output

	// Format a readable error message, display a message box, 
	// and exit from the application.
	inline void ErrorExit(PCTSTR lpszFunction) {
		LPVOID lpMsgBuf;
		LPVOID lpDisplayBuf;
		DWORD dw = GetLastError();

		FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			dw,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)&lpMsgBuf,
			0, NULL);

		lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
			(lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
		StringCchPrintf((LPTSTR)lpDisplayBuf,
			LocalSize(lpDisplayBuf) / sizeof(TCHAR),
			TEXT("%s failed with error %d: %s"),
			lpszFunction, dw, lpMsgBuf);
		MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

		LocalFree(lpMsgBuf);
		LocalFree(lpDisplayBuf);
		ExitProcess(1);
	}

	inline std::wstring Utf8ToWstring(const std::string& s) {
		if (s.empty()) return {};
		int size_needed = MultiByteToWideChar(CP_UTF8, 0, &s[0], (int)s.size(), NULL, 0);
		std::wstring wstrTo(size_needed, 0);
		MultiByteToWideChar(CP_UTF8, 0, &s[0], (int)s.size(), &wstrTo[0], size_needed);
		return wstrTo;
	}

	inline std::string WstringToUtf8(const std::wstring& w) {
		if (w.empty()) return {};
		int size_needed = WideCharToMultiByte(CP_UTF8, 0, &w[0], (int)w.size(), NULL, 0, NULL, NULL);
		std::string strTo(size_needed, 0);
		WideCharToMultiByte(CP_UTF8, 0, &w[0], (int)w.size(), &strTo[0], size_needed, NULL, NULL);
		return strTo;
	}

	inline void ReadFromPipe(std::string& out, HANDLE hPipe) {
		DWORD dwRead;
		CHAR chBuf[4096];
		BOOL bSuccess = FALSE;

		while(true) {
			bSuccess = ReadFile(hPipe, chBuf, sizeof(chBuf), &dwRead, NULL);
			if (!bSuccess || dwRead == 0) break;
			out.append(chBuf, dwRead);
		}
	}

	inline std::pair<DWORD, std::string> ExecCmd(std::string utf8cmdline_) {
		auto cmd = Utf8ToWstring(utf8cmdline_);

		PROCESS_INFORMATION pi;
		ZeroMemory(&pi, sizeof(pi));

		SECURITY_ATTRIBUTES sa;
		sa.nLength = sizeof(SECURITY_ATTRIBUTES);
		sa.lpSecurityDescriptor = NULL;
		sa.bInheritHandle = TRUE;

		HANDLE hChildStdInRd{}, hChildStdInWr{};
		if(!CreatePipe(&hChildStdInRd, &hChildStdInWr, &sa, 0)) ErrorExit(TEXT("CreatePipe hChildStdInRd, hChildStdInWr"));
		if (!SetHandleInformation(hChildStdInRd, HANDLE_FLAG_INHERIT, 0)) ErrorExit(TEXT("SetHandleInformation hChildStdInRd"));

		HANDLE hChildStdOutRd{}, hChildStdOutWr{};
		if(!CreatePipe(&hChildStdOutRd, &hChildStdOutWr, &sa, 0)) ErrorExit(TEXT("CreatePipe hChildStdOutRd, hChildStdOutWr"));
		if (!SetHandleInformation(hChildStdOutRd, HANDLE_FLAG_INHERIT, 0)) ErrorExit(TEXT("SetHandleInformation hChildStdOutRd"));

		STARTUPINFOW si;
		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		si.hStdError = hChildStdOutWr;
		si.hStdOutput = hChildStdOutWr;
		si.hStdInput = hChildStdInRd;
		si.dwFlags |= STARTF_USESTDHANDLES;
		//si.wShowWindow = SW_HIDE;

		std::string rtv;
		if (!CreateProcessW(NULL,         // application name
			cmd.data(),    // Command line
			NULL,          // Process handle not inheritable
			NULL,          // Thread handle not inheritable
			TRUE,          // Set handle inheritance to TRUE
			0,             // No creation flags
			NULL,          // Use parent's environment block
			NULL,          // Use parent's starting directory 
			&si,           // Pointer to STARTUPINFO structure
			&pi)           // Pointer to PROCESS_INFORMATION structure
			) {
			ErrorExit(TEXT("CreateProcess"));
		}
		else {
			//ReadFromPipe(rtv, hChildStdOutRd);
			//WaitForSingleObject(pi.hProcess, INFINITE);

			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
			CloseHandle(hChildStdOutWr);
			CloseHandle(hChildStdInRd);
		}

		ReadFromPipe(rtv, hChildStdOutRd);

		return { 0, rtv };
	}

}
#endif

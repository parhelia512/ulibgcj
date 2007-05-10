#include <config.h>
#include <platform.h>
#include <stdio.h>

#undef STRICT

#include <java/lang/System$Input.h>
#include <java/lang/System$Output.h>
#include <java/lang/Runtime$ConcreteProcess.h>
#include <java/lang/IllegalThreadStateException.h>
#include <java/io/OutputStream.h>
#include <java/io/InputStream.h>
#include <java/io/IOException.h>

namespace {

void
makePipe(HANDLE p[2])
{
  SECURITY_ATTRIBUTES sa;
  sa.nLength = sizeof(sa);
  sa.bInheritHandle = 1;
  sa.lpSecurityDescriptor = 0;

  BOOL success = CreatePipe(p, p + 1, &sa, 0);
  if (not success) {
    throw new java::io::IOException
      (_Jv_WinStrError(_T("Error creating child process"), GetLastError()));
  }
}

int
descriptor(HANDLE h)
{
  int fd = _open_osfhandle(reinterpret_cast<long>(h), 0);
  if (fd == -1) {
    throw new java::io::IOException
      (JvNewStringUTF("error opening file descriptor"));
  }
  return fd;
}

} // namespace

void
java::lang::Runtime$ConcreteProcess::start()
{
  unsigned size = 0;
  for (unsigned i = 0; i < command->length; ++i)
    size += elements(command)[i]->length() + 1;

  char line[size];
  char* linep = line;
  for (unsigned i = 0; i < command->length; ++i) {
    if (i) *(linep++) = _T(' ');
    JV_TEMP_STRING_WIN32(s, elements(command)[i]);
    _tcscpy(linep, s);
    linep += elements(command)[i]->length();
  }
  *(linep++) = _T('\0');

  HANDLE in[] = { 0, 0 };
  HANDLE out[] = { 0, 0 };
  HANDLE err[] = { 0, 0 };

  try {
    makePipe(in);
    _Jv_platform_close_on_exec(in[0]);
    this->in = new java::lang::System$Input(descriptor(in[0]));

    makePipe(out);
    _Jv_platform_close_on_exec(out[1]);
    this->out = new java::lang::System$Output(descriptor(out[1]));

    makePipe(err);
    _Jv_platform_close_on_exec(err[0]);
    this->err = new java::lang::System$Input(descriptor(err[0]));

    PROCESS_INFORMATION pi;
    ZeroMemory(&pi, sizeof(pi));

    STARTUPINFO si;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESTDHANDLES;
    si.hStdOutput = in[1];
    si.hStdInput = out[0];
    si.hStdError = err[1];

    BOOL success = CreateProcess(0, line, 0, 0, 1,
                                 CREATE_NO_WINDOW | CREATE_UNICODE_ENVIRONMENT,
                                 0, 0, &si, &pi);

    if (not success) {
      throw new java::io::IOException
        (_Jv_WinStrError(_T("Error creating child process"), GetLastError()));
    }

    pid = reinterpret_cast<jlong>(pi.hProcess);
  } catch (java::lang::Throwable* e) {
    CloseHandle(reinterpret_cast<HANDLE>(pid));
    pid = 0;

    throw e;
  }
}

jint
java::lang::Runtime$ConcreteProcess::waitFor()
{
  if (pid) {
    WaitForSingleObject(reinterpret_cast<HANDLE>(pid), INFINITE);
    DWORD status = 0;
    GetExitCodeProcess(reinterpret_cast<HANDLE>(pid), &status);
    exitCode = status;
    CloseHandle(reinterpret_cast<HANDLE>(pid));
    pid = 0;
  }

  return exitCode;
}

jint
java::lang::Runtime$ConcreteProcess::exitValue()
{
  if (pid == 0) {
    return exitCode;
  } else {
    throw new java::lang::IllegalThreadStateException;
  }
}

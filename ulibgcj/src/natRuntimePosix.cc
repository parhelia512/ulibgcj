#include <config.h>

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <gcj/cni.h>
#include <java/lang/System$Input.h>
#include <java/lang/System$Output.h>
#include <java/lang/Runtime$ConcreteProcess.h>
#include <java/lang/IllegalThreadStateException.h>
#include <java/io/OutputStream.h>
#include <java/io/InputStream.h>
#include <java/io/IOException.h>

namespace {

char*
cstring(java::lang::String* s)
{
  unsigned size = _Jv_GetStringUTFLength(s);
  char* r = static_cast<char*>(_Jv_Malloc(size + 1));
  _Jv_GetStringUTFRegion(s, 0, s->length(), r);
  r[size] = 0;
  return r;
}

void
makePipe(int p[2])
{
  if (pipe(p) != 0) {
    throw new java::io::IOException(JvNewStringUTF(strerror(errno)));
  }
}

void
safeClose(int &fd)
{
  if (fd != -1) close(fd);
  fd = -1;
}

void
safeClose(int p[2])
{
  safeClose(p[0]);
  safeClose(p[1]);
}

void
close(int p[2])
{
  ::close(p[0]);
  ::close(p[1]);
}

void
clean(char** p)
{
  for (char** x = p; *x; ++x) _Jv_Free(*x);
  _Jv_Free(p);
}

} // namespace

void
java::lang::Runtime$ConcreteProcess::start()
{
  char** argv = static_cast<char**>
    (_Jv_Malloc((command->length + 1) * sizeof(char*)));

  unsigned i;
  for (i = 0; i < command->length; ++i)
    argv[i] = cstring(elements(command)[i]);
  argv[i] = 0;

//   printf("command: ");
//   for (i = 0; i < command->length; ++i)
//     printf("%s ", argv[i]);
//   printf("\n");

  int in[] = { -1, -1 };
  int out[] = { -1, -1 };
  int err[] = { -1, -1 };
  int msg[] = { -1, -1 };

  try {
    makePipe(in);
    this->in = new java::lang::System$Input(in[0]);

    makePipe(out);
    this->out = new java::lang::System$Output(out[1]);

    makePipe(err);
    this->err = new java::lang::System$Input(err[0]);

    makePipe(msg);
    if (fcntl(msg[1], F_SETFD, FD_CLOEXEC) != 0) {
      throw new java::io::IOException(JvNewStringUTF(strerror(errno)));
    }

    pid_t pid = fork();
    switch (pid) {
    case -1: // error
      throw new java::io::IOException(JvNewStringUTF(strerror(errno)));

    case 0: { // child
      dup2(in[1], 1);
      close(in);

      dup2(out[0], 0);
      close(out);

      dup2(err[1], 2);
      close(err);

      close(msg[0]);

      execvp(argv[0], argv);

      char c = errno;
      write(msg[1], &c, 1);
      exit(127);
    } break;

    default: { // parent
      this->pid = pid;

      safeClose(in[1]);
      safeClose(out[0]);
      safeClose(err[1]);
      safeClose(msg[1]);

      char c;
      int r = read(msg[0], &c, 1);
      if (r == -1) {
        throw new java::io::IOException(JvNewStringUTF(strerror(errno)));
      } else if (r) {
        throw new java::io::IOException(JvNewStringUTF(strerror(c)));
      }
    } break;
    }
  } catch (java::lang::Throwable* e) {
    safeClose(in);
    safeClose(out);
    safeClose(err);
    safeClose(msg);

    this->in = 0;
    this->out = 0;
    this->err = 0;

    clean(argv);

    throw e;
  }

  safeClose(msg[0]);
  clean(argv);
  
  fcntl(in[0], F_SETFD, FD_CLOEXEC);
  fcntl(out[1], F_SETFD, FD_CLOEXEC);
  fcntl(err[0], F_SETFD, FD_CLOEXEC);
}

jint
java::lang::Runtime$ConcreteProcess::waitFor()
{
  while (pid) {
    int status;
    waitpid(pid, &status, 0);
    if (WIFEXITED(status)) {
      pid = 0;
      exitCode = WEXITSTATUS(status);
    } else if (WIFSIGNALED(status)) {
      pid = 0;
      exitCode = -1;
    }
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

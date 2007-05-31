#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#ifdef JV_ULIBGCJ_WIN32
#  include <io.h>
#  define OPEN _open
#  define CLOSE _close
#  define READ _read
#  define WRITE _write
#  define STAT _stat
#  define STRUCT_STAT struct _stat
#  define MKDIR(path, mode) _mkdir(path)
#  define CREAT _creat
#else
#  include <unistd.h>
#  define OPEN open
#  define CLOSE close
#  define READ read
#  define WRITE write
#  define STAT stat
#  define STRUCT_STAT struct stat
#  define MKDIR mkdir
#  define CREAT creat
#endif

#include <gcj/cni.h>

#include <java/io/File.h>
#include <java/io/FileInputStream.h>
#include <java/io/FileOutputStream.h>
#include <java/io/IOException.h>
#include <java/lang/String.h>

using namespace java::io;
using namespace java::lang;

namespace {

void
copy(String* src, char* dst)
{
  int i;
  for (i = 0; i < src->length(); ++i) {
    dst[i] = src->charAt(i);
  }
  dst[i] = 0;
}

inline String*
errorString(int e)
{
  return JvNewStringLatin1(strerror(e));
}

inline String*
errorString()
{
  return errorString(errno);
}

int
doOpen(String* path, int mask)
{
  char buffer[path->length() + 1];
  copy(path, buffer);

  return OPEN(buffer, mask, S_IRUSR | S_IWUSR);
}

inline int
doClose(int fd)
{
  return CLOSE(fd);
}

inline int
doRead(int fd, void* p, int c)
{
  return READ(fd, p, c);
}

inline int
doWrite(int fd, const void* p, int c)
{
  return WRITE(fd, p, c);
}

} // namespace

jlong
File::length()
{
  char buffer[path->length() + 1];
  copy(path, buffer);

  STRUCT_STAT s;
  int r = STAT(buffer, &s);
  if (r == 0) {
    return s.st_size;
  } else {
    return -1;
  }
}

void
File::mkdir()
{
  if (not exists()) {
    char buffer[path->length() + 1];
    copy(path, buffer);

    int r = ::MKDIR(buffer, 0700);
    if (r != 0) {
      throw new IOException(errorString());      
    }
  }
}

void
File::createNewFile()
{
  if (not exists()) {
    char buffer[path->length() + 1];
    copy(path, buffer);

    int fd = CREAT(buffer, 0600);
    if (fd == -1) {
      throw new IOException(errorString());      
    } else {
      doClose(fd);
    }
  }
}

jboolean
File::exists()
{
  char buffer[path->length() + 1];
  copy(path, buffer);

  STRUCT_STAT s;
  int r = STAT(buffer, &s);
  if (r == 0) {
    return true;
  } else {
    return false;
  }
}

jlong
FileInputStream::open(String* path)
{
  int fd = doOpen(path, O_RDONLY);
  if (fd == -1) {
    throw new IOException(errorString());
  } else {
    return fd;
  }
}

void
FileInputStream::close(jlong fd)
{
  if (doClose(fd) == -1) {
    throw new IOException(errorString());
  }
}

jint
FileInputStream::read(JArray<jbyte>* b, jint offset, jint length)
{
  int r = doRead(descriptor, elements(b) + offset, length);
  if (r > 0) {
    return r;
  } else if (r == 0) {
    return -1;
  } else {
    throw new IOException(errorString());
  }
}

jlong
FileOutputStream::open(String* path)
{
  int fd = doOpen(path, O_WRONLY | O_CREAT);
  if (fd == -1) {
    throw new IOException(errorString());
  } else {
    return fd;
  }
}

void
FileOutputStream::close(jlong fd)
{
  if (doClose(fd) == -1) {
    throw new IOException(errorString());
  }
}

void
FileOutputStream::write(JArray<jbyte>* b, jint offset, jint length)
{
  int r = doWrite(descriptor, elements(b) + offset, length);
  if (r == length) {
    return;
  } else {
    throw new IOException(errorString());
  }
}

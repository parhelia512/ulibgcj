#include "config.h"
#include "gcj/cni.h"
#include "Hello.h"
#include "gc.h"

int
main()
{
  GC_set_max_heap_size(24 * 1024 * 1024);

  JvCreateJavaVM(0);
  JvAttachCurrentThread(NULL, NULL);

  Hello::main(0);

  JvDetachCurrentThread();

  return 0;
}

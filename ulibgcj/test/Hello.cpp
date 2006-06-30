#include "config.h"
#include "gcj/cni.h"
#include "Hello.h"
#ifdef HAVE_BOEHM_GC
#include "gc.h"
#endif

int
main()
{
#ifdef HAVE_BOEHM_GC
  GC_set_max_heap_size(24 * 1024 * 1024);
#endif

  JvCreateJavaVM(0);
  JvAttachCurrentThread(NULL, NULL);

  Hello::main(0);

  JvDetachCurrentThread();

  return 0;
}

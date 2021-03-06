#include "config.h"
#include "gcj/cni.h"
#include "java/io/PrintStream.h"
#include "java/lang/System.h"
#include "java/lang/System$Output.h"
#include "java/lang/Throwable.h"
#include "CoreTest.h"
#include "gc.h"

int
main()
{
  GC_set_max_heap_size(24 * 1024 * 1024);

  try {
    JvCreateJavaVM(0);
    JvAttachCurrentThread(NULL, NULL);

    CoreTest::main(0);

    JvDetachCurrentThread();
  } catch (java::lang::Throwable *t) {
    java::lang::System::err->println (JvNewStringLatin1("caught something!"));
  }

  return 0;
}

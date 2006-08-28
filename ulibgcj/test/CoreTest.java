public class CoreTest implements Runnable {

  private static final Long SOME_USELESS_CONSTANT = new Long(955);

  private final Throwable[] uselessArray;
  private boolean trouble = false;

  CoreTest() {
    uselessArray = new Throwable[3];
    uselessArray[0] = new IndexOutOfBoundsException();
  }

  private static String doToString(Object o) {
    return o.toString();
  }

  private static String toString(Object o) {
    return doToString(o);
  }

  public static void main(String[] args) throws Exception {
    System.out.println("a useless constant: " + SOME_USELESS_CONSTANT);

    System.out.println("making thrower");
    Thrower t = new Thrower();
    System.out.println("calling Thrower.throwSomething");
    try {
      t.throwSomething();
    } catch (Throwable e) {
      System.out.println("as expected, caught " + e);
    }

    try {
      Object[] array = new Object[5];
      for (int i = 0; i < 6; ++i) {
        array[i] = array;
      }
      System.err.println(" *** you should never see this *** ");
      return;
    } catch (ArrayIndexOutOfBoundsException e) {
      System.out.println("as expected, caught: " + e);
    }

    Process p = Runtime.getRuntime().exec(new String[] { "cat" });
    String hello = "hello, world!\n";
    p.getOutputStream().write(hello.getBytes());
    p.getOutputStream().flush();
    byte[] buffer = new byte[32];
    int c;
    int offset = 0;
    while (offset < hello.length()
           && (c = p.getInputStream().read
               (buffer, offset, buffer.length - offset)) != -1)
      offset += c;
    String result = new String(buffer, 0, offset);
    if (! result.equals(hello)) {
      System.err.println("expected \"" + hello + "\", got \"" + result + "\"");
      return;
    }
    p.getOutputStream().close();

    try {
      toString(null);
    } catch (NullPointerException e) {
      System.out.println("as expected, caught: " + e);
      e.printStackTrace();
    }

    Object o = new Object();

    synchronized (o) {
      System.out.println("I'm synchronized!");
    }

    CoreTest test = new CoreTest();
    Thread th = new Thread(test);

    try {
      synchronized (test) {
        th.start();
        test.wait();
      }
    } catch (Throwable e) {
      System.out.println("caught " + e);
    }

    if (test.trouble)
      System.err.println("trouble!");
    else
      System.out.println("Everybody has won, and all must have prizes!");
  }

  public void run() {
    synchronized (this) {
      int i = 0;
      try {
        System.out.println("I'm running in a seperate thread!");

        final int arrayCount = 64;
        final int arraySize = 4;
        System.out.println("Allocating and discarding " + arrayCount +
                      " arrays of " + arraySize + "MB each");
        for (; i < arrayCount; ++i) {
//           System.out.println("allocating array " + i);
          byte[] array = new byte[arraySize * 1024 * 1024];
        }

//         long nap = 60;
//         System.out.println("sleeping for " + nap + " seconds");
//         Thread.sleep(nap * 1000);
      } catch (Throwable e) {
        System.out.println("caught " + e + " in second thread after " + i +
                           " iterations");   
        trouble = true;
      } finally {
        notifyAll();
      }
    }
  }

}

public class CoreTest implements Runnable {

  private static final Long SOME_USELESS_CONSTANT = new Long(955);

  private final Throwable[] uselessArray;
  private boolean trouble = false;

  CoreTest() {
    uselessArray = new Throwable[3];
    uselessArray[0] = new IndexOutOfBoundsException();
  }

  public static void trouble() {
    System.err.println("trouble!");
    System.exit(-1);
  }

  public static void main(String[] args) {
    System.out.println("a useless constant: " + SOME_USELESS_CONSTANT);

    try {
      System.out.println("Object class: " +
                         Class.forName("java.lang.Object").getName());
    } catch (Exception e) {
      System.err.println(e.toString());
      trouble();
    }

    System.out.println("making thrower");
    Thrower t = new Thrower();
    System.out.println("calling Thrower.throwSomething");
    try {
      t.throwSomething();
      trouble();
    } catch (Throwable e) {
      System.out.println("as expected, caught " + e);
    }

    try {
      Object[] array = new Object[5];
      for (int i = 0; i < 6; ++i) {
        array[i] = array;
      }
      trouble();
    } catch (ArrayIndexOutOfBoundsException e) {
      System.out.println("as expected, caught: " + e);
    }

    try {
      Object o = null;
      o.toString();
      trouble();
    } catch (NullPointerException e) {
      System.out.println("as expected, caught: " + e);
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
      System.err.println("caught " + e);
    }

    System.out.println("home directory: " + System.getProperty("user.home"));

    if (test.trouble)
      trouble();
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

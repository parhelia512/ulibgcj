package java.io;

public class FileOutputStream extends OutputStream {
  private final long descriptor;

  public FileOutputStream(String path) {
    descriptor = open(path);
  }

  public FileOutputStream(File file) {
    this(file.getPath());
  }

  public void write(int v) throws IOException {
    byte[] b = new byte[] { (byte) v };
    write(b);
  }

  public native void write(byte[] buffer, int offset, int length)
    throws IOException;

  public void flush() {
    // not necessary, since we do no buffering
  }

  public void close() throws IOException {
    //flush();
    close(descriptor);
  }

  private static native long open(String path);

  private static native void close(long descriptor);
}

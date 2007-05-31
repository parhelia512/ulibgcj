package java.io;

public class FileInputStream extends InputStream {
  private final long descriptor;

  public FileInputStream(String path) {
    descriptor = open(path);
  }

  public FileInputStream(File file) {
    this(file.getPath());
  }

  public int read() throws IOException {
    byte[] b = new byte[1];
    int c = read(b);
    if (c == -1) return -1; else return (b[0] & 0xFF);    
  }

  public native int read(byte[] buffer, int offset, int length)
    throws IOException;

  public void close() throws IOException {
    close(descriptor);
  }

  private static native long open(String path);

  private static native void close(long descriptor);
}

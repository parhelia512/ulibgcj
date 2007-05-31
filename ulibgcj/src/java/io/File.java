package java.io;

public class File {
  private final String path;

  public File(String path) {
    if (path == null) throw new NullPointerException();
    this.path = path;
  }

  public File(String parent, String child) {
    this(parent + "/" + child);
  }

  public File(File parent, String child) {
    this(parent.getPath() + "/" + child);
  }

  public String getName() {
    int index = path.lastIndexOf("/");
    if (index >= 0) {
      return path.substring(index + 1);
    } else {
      return path;
    }
  }

  public String getAbsolutePath() {
    return path;
  }

  public native long length();

  public native void mkdir();

  public native void createNewFile();

  public native boolean exists();

  public String getPath() {
    return path;
  }
}

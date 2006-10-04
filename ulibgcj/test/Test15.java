import java.util.ArrayList;
import java.util.List;
import java.io.PrintStream;

public class Test15 {

  public static void main(String[] args) {
    final int size = 5;
    List<Foo<Integer>> list = new ArrayList<Foo<Integer>>(size);
    for (int i = 0; i < size; ++i) {
      list.add(new Foo<Integer>(i));
      ++list.get(i).value;
      list.get(i).setValue(list.get(i).getValue() * 42);
    }

    System.out.println("Hello, World!");

    print(System.out, 38, 42, list);

    System.out.println("Goodbye, World!");
  }

  private static void print(PrintStream out, Object ... args) {
    for (Object a: args) {
      out.println(a.toString());
    }

    out.println(MyEnum.Y.toString());

//     new Exception("debug trace").printStackTrace(out);
  }

  private static class Foo<T> {
    private T value;

    public Foo(T v) {
      value = v;
    }

    private void setValue(T v) {
      value = v;
    }

    private T getValue() {
      return value;
    }

    public String toString() {
      return value.toString();
    }
  }

  private static enum MyEnum {
    X, Y, Z
  }

}

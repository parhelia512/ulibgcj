/* EventObject.java -- represents an event on an object
   Copyright (C) 1999, 2000, 2002, 2005  Free Software Foundation, Inc.

This file is part of GNU Classpath.

GNU Classpath is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GNU Classpath is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU Classpath; see the file COPYING.  If not, write to the
Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
02110-1301 USA.

Linking this library statically or dynamically with other modules is
making a combined work based on this library.  Thus, the terms and
conditions of the GNU General Public License cover the whole
combination.

As a special exception, the copyright holders of this library give you
permission to link this library with independent modules to produce an
executable, regardless of the license terms of these independent
modules, and to copy and distribute the resulting executable under
terms of your choice, provided that you also meet, for each linked
independent module, the terms and conditions of the license of that
module.  An independent module is a module which is not derived from
or based on this library.  If you modify this library, you may extend
this exception to your version of the library, but you are not
obligated to do so.  If you do not wish to do so, delete this
exception statement from your version. */


package java.util;

/*if not ULIBGCJ*/
import java.io.Serializable;
/*endif*/

/**
 * Represents Events fired by Objects.
 *
 * @author Eric Blake (ebb9@email.byu.edu)
 * @see EventListener
 * @since 1.1
 * @status updated to 1.4
 */
public class EventObject
/*if not ULIBGCJ*/
  implements Serializable
/*endif*/
{
  /**
   * Compatible with JDK 1.1+.
   */
  private static final long serialVersionUID = 5516075349620653480L;

  /**
   * The source object; in other words, the object which this event takes
   * place on.
   */
  protected transient Object source;

  /**
   * Constructs an EventObject with the specified source.
   *
   * @param source the source of the event
   * @throws IllegalArgumentException if source is null (This is not
   *         specified, but matches the behavior of the JDK)
   */
  public EventObject(Object source)
  {
    // This check for null is stupid, if you ask me, since source is
    // protected and non-final, so a subclass can set it to null later on.
    if (source == null)
      throw new IllegalArgumentException();
    this.source = source;
  }

  /**
   * Returns the source of the event.
   *
   * @return the event source
   */
  public Object getSource()
  {
    return source;
  }

  /**
   * Converts the event to a String. The format is not specified, but by
   * observation, the JDK uses:
   * <code>getClass().getName() + "[source=" + source + "]";</code>.
   *
   * @return String representation of the Event
   */
  public String toString()
  {
    return getClass().getName() + "[source=" + source + "]";
  }
} // class EventObject

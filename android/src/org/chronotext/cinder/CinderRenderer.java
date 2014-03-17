/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

package org.chronotext.cinder;

import java.util.Vector;

import javax.microedition.khronos.opengles.GL10;

import org.chronotext.gl.GLRenderer;
import org.chronotext.gl.Touch;

import android.content.Context;
import android.content.res.Configuration;
import android.view.Display;
import android.view.Surface;
import android.view.WindowManager;

/*
 * WARNING: BE SURE TO DEFINE android:screenOrientation IN THE MANIFEST
 * OR TO CALL setRequestedOrientation() INSIDE Activity.onCreate()
 * BECAUSE THE CURRENT SYSTEM IS NOT HANDLING AUTO-ROTATION
 */

public class CinderRenderer extends GLRenderer
{
  public static final int EVENT_ATTACHED = 1;
  public static final int EVENT_DETACHED = 2;
  public static final int EVENT_PAUSED = 3;
  public static final int EVENT_RESUMED = 4;
  public static final int EVENT_SHOWN = 5;
  public static final int EVENT_HIDDEN = 6;
  public static final int EVENT_BACKGROUND = 7;
  public static final int EVENT_FOREGROUND = 8;

  protected Context mContext;
  protected Object mListener;
  
  public CinderRenderer(Context context, float density, Object listener)
  {
	super(density);
	
    mContext = context;
    mListener = listener;
    
    prelaunch();
  }

  /*
   * REFERENCES:
   * http://android-developers.blogspot.co.il/2010/09/one-screen-turn-deserves-another.html
   * http://developer.download.nvidia.com/tegra/docs/tegra_android_accelerometer_v5f.pdf
   */
  protected int getDisplayRotation()
  {
    Display display = ((WindowManager) mContext.getSystemService(Context.WINDOW_SERVICE)).getDefaultDisplay();
    return display.getRotation();
  }

  // ---------------------------------------- CALL-BACKS TAKING PLACE ON THE RENDERER'S THREAD ----------------------------------------

  public void launch()
  {
    launch(mContext, mListener);
  }

  public void setup(GL10 gl, float density, int width, int height)
  {
    setup(density, width, height, getDisplayRotation());
    initialized = true;
  }

  public void draw(GL10 gl)
  {
    draw();
  }

  public void attached()
  {
    event(EVENT_ATTACHED);
    attached = true;
    hidden = false;
  }

  public void detached()
  {
    event(EVENT_DETACHED);
    attached = false;
  }

  public void paused()
  {
    event(EVENT_PAUSED);
    resumed = false;
  }

  public void resumed()
  {
    event(EVENT_RESUMED);
    resumed = true;
    hidden = false;
  }

  public void background()
  {
    event(EVENT_BACKGROUND);
  }

  public void foreground()
  {
    event(EVENT_FOREGROUND);
  }

  public void shown()
  {
    event(EVENT_SHOWN);
    hidden = false;
  }

  public void hidden()
  {
    event(EVENT_HIDDEN);
    hidden = true;
  }
    
  public void addTouches(Vector<Touch> touches)
  {
    for (Touch touch : touches)
    {
      addTouch(touch.index, touch.x, touch.y);
    }
  }

  public void updateTouches(Vector<Touch> touches)
  {
    for (Touch touch : touches)
    {
      updateTouch(touch.index, touch.x, touch.y);
    }
  }

  public void removeTouches(Vector<Touch> touches)
  {
    for (Touch touch : touches)
    {
      removeTouch(touch.index, touch.x, touch.y);
    }
  }

  // ---------------------------------------- JNI ----------------------------------------

  public native void prelaunch();

  public native void launch(Context context, Object listener);

  public native void setup(float density, int width, int height, int displayRotation);

  public native void shutdown();

  public native void draw();

  public native void event(int id);

  public native void addTouch(int index, float x, float y);

  public native void updateTouch(int index, float x, float y);

  public native void removeTouch(int index, float x, float y);

  public native void sendMessage(int what, String body);
}

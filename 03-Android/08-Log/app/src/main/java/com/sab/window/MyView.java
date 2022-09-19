package com.sab.window;

import androidx.appcompat.widget.AppCompatTextView;
import android.content.Context;
import android.graphics.Color;
import android.view.Gravity;
// Event Related
import android.view.GestureDetector;
import android.view.GestureDetector.OnDoubleTapListener;
import android.view.GestureDetector.OnGestureListener;
import android.view.MotionEvent;

public class MyView extends AppCompatTextView implements OnDoubleTapListener, OnGestureListener
{
	private GestureDetector gestureDetector;
	MyView(Context context) // All global environmental info about apk in context. system.object la inherit karto
	{
		// Code
		super(context);

		setTextSize(48);
		setTextColor(Color.rgb(0, 255, 0));
		setGravity(Gravity.CENTER);
		setText("Hello World!!!");

		// Gesture Related Code
		gestureDetector = new GestureDetector(context, this, null, false);
		gestureDetector.setOnDoubleTapListener(this);
	}

	@Override
	public boolean onTouchEvent(MotionEvent e)
	{
		// Code
		if(!gestureDetector.onTouchEvent(e))
		{
			super.onTouchEvent(e);
			
		}
		return true;
	}

	// 3 methods of onDoubleTapListener interface
	@Override
	public boolean onDoubleTap(MotionEvent e)
	{
		// Code
		setText("Double Tap = SAB");
		return true;
	}
	
	@Override
	public boolean onDoubleTapEvent(MotionEvent e)
	{
		// Code
		return true;
	}

	@Override
	public boolean onSingleTapConfirmed(MotionEvent e)
	{
		// Code
		setText("Single Tap = SAB");
		return true;
	}

	// 6 methods of ontaplistener
	@Override
	public boolean onDown(MotionEvent e)
	{
		// Code
		return true;
	}
	
	@Override
	public boolean onFling(MotionEvent e1, MotionEvent e2, float velocityX, float velocityY)
	{
		// Code
		return true;
	}

	@Override
	public void onLongPress(MotionEvent e)
	{
		// Code
		setText("Long Press = SAB");
	}

	@Override
	public boolean onScroll(MotionEvent e1, MotionEvent e2, float distanceX, float distanceY)  // Swipe
	{
		// Code
		System.out.println("SAB: Swipe");
		System.exit(0);
		return true;
	}

	@Override
	public void onShowPress(MotionEvent e)
	{
		// Code
	}

	@Override
	public boolean onSingleTapUp(MotionEvent e)
	{
		// Code
		return true;
	}
}

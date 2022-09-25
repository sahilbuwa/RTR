package com.sab.window;

import androidx.appcompat.app.AppCompatActivity;
import android.os.Bundle; // Environment Variables Class , char * envp equivalent

import android.graphics.Color;
import android.view.View;
import android.view.Window;
import android.content.pm.ActivityInfo;

public class MainActivity extends AppCompatActivity
{
	private MyView myView;
	@Override
	protected void onCreate(Bundle savedInstanceState)  // WM_CREATE sarkhe
	{
		super.onCreate(savedInstanceState);
		// Doing FullScreen
		getWindow().getDecorView().setSystemUiVisibility(View.SYSTEM_UI_FLAG_FULLSCREEN | 
														 View.SYSTEM_UI_FLAG_HIDE_NAVIGATION |
														 View.SYSTEM_UI_FLAG_IMMERSIVE );
		supportRequestWindowFeature(Window.FEATURE_NO_TITLE);
		// Forced Landscape Orientation
		setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);

		// Setting background color
		getWindow().getDecorView().setBackgroundColor(Color.rgb(0, 0, 0));
		myView = new MyView(this);
		setContentView(myView);
	}
}

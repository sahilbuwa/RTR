package com.sab.window;

import androidx.appcompat.app.AppCompatActivity;
import android.os.Bundle; // Environment Variables Class , char * envp equivalent

import android.graphics.Color;
import android.content.pm.ActivityInfo;

// For fullscreen
import androidx.core.view.WindowCompat;
import androidx.core.view.WindowInsetsControllerCompat;
import androidx.core.view.WindowInsetsCompat;

public class MainActivity extends AppCompatActivity
{
	private GLESView glesView;
	@Override
	protected void onCreate(Bundle savedInstanceState)  // WM_CREATE sarkhe
	{
		super.onCreate(savedInstanceState);
		// Hide action bar
		getSupportActionBar().hide();
		// FullScreen
		WindowCompat.setDecorFitsSystemWindows(getWindow(), false);
		
		// Forced Landscape Orientation
		setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
		
		// Hiding system bars and IME, insets mhanje NC area
		WindowInsetsControllerCompat windowInsetsControllerCompat = WindowCompat.getInsetsController(getWindow(), getWindow().getDecorView());
		windowInsetsControllerCompat.hide(WindowInsetsCompat.Type.systemBars() | WindowInsetsCompat.Type.ime());

		// Setting background color
		getWindow().getDecorView().setBackgroundColor(Color.rgb(0, 0, 0));
		glesView = new GLESView(this);
		setContentView(glesView);
	}

	@Override
	protected void onPause()
	{
		super.onPause();
	}

	@Override
	protected void onResume()
	{
		super.onResume();
	}	
}

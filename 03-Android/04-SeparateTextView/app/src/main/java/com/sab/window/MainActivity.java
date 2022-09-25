package com.sab.window;

import androidx.appcompat.app.AppCompatActivity;
import android.os.Bundle; // Environment Variables Class , char * envp equivalent

import android.graphics.Color;

public class MainActivity extends AppCompatActivity
{
	private MyView myView;
	@Override
	protected void onCreate(Bundle savedInstanceState) 
	{
		super.onCreate(savedInstanceState);
		getWindow().getDecorView().setBackgroundColor(Color.rgb(0, 0, 0));
		myView = new MyView(this);
		setContentView(myView);
	}
}

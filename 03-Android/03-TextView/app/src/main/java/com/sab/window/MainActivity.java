package com.sab.window;

import androidx.appcompat.app.AppCompatActivity;
import android.os.Bundle; // Environment Variables Class , char * envp equivalent
// Text view Related Packages
import androidx.appcompat.widget.AppCompatTextView;
// Color sathi
import android.graphics.Color;
import android.view.Gravity;

public class MainActivity extends AppCompatActivity 
{
	@Override
	protected void onCreate(Bundle savedInstanceState) 
	{
		super.onCreate(savedInstanceState);
		getWindow().getDecorView().setBackgroundColor(Color.rgb(0,0,0));
		// setContentView(R.layout.activity_main);
		AppCompatTextView myView = new AppCompatTextView(this);
		myView.setTextSize(64);
		myView.setTextColor(Color.GREEN);
		myView.setGravity(Gravity.CENTER);
		myView.setText("Hello World!!!");
		setContentView(myView);
	}
}

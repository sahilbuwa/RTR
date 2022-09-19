package com.sab.window;

import androidx.appcompat.widget.AppCompatTextView;
import android.content.Context;
import android.graphics.Color;
import android.view.Gravity;

public class MyView extends AppCompatTextView
{
	MyView(Context context)
	{
		super(context);

		setTextSize(48);
		setTextColor(Color.rgb(0, 255, 0));
		setGravity(Gravity.CENTER);
		setText("Hello World!!!");
	}
}
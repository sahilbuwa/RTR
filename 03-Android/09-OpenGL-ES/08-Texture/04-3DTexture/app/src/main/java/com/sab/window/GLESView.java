package com.sab.window; // Files with this package are aligned with each other.

import android.opengl.GLSurfaceView;
import javax.microedition.khronos.opengles.GL10;
import javax.microedition.khronos.egl.EGLConfig;
import android.opengl.GLES32;
import android.content.Context;

// Event Related
import android.view.GestureDetector;
import android.view.GestureDetector.OnDoubleTapListener;
import android.view.GestureDetector.OnGestureListener;
import android.view.MotionEvent;

// Buffer Related Packages
import java.nio.ByteBuffer; // Native IO, Non-Blocking/Asynchronous IO
import java.nio.ByteOrder;
import java.nio.FloatBuffer;

// Matrix related Packages
import android.opengl.Matrix;

// Texture related Packages
import android.graphics.BitmapFactory;
import android.graphics.Bitmap;
import android.opengl.GLUtils;

public class GLESView extends GLSurfaceView implements OnDoubleTapListener, OnGestureListener, GLSurfaceView.Renderer
{
	private GestureDetector gestureDetector;
	private Context context;
	private int shaderProgramObject;
	private int vao_pyramid[] = new int[1];
	private int vbo_pyramid_position[] = new int[1];
	private int vbo_pyramid_texcoord[] = new int[1];
	private int vao_cube[] = new int[1];
	private int vbo_cube_position[] = new int[1];
	private int vbo_cube_texcoord[] = new int[1];
	private int mvpMatrixUniform;
	private int textureSamplerUniform;
	private int texture_stone[] = new int[1];
	private int texture_kundali[] = new int[1];
	private float anglePyramid = 0.0f;
	private float angleCube = 0.0f;

	private float perspectiveProjectionMatrix[] = new float[16];

	GLESView(Context _context) // All global environmental info about apk in context. system.object la inherit karto
	{
		// Code
		super(_context);
		context = _context;

		setEGLContextClientVersion(3); // OpenGL version 3.2 , based on NDK EGL
		setRenderer(this);
		setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);

		// Gesture Related Code
		gestureDetector = new GestureDetector(_context, this, null, false);
		gestureDetector.setOnDoubleTapListener(this);
	}
	// 3 methods of GLSurfaceview.Renderer
	@Override
	public void onSurfaceCreated(GL10 gl, EGLConfig config)
	{
		String glesVersion = gl.glGetString(GL10.GL_VERSION);
		System.out.println("SAB: "+glesVersion);
		String renderer = gl.glGetString(GL10.GL_RENDERER);
		System.out.println("SAB: "+renderer);
		String glslVersion = gl.glGetString(GLES32.GL_SHADING_LANGUAGE_VERSION);
		System.out.println("SAB: "+glslVersion);

		initialize();
	}

	@Override
	public void onSurfaceChanged(GL10 unused, int width, int height)
	{
		resize(width, height);	
	}

	@Override
	public void onDrawFrame(GL10 unused)
	{
		// onDrawFrame should be considered as game loop
		update();
		display();
	}

	// Custom Private functions
	private void initialize()
	{
		// Vertex Shader
		final String vertexShaderSourceCode = String.format(
			"#version 320 es" +
			"\n" +
			"in vec4 a_position;" +
			"in vec2 a_texcoord;" +
			"uniform mat4 u_mvpMatrix;" +
			"out vec2 a_texcoord_out;" +
			"void main(void)" +
			"{" +
			"gl_Position = u_mvpMatrix * a_position;" +
			"a_texcoord_out = a_texcoord;" +
			"}"
		);
		int vertexShaderObject = GLES32.glCreateShader(GLES32.GL_VERTEX_SHADER);
		GLES32.glShaderSource(vertexShaderObject, vertexShaderSourceCode);
		GLES32.glCompileShader(vertexShaderObject);

		int status[] = new int[1];
		int infoLogLength[] = new int[1];
		String log = null;
		GLES32.glGetShaderiv(vertexShaderObject, GLES32.GL_COMPILE_STATUS, status, 0);
		if(status[0] == GLES32.GL_FALSE)
		{
			GLES32.glGetShaderiv(vertexShaderObject, GLES32.GL_INFO_LOG_LENGTH, infoLogLength, 0);
			if(infoLogLength[0] > 0)
			{
				log = GLES32.glGetShaderInfoLog(vertexShaderObject);
				System.out.println("SAB: Vertex Shader Compilation log : "+log);
				uninitialize();
				System.exit(0);
			}
		}

		// Fragment Shader
		final String fragmentShaderSourceCode = String.format(
			"#version 320 es" +
			"\n" +
			"precision highp float;" +
			"in vec2 a_texcoord_out;" +
			"uniform highp sampler2D u_textureSampler;" +
			"out vec4 FragColor;" +
			"void main(void)" +
			"{" +
			"FragColor = texture(u_textureSampler, a_texcoord_out);" +
			"}"
		);
		int fragmentShaderObject = GLES32.glCreateShader(GLES32.GL_FRAGMENT_SHADER);
		GLES32.glShaderSource(fragmentShaderObject, fragmentShaderSourceCode);
		GLES32.glCompileShader(fragmentShaderObject);

		status[0] = 0;
		infoLogLength[0] = 0;
		log = null;
		GLES32.glGetShaderiv(fragmentShaderObject, GLES32.GL_COMPILE_STATUS, status, 0);
		if(status[0] == GLES32.GL_FALSE)
		{
			GLES32.glGetShaderiv(fragmentShaderObject, GLES32.GL_INFO_LOG_LENGTH, infoLogLength, 0);
			if(infoLogLength[0] > 0)
			{
				log = GLES32.glGetShaderInfoLog(fragmentShaderObject);
				System.out.println("SAB: Fragment Shader Compilation log : "+log);
				uninitialize();
				System.exit(0);
			}
		}
		
		// Shader Program
		shaderProgramObject = GLES32.glCreateProgram();
		GLES32.glAttachShader(shaderProgramObject, vertexShaderObject);
		GLES32.glAttachShader(shaderProgramObject, fragmentShaderObject);
		GLES32.glBindAttribLocation(shaderProgramObject, MyGLESMacros.SAB_ATTRIBUTE_POSITION, "a_position");
		GLES32.glBindAttribLocation(shaderProgramObject, MyGLESMacros.SAB_ATTRIBUTE_TEXTURE0, "a_texcoord");
		GLES32.glLinkProgram(shaderProgramObject);
		
		// Error Checking
		status[0] = 0;
		infoLogLength[0] = 0;
		log = null;
		GLES32.glGetProgramiv(shaderProgramObject, GLES32.GL_LINK_STATUS, status, 0);
		if(status[0] == GLES32.GL_FALSE)
		{
			GLES32.glGetProgramiv(shaderProgramObject, GLES32.GL_INFO_LOG_LENGTH, infoLogLength, 0);
			if(infoLogLength[0] > 0)
			{
				log = GLES32.glGetProgramInfoLog(shaderProgramObject);
				System.out.println("SAB: Shader Program Link log : "+log);
				uninitialize();
				System.exit(0);
			}
		}

		// Post linking Uniforms
		mvpMatrixUniform = GLES32.glGetUniformLocation(shaderProgramObject, "u_mvpMatrix");
		textureSamplerUniform = GLES32.glGetUniformLocation(shaderProgramObject, "u_textureSampler");

		final float position[] = new float[] 
		{
			1.0f, 1.0f, 0.0f,
			-1.0f, 1.0f, 0.0f,
			-1.0f, -1.0f, 0.0f,
			1.0f, -1.0f, 0.0f
		};

		final float texcoords[] = new float[]
		{
			1.0f, 0.0f,
			0.0f, 0.0f,
			0.0f, 1.0f,
			1.0f, 1.0f	
		};
		// Vao
		GLES32.glGenVertexArrays(1, vao, 0);
		GLES32.glBindVertexArray(vao[0]);
		// Vbo Position
		GLES32.glGenBuffers(1, vbo_position, 0);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_position[0]);
		ByteBuffer byteBuffer = ByteBuffer.allocateDirect(position.length * 4); // sizeof replacement
		byteBuffer.order(ByteOrder.nativeOrder());
		FloatBuffer positionBuffer = byteBuffer.asFloatBuffer();
		positionBuffer.put(position);
		positionBuffer.position(0);
		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, position.length * 4, positionBuffer, GLES32.GL_STATIC_DRAW);
		GLES32.glVertexAttribPointer(MyGLESMacros.SAB_ATTRIBUTE_POSITION, 3, GLES32.GL_FLOAT, false, 0, 0);
		GLES32.glEnableVertexAttribArray(MyGLESMacros.SAB_ATTRIBUTE_POSITION);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);

		// Vbo Texcoord
		GLES32.glGenBuffers(1, vbo_texcoord, 0);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_texcoord[0]);
		byteBuffer = ByteBuffer.allocateDirect(texcoords.length * 4); // sizeof replacement
		byteBuffer.order(ByteOrder.nativeOrder());
		FloatBuffer texcoordBuffer = byteBuffer.asFloatBuffer();
		texcoordBuffer.put(texcoords);
		texcoordBuffer.position(0);
		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, texcoords.length * 4, texcoordBuffer, GLES32.GL_STATIC_DRAW);
		GLES32.glVertexAttribPointer(MyGLESMacros.SAB_ATTRIBUTE_TEXTURE0, 2, GLES32.GL_FLOAT, false, 0, 0);
		GLES32.glEnableVertexAttribArray(MyGLESMacros.SAB_ATTRIBUTE_TEXTURE0);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);
		
		GLES32.glBindVertexArray(0);

		// Depth Related Changes
		GLES32.glClearDepthf(1.0f);
		GLES32.glEnable(GLES32.GL_DEPTH_TEST);
		GLES32.glDepthFunc(GLES32.GL_LEQUAL);

		// Enable texture
		GLES32.glEnable(GLES32.GL_TEXTURE_2D);
		// Clearing the screen to black color
		GLES32.glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		// LoadGLTexture call
		texture_smiley[0] = loadGLTexture(R.raw.smiley);
		// Initialization of Projection Matrix
		Matrix.setIdentityM(perspectiveProjectionMatrix, 0); // OpenGL purpose
	}

	private int loadGLTexture(int imageResourceID)
	{
		// Code
		BitmapFactory.Options options = new BitmapFactory.Options();
		options.inScaled = false;

		Bitmap bitmap = BitmapFactory.decodeResource(context.getResources(), imageResourceID, options);
		int texture[] = new int[1];
		GLES32.glPixelStorei(GLES32.GL_UNPACK_ALIGNMENT, 1);
		GLES32.glGenTextures(1, texture, 0);
		GLES32.glBindTexture(GLES32.GL_TEXTURE_2D, texture[0]);
		GLES32.glTexParameteri(GLES32.GL_TEXTURE_2D, GLES32.GL_TEXTURE_MAG_FILTER, GLES32.GL_LINEAR);
		GLES32.glTexParameteri(GLES32.GL_TEXTURE_2D, GLES32.GL_TEXTURE_MIN_FILTER, GLES32.GL_LINEAR_MIPMAP_LINEAR);

		// Create The Texture
		GLUtils.texImage2D(GLES32.GL_TEXTURE_2D, 0, bitmap, 0);
		GLES32.glGenerateMipmap(GLES32.GL_TEXTURE_2D);

		GLES32.glBindTexture(GLES32.GL_TEXTURE_2D, 0);

		return texture[0];
	}

	private void resize(int width, int height)
	{
		// Code
		if(height == 0)
			height = 1;
		GLES32.glViewport(0, 0, width, height);
		Matrix.perspectiveM(perspectiveProjectionMatrix, 0, 45.0f, (float)width / (float)height, 0.1f, 100.0f);
	}

	private void display()
	{
		// Code
		GLES32.glClear(GLES32.GL_COLOR_BUFFER_BIT | GLES32.GL_DEPTH_BUFFER_BIT);
		GLES32.glUseProgram(shaderProgramObject);
		// Transformations
		float translationMatrix[] = new float[16];
		Matrix.setIdentityM(translationMatrix, 0);
		Matrix.translateM(translationMatrix, 0, 0.0f, 0.0f, -6.0f);
		float modelMatrix[] = new float[16];
		Matrix.setIdentityM(modelMatrix, 0);
		float modelViewMatrix[] = new float[16];
		Matrix.setIdentityM(modelViewMatrix, 0);
		float modelViewProjectionMatrix[] = new float[16];
		Matrix.setIdentityM(modelViewProjectionMatrix, 0);
		Matrix.multiplyMM(modelMatrix, 0, modelMatrix, 0, translationMatrix, 0);
		Matrix.multiplyMM(modelViewMatrix, 0, modelViewMatrix, 0, modelMatrix, 0);
		Matrix.multiplyMM(modelViewProjectionMatrix , 0, perspectiveProjectionMatrix, 0, modelViewMatrix, 0);

		GLES32.glUniformMatrix4fv(mvpMatrixUniform, 1, false, modelViewProjectionMatrix, 0);
		
		GLES32.glBindVertexArray(vao[0]);

		GLES32.glActiveTexture(GLES32.GL_TEXTURE0);
		GLES32.glBindTexture(GLES32.GL_TEXTURE_2D, texture_smiley[0]);
		GLES32.glUniform1i(textureSamplerUniform, 0);
		// Drawing Code
		GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN, 0, 4);

		GLES32.glBindVertexArray(0);

		// Unuse the shader program object
		GLES32.glUseProgram(0);

		// UnuseProgram
		GLES32.glUseProgram(0);
		requestRender(); // SwapBuffers()
	}

	private void update()
	{
		// Code
	}

	private void uninitialize()
	{
		// Code
		if(shaderProgramObject > 0)
		{
			GLES32.glUseProgram(shaderProgramObject);
			int retVal[] = new int[1];
			GLES32.glGetProgramiv(shaderProgramObject, GLES32.GL_ATTACHED_SHADERS, retVal, 0);
			if(retVal[0] > 0)
			{
				int numAttachedShaders = retVal[0];
				int shaderObjects[] = new int[numAttachedShaders];
				GLES32.glGetAttachedShaders(shaderProgramObject, numAttachedShaders, retVal, 0, shaderObjects, 0);
				for(int i = 0; i < numAttachedShaders; i++)
				{
					GLES32.glDetachShader(shaderProgramObject, shaderObjects[i]);
					GLES32.glDeleteShader(shaderObjects[i]);
					shaderObjects[i] = 0;
				}
			}
			GLES32.glUseProgram(0);
			GLES32.glDeleteProgram(shaderProgramObject);
			shaderProgramObject = 0;	
		}
		if(texture_smiley[0] > 0)
		{
			GLES32.glDeleteTextures(1, texture_smiley, 0);
			texture_smiley[0] = 0;
		}
		if(vbo_texcoord[0] > 0)
		{
			GLES32.glDeleteBuffers(1, vbo_texcoord, 0);
			vbo_texcoord[0] = 0;
		}
		if(vbo_position[0] > 0)
		{
			GLES32.glDeleteBuffers(1, vbo_position, 0);
			vbo_position[0] = 0;
		}
		if(vao[0] > 0)
		{
			GLES32.glDeleteVertexArrays(1, vao, 0);
			vao[0] = 0;
		}
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
	}

	@Override
	public boolean onScroll(MotionEvent e1, MotionEvent e2, float distanceX, float distanceY)  // Swipe
	{
		// Code
		uninitialize();
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

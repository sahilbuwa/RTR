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

// Math related Packages
import java.lang.Math;

public class GLESView extends GLSurfaceView implements OnDoubleTapListener, OnGestureListener, GLSurfaceView.Renderer
{
	private GestureDetector gestureDetector;
	private Context context;
	private int shaderProgramObject;
	private int vao_pyramid[] = new int[1];
	private int vbo_pyramid_position[] = new int[1];
	private int vbo_pyramid_normal[] = new int[1];

	private int modelMatrixUniform = 0;
	private int viewMatrixUniform = 0;
	private int projectionMatrixUniform = 0;
	private int ldUniform = 0;
	private int kdUniform = 0;
	private int lightPositionUniform = 0;
	private int lightingEnableUniform = 0;
	private float anglePyramid = 0.0f;
	private float[] lightDiffuse = new float[] {1.0f, 1.0f, 1.0f, 1.0f};
	private float[] materialDiffuse = new float[] {0.5f, 0.5f, 0.5f, 1.0f};
	private float[] lightPosition = new float[] {0.0f, 0.0f, 2.0f, 1.0f};
	private int doubleTapCount = 0;
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
			"precision mediump int;" +
			"in vec4 a_position;" +
			"in vec3 a_normal;" +
			"uniform mat4 u_modelMatrix;" +
			"uniform mat4 u_viewMatrix;" +
			"uniform mat4 u_projectionMatrix;" +
			"uniform vec3 u_ld;" +
			"uniform vec3 u_kd;" +
			"uniform vec4 u_lightPosition;" +
			"uniform int u_lightingEnabled;" +
			"out vec3 diffuse_light_color;" +
			"void main(void)" +
			"{" +
			"if(u_lightingEnabled == 1)" +
			"{" +
			"vec4 eyeCoordinates = u_viewMatrix * u_modelMatrix * a_position;" +
			"mat3 normalMatrix = mat3(transpose(inverse(u_viewMatrix * u_modelMatrix)));" +
			"vec3 transformedNormals = normalize(normalMatrix * a_normal);" +
			"vec3 lightDirection = normalize(vec3(u_lightPosition - eyeCoordinates));" +
			"diffuse_light_color = u_ld * u_kd * max(dot(lightDirection, transformedNormals), 0.0);" +
			"}" +
			"gl_Position =  u_projectionMatrix * u_viewMatrix * u_modelMatrix * a_position;" +
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
			"precision mediump int;" +
			"precision highp float;" +
			"in vec3 diffuse_light_color;" +
			"uniform int u_lightingEnabled;" +
			"out vec4 FragColor;" +
			"void main(void)" +
			"{" +
			"if(u_lightingEnabled == 1)" +
			"{" +
			"FragColor = vec4(diffuse_light_color, 1.0);" +
			"}" +
			"else" +
			"{" +
			"FragColor = vec4(1.0, 1.0, 1.0, 1.0);" +
			"}" +
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
		GLES32.glBindAttribLocation(shaderProgramObject, MyGLESMacros.SAB_ATTRIBUTE_NORMAL, "a_normal");
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
		modelMatrixUniform = GLES32.glGetUniformLocation(shaderProgramObject, "u_modelMatrix");
		viewMatrixUniform = GLES32.glGetUniformLocation(shaderProgramObject, "u_viewMatrix");
		projectionMatrixUniform = GLES32.glGetUniformLocation(shaderProgramObject, "u_projectionMatrix");
	
		ldUniform = GLES32.glGetUniformLocation(shaderProgramObject, "u_ld");
		kdUniform = GLES32.glGetUniformLocation(shaderProgramObject, "u_kd");
		lightPositionUniform = GLES32.glGetUniformLocation(shaderProgramObject, "u_lightPosition");
		lightingEnableUniform = GLES32.glGetUniformLocation(shaderProgramObject, "u_lightingEnabled");

		final float pyramidPosition[] = new float[] 
		{
			// front
			0.0f, 1.0f, 0.0f,
			-1.0f, -1.0f, 1.0f,
			1.0f, -1.0f, 1.0f,
	
			// right
			0.0f, 1.0f, 0.0f,
			1.0f, -1.0f, 1.0f,
			1.0f, -1.0f, -1.0f,
	
			// back
			0.0f, 1.0f, 0.0f,
			1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
	
			// left
			0.0f, 1.0f, 0.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f, 1.0f
		};


		final float pyramidNormals[] = new float[]
		{
			0.0f, 0.447214f, 0.894427f,// front-top
			0.0f, 0.447214f, 0.894427f,// front-left
			0.0f, 0.447214f, 0.894427f,// front-right
	
			0.894427f, 0.447214f, 0.0f, // right-top
			0.894427f, 0.447214f, 0.0f, // right-left
			0.894427f, 0.447214f, 0.0f, // right-right
	
			0.0f, 0.447214f, -0.894427f, // back-top
			0.0f, 0.447214f, -0.894427f, // back-left
			0.0f, 0.447214f, -0.894427f, // back-right
	
			-0.894427f, 0.447214f, 0.0f, // left-top
			-0.894427f, 0.447214f, 0.0f, // left-left
			-0.894427f, 0.447214f, 0.0f // left-right
		};
		// Vao
		GLES32.glGenVertexArrays(1, vao_pyramid, 0);
		GLES32.glBindVertexArray(vao_pyramid[0]);
		// Vbo Position
		GLES32.glGenBuffers(1, vbo_pyramid_position, 0);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_pyramid_position[0]);
		ByteBuffer byteBuffer = ByteBuffer.allocateDirect(pyramidPosition.length * 4); // sizeof replacement
		byteBuffer.order(ByteOrder.nativeOrder());
		FloatBuffer positionBuffer = byteBuffer.asFloatBuffer();
		positionBuffer.put(pyramidPosition);
		positionBuffer.position(0);
		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, pyramidPosition.length * 4, positionBuffer, GLES32.GL_STATIC_DRAW);
		GLES32.glVertexAttribPointer(MyGLESMacros.SAB_ATTRIBUTE_POSITION, 3, GLES32.GL_FLOAT, false, 0, 0);
		GLES32.glEnableVertexAttribArray(MyGLESMacros.SAB_ATTRIBUTE_POSITION);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);

		// Vbo Normals
		GLES32.glGenBuffers(1, vbo_pyramid_normal, 0);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_pyramid_normal[0]);
		byteBuffer = ByteBuffer.allocateDirect(pyramidNormals.length * 4); // sizeof replacement
		byteBuffer.order(ByteOrder.nativeOrder());
		FloatBuffer normalBuffer = byteBuffer.asFloatBuffer();
		normalBuffer.put(pyramidNormals);
		normalBuffer.position(0);
		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, pyramidNormals.length * 4, normalBuffer, GLES32.GL_STATIC_DRAW);
		GLES32.glVertexAttribPointer(MyGLESMacros.SAB_ATTRIBUTE_NORMAL, 3, GLES32.GL_FLOAT, false, 0, 0);
		GLES32.glEnableVertexAttribArray(MyGLESMacros.SAB_ATTRIBUTE_NORMAL);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);
		
		GLES32.glBindVertexArray(0);

		// Depth Related Changes
		GLES32.glClearDepthf(1.0f);
		GLES32.glEnable(GLES32.GL_DEPTH_TEST);
		GLES32.glDepthFunc(GLES32.GL_LEQUAL);
		
		// Clearing the screen to black color
		GLES32.glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		// Initialization of Projection Matrix
		Matrix.setIdentityM(perspectiveProjectionMatrix, 0); // OpenGL purpose
	}

	private void resize(int width, int height)
	{
		if(height == 0)
			height = 1;
		GLES32.glViewport(0, 0, width, height);
		Matrix.perspectiveM(perspectiveProjectionMatrix, 0, 45.0f, (float)width / (float)height, 0.1f, 100.0f);
	}

	private void display()
	{
		GLES32.glClear(GLES32.GL_COLOR_BUFFER_BIT | GLES32.GL_DEPTH_BUFFER_BIT);
		GLES32.glUseProgram(shaderProgramObject);

		// Transformations
		float translationMatrix[] = new float[16];
		Matrix.setIdentityM(translationMatrix, 0);
		Matrix.translateM(translationMatrix, 0, 0.0f, 0.0f, -4.0f);
		float rotationMatrix[] = new float[16];
		Matrix.setIdentityM(rotationMatrix, 0);
		Matrix.rotateM(rotationMatrix, 0, anglePyramid, 0.0f, 1.0f, 0.0f);
		float modelMatrix[] = new float[16];
		Matrix.setIdentityM(modelMatrix, 0);
		float viewMatrix[] = new float[16];
		Matrix.setIdentityM(viewMatrix, 0);
		Matrix.multiplyMM(modelMatrix, 0, translationMatrix, 0, rotationMatrix, 0);

		GLES32.glUniformMatrix4fv(modelMatrixUniform, 1, false, modelMatrix, 0);
		GLES32.glUniformMatrix4fv(viewMatrixUniform, 1, false, viewMatrix, 0);
		GLES32.glUniformMatrix4fv(projectionMatrixUniform, 1, false, perspectiveProjectionMatrix, 0);
		
		if(doubleTapCount == 1)
		{
			GLES32.glUniform1i(lightingEnableUniform, 1);

			GLES32.glUniform3fv(ldUniform, 1, lightDiffuse, 0);
			GLES32.glUniform3fv(kdUniform, 1, materialDiffuse, 0);
			GLES32.glUniform4fv(lightPositionUniform, 1, lightPosition, 0);
		}
		else
		{
			GLES32.glUniform1i(lightingEnableUniform, 0);
		}
		
		GLES32.glBindVertexArray(vao_pyramid[0]);
		// Drawing Code
		GLES32.glDrawArrays(GLES32.GL_TRIANGLES, 0, 12);

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
		anglePyramid += 1.0f;
		if(anglePyramid >= 360.0f)
			anglePyramid -= 360.0f;
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
		if(vbo_pyramid_normal[0] > 0)
		{
			GLES32.glDeleteBuffers(1, vbo_pyramid_normal, 0);
			vbo_pyramid_normal[0] = 0;
		}
		if(vbo_pyramid_position[0] > 0)
		{
			GLES32.glDeleteBuffers(1, vbo_pyramid_position, 0);
			vbo_pyramid_position[0] = 0;
		}
		if(vao_pyramid[0] > 0)
		{
			GLES32.glDeleteVertexArrays(1, vao_pyramid, 0);
			vao_pyramid[0] = 0;
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
		doubleTapCount++;
		if(doubleTapCount > 1)
			doubleTapCount = 0;
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

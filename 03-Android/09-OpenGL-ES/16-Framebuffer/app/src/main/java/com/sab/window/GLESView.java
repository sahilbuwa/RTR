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
import java.nio.ShortBuffer;

// Matrix related Packages
import android.opengl.Matrix;

// Math related packages
import java.lang.Math;

public class GLESView extends GLSurfaceView implements OnDoubleTapListener, OnGestureListener, GLSurfaceView.Renderer
{
	private final int FBO_WIDTH = 1024;
	private final int FBO_HEIGHT = 1024;
	private GestureDetector gestureDetector;
	private Context context;
	private int shaderProgramObject;
	private int shaderProgramObject_sphere;

	private int vao_cube[] = new int[1];
	private int vbo_cube_position[] = new int[1];
	private int vbo_cube_texcoord[] = new int[1];
	private int mvpMatrixUniform;
	private int textureSamplerUniform;
	private float angleCube = 0.0f;
	private float perspectiveProjectionMatrix[] = new float[16];
	private int gWidth = 0;
	private int gHeight = 0;

	// FBO 
	private int fbo[] = new int[1];
	private int rbo[] = new int[1];
	private int fbo_texture[] = new int[1];
	private boolean fboResult = false;

	private int modelMatrixUniform_sphere = 0;
	private int viewMatrixUniform_sphere = 0;
	private int projectionMatrixUniform_sphere = 0;
	private int ldUniform_sphere[] = new int[3];
	private int laUniform_sphere[] = new int[3];
	private int lsUniform_sphere[] = new int[3];
	private int lightPositionUniform_sphere[] = new int[3];

	private int kaUniform_sphere = 0;
	private int kdUniform_sphere = 0;
	private int ksUniform_sphere = 0;
	private int materialShininessUniform_sphere = 0;

	private int lightingEnableUniform_sphere = 0;
	
	private class Light
	{
		float lightAmbient[] = new float[4];
		float lightDiffuse[] = new float[4];
		float lightSpecular[] = new float[4];
		float lightPosition[] = new float[4];
	}
	private Light lights[] = new Light[3];

	private float[] materialAmbient = new float[] {0.0f, 0.0f, 0.0f, 1.0f};
	private float[] materialDiffuse = new float[] {1.0f, 1.0f, 1.0f, 1.0f};
	private float[] materialSpecular = new float[] {1.0f, 1.0f, 1.0f, 1.0f};
	private float materialShininess = 50.0f;

	private int doubleTapCount = 0;

	private int[] vao_sphere = new int[1];
	private int[] vbo_sphere_position = new int[1];
	private int[] vbo_sphere_normal = new int[1];
	private int[] vbo_sphere_element = new int[1];
	private int numVertices = 0;
	private int numElements = 0;

	private float redLightAngle = 0.0f;
	private float greenLightAngle = 0.0f;
	private float blueLightAngle = 0.0f;

	private float perspectiveProjectionMatrix_sphere[] = new float[16];

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
		gWidth = width;
		gHeight = height;
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
		// Code
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

		final float cubePosition[] = new float[] 
		{
			// top
			1.0f, 1.0f, -1.0f,
			-1.0f, 1.0f, -1.0f, 
			-1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,  
	
			// bottom
			1.0f, -1.0f, -1.0f,
		   -1.0f, -1.0f, -1.0f,
		   -1.0f, -1.0f,  1.0f,
			1.0f, -1.0f,  1.0f,
	
			// front
			1.0f, 1.0f, 1.0f,
		   -1.0f, 1.0f, 1.0f,
		   -1.0f, -1.0f, 1.0f,
			1.0f, -1.0f, 1.0f,
	
			// back
			1.0f, 1.0f, -1.0f,
		   -1.0f, 1.0f, -1.0f,
		   -1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
	
			// right
			1.0f, 1.0f, -1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 1.0f,
			1.0f, -1.0f, -1.0f,
	
			// left
			-1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f, -1.0f, 
			-1.0f, -1.0f, -1.0f, 
			-1.0f, -1.0f, 1.0f,
		};

		final float cubeTexcoords[] = new float[]
		{
			0.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 1.0f,
			0.0f, 1.0f,
	
			0.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 1.0f,
			0.0f, 1.0f,
	
			0.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 1.0f,
			0.0f, 1.0f,
			// Bottom
			0.0f, 0.0f,
			0.0f, 1.0f,
			1.0f, 1.0f,
			1.0f, 0.0f,
	
			0.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 1.0f,
			0.0f, 1.0f,
	
			0.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 1.0f,
			0.0f, 1.0f
		};

		// Vao Cube
		GLES32.glGenVertexArrays(1, vao_cube, 0);
		GLES32.glBindVertexArray(vao_cube[0]);
		// Vbo Position
		GLES32.glGenBuffers(1, vbo_cube_position, 0);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_cube_position[0]);
		ByteBuffer byteBuffer = ByteBuffer.allocateDirect(cubePosition.length * 4); // sizeof replacement
		byteBuffer.order(ByteOrder.nativeOrder());
		FloatBuffer positionBuffer = byteBuffer.asFloatBuffer();
		positionBuffer.put(cubePosition);
		positionBuffer.position(0);
		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, cubePosition.length * 4, positionBuffer, GLES32.GL_STATIC_DRAW);
		GLES32.glVertexAttribPointer(MyGLESMacros.SAB_ATTRIBUTE_POSITION, 3, GLES32.GL_FLOAT, false, 0, 0);
		GLES32.glEnableVertexAttribArray(MyGLESMacros.SAB_ATTRIBUTE_POSITION);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);

		// Vbo Texcoord
		GLES32.glGenBuffers(1, vbo_cube_texcoord, 0);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_cube_texcoord[0]);
		byteBuffer = ByteBuffer.allocateDirect(cubeTexcoords.length * 4); // sizeof replacement
		byteBuffer.order(ByteOrder.nativeOrder());
		FloatBuffer texcoordBuffer = byteBuffer.asFloatBuffer();
		texcoordBuffer.put(cubeTexcoords);
		texcoordBuffer.position(0);
		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, cubeTexcoords.length * 4, texcoordBuffer, GLES32.GL_STATIC_DRAW);
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
		// Clearing the screen to white color
		GLES32.glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

		// Initialization of Projection Matrix
		Matrix.setIdentityM(perspectiveProjectionMatrix, 0); // OpenGL purpose
		fboResult = createFBO(FBO_WIDTH, FBO_HEIGHT);
		if(fboResult)
			initialize_sphere();
		else
			System.out.println("SAB: Create FBO failed.\n");
	}

	private boolean createFBO(int textureWidth, int textureHeight)
	{
		// Local Variable Declarations
		int maxRenderbufferSize[] = new int[1];
		// Code
		// Step 1: Check available renderbuffer size.
		GLES32.glGetIntegerv(GLES32.GL_MAX_RENDERBUFFER_SIZE, maxRenderbufferSize, 0);
		if(maxRenderbufferSize[0] < textureWidth || maxRenderbufferSize[0] < textureHeight)
		{
			System.out.println("SAB: Insufficient renderbuffer size.\n");
			return false;
		}
		// Step 2: Create framebuffer object.
		GLES32.glGenFramebuffers(1, fbo, 0);
		GLES32.glBindFramebuffer(GLES32.GL_FRAMEBUFFER, fbo[0]);
		// Step 3: Create renderbuffer object.
		GLES32.glGenRenderbuffers(1, rbo, 0);
		GLES32.glBindRenderbuffer(GLES32.GL_RENDERBUFFER, rbo[0]);
		// Step 4: Storage and format of renderbuffer
		GLES32.glRenderbufferStorage(GLES32.GL_RENDERBUFFER, GLES32.GL_DEPTH_COMPONENT16, textureWidth, textureHeight);
		// Step 5: Create empty texture for upcoming scene
		GLES32.glGenTextures(1, fbo_texture, 0);
		GLES32.glBindTexture(GLES32.GL_TEXTURE_2D, fbo_texture[0]);

		GLES32.glTexParameteri(GLES32.GL_TEXTURE_2D, GLES32.GL_TEXTURE_WRAP_S, GLES32.GL_CLAMP_TO_EDGE);
		GLES32.glTexParameteri(GLES32.GL_TEXTURE_2D, GLES32.GL_TEXTURE_WRAP_T, GLES32.GL_CLAMP_TO_EDGE);
		GLES32.glTexParameteri(GLES32.GL_TEXTURE_2D, GLES32.GL_TEXTURE_MAG_FILTER, GLES32.GL_LINEAR);
		GLES32.glTexParameteri(GLES32.GL_TEXTURE_2D, GLES32.GL_TEXTURE_MIN_FILTER, GLES32.GL_LINEAR);

		GLES32.glTexImage2D(GLES32.GL_TEXTURE_2D, 0, GLES32.GL_RGB, textureWidth, textureHeight, 0, GLES32.GL_RGB, GLES32.GL_UNSIGNED_SHORT_5_6_5, null); // 2nd last param matches with GL_DEPTH_COMPONENT16.
		// Step 6: Give above texture to fbo
		GLES32.glFramebufferTexture2D(GLES32.GL_FRAMEBUFFER, GLES32.GL_COLOR_ATTACHMENT0, GLES32.GL_TEXTURE_2D, fbo_texture[0], 0);
		// Step 7 : Give renderbuffer to fbo.
		GLES32.glFramebufferRenderbuffer(GLES32.GL_FRAMEBUFFER, GLES32.GL_DEPTH_ATTACHMENT, GLES32.GL_RENDERBUFFER, rbo[0]);
		// Step 8: Check whether the framebuffer is create successfully or not.
		int result = GLES32.glCheckFramebufferStatus(GLES32.GL_FRAMEBUFFER);
		if(result != GLES32.GL_FRAMEBUFFER_COMPLETE)
		{
			System.out.println("SAB: Framebuffer is not complete.\n");
			return false;
		}
		GLES32.glBindFramebuffer(GLES32.GL_FRAMEBUFFER, 0); // Texture and rbo will be detached automatically.
		return true;
	}

	private void initialize_sphere()
	{
		// Vertex Shader Per Fragment
		final String vertexShaderSourceCodeF = String.format(
			"#version 320 es" +
			"\n" +
			"precision mediump int;" +
			"precision highp float;" +
			"in vec4 a_position;\n" +
			"in vec3 a_normal;\n" +
			"uniform mat4 u_modelMatrix;\n" +
			"uniform mat4 u_viewMatrix;\n" +
			"uniform mat4 u_projectionMatrix;\n" +
			"uniform int u_lightingEnabled;\n" +
			"out vec3 transformedNormals;\n" +
			"out vec3 viewerVector;\n" +
			"void main(void)\n" +
			"{\n" +
			"if(u_lightingEnabled == 1)\n" +
			"{\n" +
			"vec4 eyeCoordinates = u_viewMatrix * u_modelMatrix * a_position;\n" +
			"mat3 normalMatrix = mat3(u_viewMatrix * u_modelMatrix);\n" +
			"transformedNormals = normalMatrix * a_normal;\n" +
			"viewerVector = -eyeCoordinates.xyz;\n" +
			"}\n" +
			"gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * a_position;\n" +
			"}\n"
		);
		int vertexShaderObjectF = GLES32.glCreateShader(GLES32.GL_VERTEX_SHADER);
		GLES32.glShaderSource(vertexShaderObjectF, vertexShaderSourceCodeF);
		GLES32.glCompileShader(vertexShaderObjectF);

		int status[] = new int[1];
		int infoLogLength[] = new int[1];
		String log = null;
		GLES32.glGetShaderiv(vertexShaderObjectF, GLES32.GL_COMPILE_STATUS, status, 0);
		if(status[0] == GLES32.GL_FALSE)
		{
			GLES32.glGetShaderiv(vertexShaderObjectF, GLES32.GL_INFO_LOG_LENGTH, infoLogLength, 0);
			if(infoLogLength[0] > 0)
			{
				log = GLES32.glGetShaderInfoLog(vertexShaderObjectF);
				System.out.println("SAB: Vertex Shader Compilation log : "+log);
				uninitialize();
				System.exit(0);
			}
		}

		// Fragment Shader
		final String fragmentShaderSourceCodeF = String.format(
			"#version 320 es" +
			"\n" +
			"precision mediump int;" +
			"precision highp float;" +
			"in vec3 transformedNormals;\n" +
			"in vec3 viewerVector;\n" +
			"uniform vec3 u_la[3];\n" +
			"uniform vec3 u_ld[3];\n" +
			"uniform vec3 u_ls[3];\n" +
			"uniform vec4 u_lightPosition[3];\n" +
			"uniform vec3 u_ka;\n" +
			"uniform vec3 u_kd;\n" +
			"uniform vec3 u_ks;\n" +
			"uniform float u_materialShininess;\n" +
			"uniform int u_lightingEnabled;\n" +
			"out vec4 FragColor;\n" +
			"void main(void)\n" +
			"{\n" +
			"vec4 fong_ads_light;" +
			"if(u_lightingEnabled == 1)\n" +
			"{\n" +
			"vec3 ambient[3];\n" +
			"vec3 diffuse[3];\n" +
			"vec3 specular[3];\n" +
			"vec3 normalized_lightDirection[3];\n" +
			"vec3 reflectionVector[3];\n" +
			"vec3 normalized_transformedNormals = normalize(transformedNormals);\n" +
			"vec3 normalized_viewerVector = normalize(viewerVector);\n" +
			"for(int i=0;i<3;i++)\n" +
			"{\n" +
			"ambient[i] = u_la[i] * u_ka;\n" +
			"normalized_lightDirection[i] = normalize(vec3(u_lightPosition[i]));\n" +
			"diffuse[i] = u_ld[i] * u_kd * max(dot(normalized_lightDirection[i], normalized_transformedNormals), 0.0);\n" +
			"reflectionVector[i] = reflect(-normalized_lightDirection[i], normalized_transformedNormals);\n" +
			"specular[i] = u_ls[i] * u_ks * pow(max(dot(reflectionVector[i], normalized_viewerVector), 0.0), u_materialShininess);\n" +
			"fong_ads_light = fong_ads_light + vec4(ambient[i] + diffuse[i] + specular[i], 1.0);\n" +
			"}\n" +
			"}\n" +
			"else\n" +
			"{\n" +
			"fong_ads_light = vec4(1.0, 1.0, 1.0, 1.0);\n" +
			"}\n" +
			"FragColor = fong_ads_light;\n" +
			"}\n"
		);
		int fragmentShaderObjectF = GLES32.glCreateShader(GLES32.GL_FRAGMENT_SHADER);
		GLES32.glShaderSource(fragmentShaderObjectF, fragmentShaderSourceCodeF);
		GLES32.glCompileShader(fragmentShaderObjectF);

		status[0] = 0;
		infoLogLength[0] = 0;
		log = null;
		GLES32.glGetShaderiv(fragmentShaderObjectF, GLES32.GL_COMPILE_STATUS, status, 0);
		if(status[0] == GLES32.GL_FALSE)
		{
			GLES32.glGetShaderiv(fragmentShaderObjectF, GLES32.GL_INFO_LOG_LENGTH, infoLogLength, 0);
			if(infoLogLength[0] > 0)
			{
				log = GLES32.glGetShaderInfoLog(fragmentShaderObjectF);
				System.out.println("SAB: Fragment Shader Compilation log : "+log);
				uninitialize();
				System.exit(0);
			}
		}
		
		// Shader Program
		shaderProgramObject_sphere = GLES32.glCreateProgram();
		GLES32.glAttachShader(shaderProgramObject_sphere, vertexShaderObjectF);
		GLES32.glAttachShader(shaderProgramObject_sphere, fragmentShaderObjectF);
		GLES32.glBindAttribLocation(shaderProgramObject_sphere, MyGLESMacros.SAB_ATTRIBUTE_POSITION, "a_position");
		GLES32.glBindAttribLocation(shaderProgramObject_sphere, MyGLESMacros.SAB_ATTRIBUTE_NORMAL, "a_normal");
		GLES32.glLinkProgram(shaderProgramObject_sphere);
		
		// Error Checking
		status[0] = 0;
		infoLogLength[0] = 0;
		log = null;
		GLES32.glGetProgramiv(shaderProgramObject_sphere, GLES32.GL_LINK_STATUS, status, 0);
		if(status[0] == GLES32.GL_FALSE)
		{
			GLES32.glGetProgramiv(shaderProgramObject_sphere, GLES32.GL_INFO_LOG_LENGTH, infoLogLength, 0);
			if(infoLogLength[0] > 0)
			{
				log = GLES32.glGetProgramInfoLog(shaderProgramObject_sphere);
				System.out.println("SAB: Shader Program Link log : "+log);
				uninitialize();
				System.exit(0);
			}
		}
		// Post linking Uniforms Fragment
		modelMatrixUniform_sphere = GLES32.glGetUniformLocation(shaderProgramObject_sphere, "u_modelMatrix");
		viewMatrixUniform_sphere = GLES32.glGetUniformLocation(shaderProgramObject_sphere, "u_viewMatrix");
		projectionMatrixUniform_sphere = GLES32.glGetUniformLocation(shaderProgramObject_sphere, "u_projectionMatrix");
	
		laUniform_sphere[0] = GLES32.glGetUniformLocation(shaderProgramObject_sphere, "u_la[0]");
		ldUniform_sphere[0] = GLES32.glGetUniformLocation(shaderProgramObject_sphere, "u_ld[0]");
		lsUniform_sphere[0] = GLES32.glGetUniformLocation(shaderProgramObject_sphere, "u_ls[0]");
		lightPositionUniform_sphere[0] = GLES32.glGetUniformLocation(shaderProgramObject_sphere, "u_lightPosition[0]");

		laUniform_sphere[1] = GLES32.glGetUniformLocation(shaderProgramObject_sphere, "u_la[1]");
		ldUniform_sphere[1] = GLES32.glGetUniformLocation(shaderProgramObject_sphere, "u_ld[1]");
		lsUniform_sphere[1] = GLES32.glGetUniformLocation(shaderProgramObject_sphere, "u_ls[1]");
		lightPositionUniform_sphere[1] = GLES32.glGetUniformLocation(shaderProgramObject_sphere, "u_lightPosition[1]");

		laUniform_sphere[2] = GLES32.glGetUniformLocation(shaderProgramObject_sphere, "u_la[2]");
		ldUniform_sphere[2] = GLES32.glGetUniformLocation(shaderProgramObject_sphere, "u_ld[2]");
		lsUniform_sphere[2] = GLES32.glGetUniformLocation(shaderProgramObject_sphere, "u_ls[2]");
		lightPositionUniform_sphere[2] = GLES32.glGetUniformLocation(shaderProgramObject_sphere, "u_lightPosition[2]");

		kaUniform_sphere = GLES32.glGetUniformLocation(shaderProgramObject_sphere, "u_ka");
		kdUniform_sphere = GLES32.glGetUniformLocation(shaderProgramObject_sphere, "u_kd");
		ksUniform_sphere = GLES32.glGetUniformLocation(shaderProgramObject_sphere, "u_ks");
		materialShininessUniform_sphere = GLES32.glGetUniformLocation(shaderProgramObject_sphere, "u_materialShininess");

		lightingEnableUniform_sphere = GLES32.glGetUniformLocation(shaderProgramObject_sphere, "u_lightingEnabled");

		Sphere sphere = new Sphere();
		float sphere_vertices[] = new float[1146];
		float sphere_normals[] = new float[1146];
		float sphere_textures[] = new float[764];
		short sphere_elements[] = new short[2280];
		sphere.getSphereVertexData(sphere_vertices, sphere_normals, sphere_textures, sphere_elements);
		numVertices = sphere.getNumberOfSphereVertices();
		numElements = sphere.getNumberOfSphereElements();

		// vao
		GLES32.glGenVertexArrays(1, vao_sphere, 0);
		GLES32.glBindVertexArray(vao_sphere[0]);
		
		// position vbo
		GLES32.glGenBuffers(1, vbo_sphere_position, 0);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_sphere_position[0]);
		
		ByteBuffer byteBuffer = ByteBuffer.allocateDirect(sphere_vertices.length * 4);
		byteBuffer.order(ByteOrder.nativeOrder());
		FloatBuffer verticesBuffer=byteBuffer.asFloatBuffer();
		verticesBuffer.put(sphere_vertices);
		verticesBuffer.position(0);
		
		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER,
							sphere_vertices.length * 4,
							verticesBuffer,
							GLES32.GL_STATIC_DRAW);
		
		GLES32.glVertexAttribPointer(MyGLESMacros.SAB_ATTRIBUTE_POSITION,
									 3,
									 GLES32.GL_FLOAT,
									 false,0,0);
		
		GLES32.glEnableVertexAttribArray(MyGLESMacros.SAB_ATTRIBUTE_POSITION);
		
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);
		
		// normal vbo
		GLES32.glGenBuffers(1,vbo_sphere_normal, 0);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_sphere_normal[0]);
		
		byteBuffer = ByteBuffer.allocateDirect(sphere_normals.length * 4);
		byteBuffer.order(ByteOrder.nativeOrder());
		verticesBuffer = byteBuffer.asFloatBuffer();
		verticesBuffer.put(sphere_normals);
		verticesBuffer.position(0);
		
		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER,
							sphere_normals.length * 4,
							verticesBuffer,
							GLES32.GL_STATIC_DRAW);
		
		GLES32.glVertexAttribPointer(MyGLESMacros.SAB_ATTRIBUTE_NORMAL,
									 3,
									 GLES32.GL_FLOAT,
									 false,0,0);
		
		GLES32.glEnableVertexAttribArray(MyGLESMacros.SAB_ATTRIBUTE_NORMAL);
		
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER,0);
		
		// element vbo
		GLES32.glGenBuffers(1, vbo_sphere_element, 0);
		GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);
		
		byteBuffer = ByteBuffer.allocateDirect(sphere_elements.length * 2);
		byteBuffer.order(ByteOrder.nativeOrder());
		ShortBuffer elementsBuffer = byteBuffer.asShortBuffer();
		elementsBuffer.put(sphere_elements);
		elementsBuffer.position(0);
		
		GLES32.glBufferData(GLES32.GL_ELEMENT_ARRAY_BUFFER,
							sphere_elements.length * 2,
							elementsBuffer,
							GLES32.GL_STATIC_DRAW);
		
		GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, 0);

		GLES32.glBindVertexArray(0);

		// Depth Related Changes
		GLES32.glClearDepthf(1.0f);
		GLES32.glEnable(GLES32.GL_DEPTH_TEST);
		GLES32.glDepthFunc(GLES32.GL_LEQUAL);


		// Clearing the screen to black color
		GLES32.glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		// Lights Initialization
		lights[0] = new Light();

		lights[0].lightAmbient[0] = 0.0f;
		lights[0].lightAmbient[1] = 0.0f;
		lights[0].lightAmbient[2] = 0.0f;
		lights[0].lightAmbient[3] = 1.0f;

		lights[0].lightDiffuse[0] = 1.0f;
		lights[0].lightDiffuse[1] = 0.0f;
		lights[0].lightDiffuse[2] = 0.0f;
		lights[0].lightDiffuse[3] = 1.0f;

		lights[0].lightSpecular[0] = 1.0f;
		lights[0].lightSpecular[1] = 0.0f;
		lights[0].lightSpecular[2] = 0.0f;
		lights[0].lightSpecular[3] = 1.0f;

		lights[0].lightPosition[0] = 0.0f;
		lights[0].lightPosition[1] = 0.0f;
		lights[0].lightPosition[2] = 0.0f;
		lights[0].lightPosition[3] = 1.0f;

		lights[1] = new Light();

		lights[1].lightAmbient[0] = 0.0f;
		lights[1].lightAmbient[1] = 0.0f;
		lights[1].lightAmbient[2] = 0.0f;
		lights[1].lightAmbient[3] = 1.0f;

		lights[1].lightDiffuse[0] = 0.0f;
		lights[1].lightDiffuse[1] = 1.0f;
		lights[1].lightDiffuse[2] = 0.0f;
		lights[1].lightDiffuse[3] = 1.0f;

		lights[1].lightSpecular[0] = 0.0f;
		lights[1].lightSpecular[1] = 1.0f;
		lights[1].lightSpecular[2] = 0.0f;
		lights[1].lightSpecular[3] = 1.0f;

		lights[1].lightPosition[0] = 0.0f;
		lights[1].lightPosition[1] = 0.0f;
		lights[1].lightPosition[2] = 0.0f;
		lights[1].lightPosition[3] = 1.0f;

		lights[2] = new Light();

		lights[2].lightAmbient[0] = 0.0f;
		lights[2].lightAmbient[1] = 0.0f;
		lights[2].lightAmbient[2] = 0.0f;
		lights[2].lightAmbient[3] = 1.0f;

		lights[2].lightDiffuse[0] = 0.0f;
		lights[2].lightDiffuse[1] = 0.0f;
		lights[2].lightDiffuse[2] = 1.0f;
		lights[2].lightDiffuse[3] = 1.0f;

		lights[2].lightSpecular[0] = 0.0f;
		lights[2].lightSpecular[1] = 0.0f;
		lights[2].lightSpecular[2] = 1.0f;
		lights[2].lightSpecular[3] = 1.0f;

		lights[2].lightPosition[0] = 0.0f;
		lights[2].lightPosition[1] = 0.0f;
		lights[2].lightPosition[2] = 0.0f;
		lights[2].lightPosition[3] = 1.0f;
		// Initialization of Projection Matrix
		Matrix.setIdentityM(perspectiveProjectionMatrix_sphere, 0); // OpenGL purpose
		resize_sphere(FBO_WIDTH, FBO_HEIGHT);
	}

	private void resize(int width, int height)
	{
		if(height == 0)
			height = 1;
		GLES32.glViewport(0, 0, width, height);
		Matrix.perspectiveM(perspectiveProjectionMatrix, 0, 45.0f, (float)width / (float)height, 0.1f, 100.0f);
	}

	private void resize_sphere(int width, int height)
	{
		if(height == 0)
			height = 1;
		GLES32.glViewport(0, 0, width, height);
		Matrix.perspectiveM(perspectiveProjectionMatrix_sphere, 0, 45.0f, (float)width / (float)height, 0.1f, 100.0f);
	}

	private void display()
	{
		// Code
		if(fboResult == true)
		{
			display_sphere();
			update_sphere();
		}

		GLES32.glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		resize(gWidth, gHeight);

		GLES32.glClear(GLES32.GL_COLOR_BUFFER_BIT | GLES32.GL_DEPTH_BUFFER_BIT);
		GLES32.glUseProgram(shaderProgramObject);
		// Transformations Pyramid
		float translationMatrix[] = new float[16];
		Matrix.setIdentityM(translationMatrix, 0);
		Matrix.translateM(translationMatrix, 0, 0.0f, 0.0f, -4.5f);
		float rotationMatrix[] = new float[16];
		Matrix.setIdentityM(rotationMatrix, 0);
		Matrix.rotateM(rotationMatrix, 0, angleCube, 1.0f, 0.0f, 0.0f);
		Matrix.rotateM(rotationMatrix, 0, angleCube, 0.0f, 1.0f, 0.0f);
		Matrix.rotateM(rotationMatrix, 0, angleCube, 0.0f, 0.0f, 1.0f);
		float modelMatrix[] = new float[16];
		Matrix.setIdentityM(modelMatrix, 0);
		float modelViewMatrix[] = new float[16];
		Matrix.setIdentityM(modelViewMatrix, 0);
		float modelViewProjectionMatrix[] = new float[16];
		Matrix.setIdentityM(modelViewProjectionMatrix, 0);
		Matrix.multiplyMM(modelMatrix, 0, translationMatrix, 0, rotationMatrix, 0);
		Matrix.multiplyMM(modelViewMatrix, 0, modelViewMatrix, 0, modelMatrix, 0);
		Matrix.multiplyMM(modelViewProjectionMatrix , 0, perspectiveProjectionMatrix, 0, modelViewMatrix, 0);

		GLES32.glUniformMatrix4fv(mvpMatrixUniform, 1, false, modelViewProjectionMatrix, 0);
		
		GLES32.glBindVertexArray(vao_cube[0]);

		GLES32.glActiveTexture(GLES32.GL_TEXTURE0);
		GLES32.glBindTexture(GLES32.GL_TEXTURE_2D, fbo_texture[0]);
		GLES32.glUniform1i(textureSamplerUniform, 0);

		// Drawing Code
		GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN, 0, 4);
		GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN, 4, 4);
		GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN, 8, 4);
		GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN, 12, 4);
		GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN, 16, 4);
		GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN, 20, 4);

		GLES32.glBindVertexArray(0);

		// Unuse the shader program object
		GLES32.glUseProgram(0);
		requestRender(); // SwapBuffers()
	}

	private void update()
	{
		// Code
		angleCube = angleCube + 1.0f;
		if(angleCube >= 360.0f)
			angleCube -= 360.0f;
	}

	private void display_sphere()
	{
		// Code
		GLES32.glBindFramebuffer(GLES32.GL_FRAMEBUFFER, fbo[0]);
		GLES32.glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		resize_sphere(FBO_WIDTH, FBO_HEIGHT);

		GLES32.glClear(GLES32.GL_COLOR_BUFFER_BIT | GLES32.GL_DEPTH_BUFFER_BIT);

		// Transformations
		float translationMatrix[] = new float[16];
		Matrix.setIdentityM(translationMatrix, 0);
		Matrix.translateM(translationMatrix, 0, 0.0f, 0.0f, -2.0f);
		float modelMatrix[] = new float[16];
		Matrix.setIdentityM(modelMatrix, 0);
		float viewMatrix[] = new float[16];
		Matrix.setIdentityM(viewMatrix, 0);
		Matrix.multiplyMM(modelMatrix, 0, modelMatrix, 0, translationMatrix, 0);

		GLES32.glUseProgram(shaderProgramObject_sphere);
		GLES32.glUniformMatrix4fv(modelMatrixUniform_sphere, 1, false, modelMatrix, 0);
		GLES32.glUniformMatrix4fv(viewMatrixUniform_sphere, 1, false, viewMatrix, 0);
		GLES32.glUniformMatrix4fv(projectionMatrixUniform_sphere, 1, false, perspectiveProjectionMatrix, 0);
			
		if(doubleTapCount == 1)
		{
			GLES32.glUniform1i(lightingEnableUniform_sphere, 1);

			for(int i = 0; i < 3; i++)
			{
				GLES32.glUniform3fv(laUniform_sphere[i], 1, lights[i].lightAmbient, 0);
				GLES32.glUniform3fv(ldUniform_sphere[i], 1, lights[i].lightDiffuse, 0);
				GLES32.glUniform3fv(lsUniform_sphere[i], 1, lights[i].lightSpecular, 0);
				GLES32.glUniform4fv(lightPositionUniform_sphere[i], 1, lights[i].lightPosition, 0);
			}
			GLES32.glUniform3fv(kaUniform_sphere, 1, materialAmbient, 0);
			GLES32.glUniform3fv(kdUniform_sphere, 1, materialDiffuse, 0);
			GLES32.glUniform3fv(ksUniform_sphere, 1, materialSpecular, 0);
			GLES32.glUniform1f(materialShininessUniform_sphere, materialShininess);

		}
		else
		{
			GLES32.glUniform1i(lightingEnableUniform_sphere, 0);
		}
		// bind vao
		GLES32.glBindVertexArray(vao_sphere[0]);
		
		// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
		GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);
		GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements, GLES32.GL_UNSIGNED_SHORT, 0);
		
		// unbind vao
		GLES32.glBindVertexArray(0);
		// Unuse the shader program object
		GLES32.glUseProgram(0);

		GLES32.glBindFramebuffer(GLES32.GL_FRAMEBUFFER, 0);
		requestRender(); // SwapBuffers()
	}

	private void update_sphere()
	{
		// Code
		redLightAngle += 1.0f;
		if(redLightAngle >= 360.0f)
			redLightAngle -= 360.0f;
		lights[0].lightPosition[0] = 0.0f;
		lights[0].lightPosition[1] = 10.0f * (float) Math.cos(redLightAngle * Math.PI / 180.0f);
		lights[0].lightPosition[2] = 10.0f * (float) Math.sin(redLightAngle * Math.PI / 180.0f);
		lights[0].lightPosition[3] = 1.0f;

		greenLightAngle += 1.0f;
		if(greenLightAngle >= 360.0f)
			greenLightAngle -= 360.0f;
		lights[1].lightPosition[0] = 10.0f * (float) Math.cos(greenLightAngle * Math.PI / 180.0f);
		lights[1].lightPosition[1] = 0.0f;
		lights[1].lightPosition[2] = 10.0f * (float) Math.sin(greenLightAngle * Math.PI / 180.0f);
		lights[1].lightPosition[3] = 1.0f;

		blueLightAngle += 1.0f;
		if(blueLightAngle >= 360.0f)
			blueLightAngle -= 360.0f;
		lights[2].lightPosition[0] = 10.0f * (float) Math.cos(blueLightAngle * Math.PI / 180.0f);
		lights[2].lightPosition[1] = 10.0f * (float) Math.sin(blueLightAngle * Math.PI / 180.0f);
		lights[2].lightPosition[2] = 0.0f;
		lights[2].lightPosition[3] = 1.0f;
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
		if(vbo_cube_texcoord[0] > 0)
		{
			GLES32.glDeleteBuffers(1, vbo_cube_texcoord, 0);
			vbo_cube_texcoord[0] = 0;
		}
		if(vbo_cube_position[0] > 0)
		{
			GLES32.glDeleteBuffers(1, vbo_cube_position, 0);
			vbo_cube_position[0] = 0;
		}
		if(vao_cube[0] > 0)
		{
			GLES32.glDeleteVertexArrays(1, vao_cube, 0);
			vao_cube[0] = 0;
		}
	}

	private void uninitialize_sphere()
	{
		// Code
		if(shaderProgramObject_sphere > 0)
		{
			GLES32.glUseProgram(shaderProgramObject_sphere);
			int retVal[] = new int[1];
			GLES32.glGetProgramiv(shaderProgramObject_sphere, GLES32.GL_ATTACHED_SHADERS, retVal, 0);
			if(retVal[0] > 0)
			{
				int numAttachedShaders = retVal[0];
				int shaderObjects[] = new int[numAttachedShaders];
				GLES32.glGetAttachedShaders(shaderProgramObject_sphere, numAttachedShaders, retVal, 0, shaderObjects, 0);
				for(int i = 0; i < numAttachedShaders; i++)
				{
					GLES32.glDetachShader(shaderProgramObject_sphere, shaderObjects[i]);
					GLES32.glDeleteShader(shaderObjects[i]);
					shaderObjects[i] = 0;
				}
			}
			GLES32.glUseProgram(0);
			GLES32.glDeleteProgram(shaderProgramObject_sphere);
			shaderProgramObject_sphere = 0;	
		}
		
	   // destroy vao
	   if(vao_sphere[0] != 0)
	   {
		   GLES32.glDeleteVertexArrays(1, vao_sphere, 0);
		   vao_sphere[0]=0;
	   }
	   
	   // destroy position vbo
	   if(vbo_sphere_position[0] != 0)
	   {
		   GLES32.glDeleteBuffers(1, vbo_sphere_position, 0);
		   vbo_sphere_position[0]=0;
	   }
	   
	   // destroy normal vbo
	   if(vbo_sphere_normal[0] != 0)
	   {
			GLES32.glDeleteBuffers(1, vbo_sphere_normal, 0);
			vbo_sphere_normal[0]=0;
		}
	
	   // destroy element vbo
	   if(vbo_sphere_element[0] != 0)
	   {
			GLES32.glDeleteBuffers(1, vbo_sphere_element, 0);
			vbo_sphere_element[0]=0;
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

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
	private GestureDetector gestureDetector;
	private Context context;
	private int shaderProgramObjectV;
	private int shaderProgramObjectF;
	// Vertex
	private int modelMatrixUniformV = 0;
	private int viewMatrixUniformV = 0;
	private int projectionMatrixUniformV = 0;
	private int ldUniformV[] = new int[3];
	private int laUniformV[] = new int[3];
	private int lsUniformV[] = new int[3];
	private int lightPositionUniformV[] = new int[3];

	private int kaUniformV = 0;
	private int kdUniformV = 0;
	private int ksUniformV = 0;
	private int materialShininessUniformV = 0;

	private int lightingEnableUniformV = 0;

	// Fragment
	private int modelMatrixUniformF = 0;
	private int viewMatrixUniformF = 0;
	private int projectionMatrixUniformF = 0;
	private int ldUniformF[] = new int[3];
	private int laUniformF[] = new int[3];
	private int lsUniformF[] = new int[3];
	private int lightPositionUniformF[] = new int[3];

	private int kaUniformF = 0;
	private int kdUniformF = 0;
	private int ksUniformF = 0;
	private int materialShininessUniformF = 0;

	private int lightingEnableUniformF = 0;
	
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
	private int singleTapCount = 0;

	private int[] vao_sphere = new int[1];
	private int[] vbo_sphere_position = new int[1];
	private int[] vbo_sphere_normal = new int[1];
	private int[] vbo_sphere_element = new int[1];
	private int numVertices = 0;
	private int numElements = 0;

	private float redLightAngle = 0.0f;
	private float greenLightAngle = 0.0f;
	private float blueLightAngle = 0.0f;

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
		// Vertex Shader Per Vertex
		final String vertexShaderSourceCodeV = String.format(
			"#version 320 es" +
			"\n" +
			"precision mediump int;" +
			"precision highp float;" +
			"in vec4 a_position;\n" +
			"in vec3 a_normal;\n" +
			"uniform mat4 u_modelMatrix;\n" +
			"uniform mat4 u_viewMatrix;\n" +
			"uniform mat4 u_projectionMatrix;\n" +
			"uniform vec3 u_la[3];\n" +
			"uniform vec3 u_ld[3];" +
			"uniform vec3 u_ls[3];" +
			"uniform vec4 u_lightPosition[3];" +
			"uniform vec3 u_ka;" +
			"uniform vec3 u_kd;" +
			"uniform vec3 u_ks;" +
			"uniform float u_materialShininess;" +
			"uniform int u_lightingEnabled;" +
			"out vec3 fong_ads_light;" +
			"void main(void)" +
			"{" +
			"if(u_lightingEnabled == 1)" +
			"{" +
			"vec4 eyeCoordinates = u_viewMatrix * u_modelMatrix * a_position;\n" +
			"mat3 normalMatrix = mat3(u_viewMatrix * u_modelMatrix);\n" +
			"vec3 transformedNormals = normalize(normalMatrix * a_normal);\n" +
			"vec3 viewerVector = normalize(-eyeCoordinates.xyz);\n" +
			"vec3 ambient[3];\n" +
			"vec3 lightDirection[3];\n" +
			"vec3 diffuse[3];\n" +
			"vec3 specular[3];\n" +
			"vec3 reflectionVector[3];" +
			"for(int i=0;i<3;i++)\n" +
			"{\n" +
			"ambient[i] = u_la[i] * u_ka;\n" +
			"lightDirection[i] = normalize(vec3(u_lightPosition[i]) - eyeCoordinates.xyz);\n" +
			"diffuse[i] = u_ld[i] * u_kd * max(dot(lightDirection[i], transformedNormals), 0.0);\n" +
			"reflectionVector[i] = reflect(-lightDirection[i], transformedNormals);\n" +
			"specular[i] = u_ls[i] * u_ks * pow(max(dot(reflectionVector[i], viewerVector), 0.0), u_materialShininess);\n" +
			"fong_ads_light = fong_ads_light + ambient[i] + diffuse[i] + specular[i];\n" +
			"}\n" +
			"}\n" +
			"else\n" +
			"{\n" +
			"fong_ads_light = vec3(1.0, 1.0, 1.0);\n" +
			"}\n" +
			"gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * a_position;\n" +
			"}"
		);
		int vertexShaderObjectV = GLES32.glCreateShader(GLES32.GL_VERTEX_SHADER);
		GLES32.glShaderSource(vertexShaderObjectV, vertexShaderSourceCodeV);
		GLES32.glCompileShader(vertexShaderObjectV);

		int status[] = new int[1];
		int infoLogLength[] = new int[1];
		String log = null;
		GLES32.glGetShaderiv(vertexShaderObjectV, GLES32.GL_COMPILE_STATUS, status, 0);
		if(status[0] == GLES32.GL_FALSE)
		{
			GLES32.glGetShaderiv(vertexShaderObjectV, GLES32.GL_INFO_LOG_LENGTH, infoLogLength, 0);
			if(infoLogLength[0] > 0)
			{
				log = GLES32.glGetShaderInfoLog(vertexShaderObjectV);
				System.out.println("SAB: Vertex Shader Compilation log : "+log);
				uninitialize();
				System.exit(0);
			}
		}

		// Fragment Shader
		final String fragmentShaderSourceCodeV = String.format(
			"#version 320 es" +
			"\n" +
			"precision mediump int;" +
			"precision highp float;" +
			"in vec3 fong_ads_light;" +
			"out vec4 FragColor;" +
			"void main(void)" +
			"{" +
			"FragColor = vec4(fong_ads_light,1.0);" +
			"}"
		);
		int fragmentShaderObjectV = GLES32.glCreateShader(GLES32.GL_FRAGMENT_SHADER);
		GLES32.glShaderSource(fragmentShaderObjectV, fragmentShaderSourceCodeV);
		GLES32.glCompileShader(fragmentShaderObjectV);

		status[0] = 0;
		infoLogLength[0] = 0;
		log = null;
		GLES32.glGetShaderiv(fragmentShaderObjectV, GLES32.GL_COMPILE_STATUS, status, 0);
		if(status[0] == GLES32.GL_FALSE)
		{
			GLES32.glGetShaderiv(fragmentShaderObjectV, GLES32.GL_INFO_LOG_LENGTH, infoLogLength, 0);
			if(infoLogLength[0] > 0)
			{
				log = GLES32.glGetShaderInfoLog(fragmentShaderObjectV);
				System.out.println("SAB: Fragment Shader Compilation log : "+log);
				uninitialize();
				System.exit(0);
			}
		}
		
		// Shader Program
		shaderProgramObjectV = GLES32.glCreateProgram();
		GLES32.glAttachShader(shaderProgramObjectV, vertexShaderObjectV);
		GLES32.glAttachShader(shaderProgramObjectV, fragmentShaderObjectV);
		GLES32.glBindAttribLocation(shaderProgramObjectV, MyGLESMacros.SAB_ATTRIBUTE_POSITION, "a_position");
		GLES32.glBindAttribLocation(shaderProgramObjectV, MyGLESMacros.SAB_ATTRIBUTE_NORMAL, "a_normal");
		GLES32.glLinkProgram(shaderProgramObjectV);
		
		// Error Checking
		status[0] = 0;
		infoLogLength[0] = 0;
		log = null;
		GLES32.glGetProgramiv(shaderProgramObjectV, GLES32.GL_LINK_STATUS, status, 0);
		if(status[0] == GLES32.GL_FALSE)
		{
			GLES32.glGetProgramiv(shaderProgramObjectV, GLES32.GL_INFO_LOG_LENGTH, infoLogLength, 0);
			if(infoLogLength[0] > 0)
			{
				log = GLES32.glGetProgramInfoLog(shaderProgramObjectV);
				System.out.println("SAB: Shader Program Link log : "+log);
				uninitialize();
				System.exit(0);
			}
		}
		// Post linking uniforms Vertex
		modelMatrixUniformV = GLES32.glGetUniformLocation(shaderProgramObjectV, "u_modelMatrix");
		viewMatrixUniformV = GLES32.glGetUniformLocation(shaderProgramObjectV, "u_viewMatrix");
		projectionMatrixUniformV = GLES32.glGetUniformLocation(shaderProgramObjectV, "u_projectionMatrix");
	
		laUniformV[0] = GLES32.glGetUniformLocation(shaderProgramObjectV, "u_la[0]");
		ldUniformV[0] = GLES32.glGetUniformLocation(shaderProgramObjectV, "u_ld[0]");
		lsUniformV[0] = GLES32.glGetUniformLocation(shaderProgramObjectV, "u_ls[0]");
		lightPositionUniformV[0] = GLES32.glGetUniformLocation(shaderProgramObjectV, "u_lightPosition[0]");

		laUniformV[1] = GLES32.glGetUniformLocation(shaderProgramObjectV, "u_la[1]");
		ldUniformV[1] = GLES32.glGetUniformLocation(shaderProgramObjectV, "u_ld[1]");
		lsUniformV[1] = GLES32.glGetUniformLocation(shaderProgramObjectV, "u_ls[1]");
		lightPositionUniformV[1] = GLES32.glGetUniformLocation(shaderProgramObjectV, "u_lightPosition[1]");

		laUniformV[2] = GLES32.glGetUniformLocation(shaderProgramObjectV, "u_la[2]");
		ldUniformV[2] = GLES32.glGetUniformLocation(shaderProgramObjectV, "u_ld[2]");
		lsUniformV[2] = GLES32.glGetUniformLocation(shaderProgramObjectV, "u_ls[2]");
		lightPositionUniformV[2] = GLES32.glGetUniformLocation(shaderProgramObjectV, "u_lightPosition[2]");

		kaUniformV = GLES32.glGetUniformLocation(shaderProgramObjectV, "u_ka");
		kdUniformV = GLES32.glGetUniformLocation(shaderProgramObjectV, "u_kd");
		ksUniformV = GLES32.glGetUniformLocation(shaderProgramObjectV, "u_ks");
		materialShininessUniformV = GLES32.glGetUniformLocation(shaderProgramObjectV, "u_materialShininess");

		lightingEnableUniformV = GLES32.glGetUniformLocation(shaderProgramObjectV, "u_lightingEnabled");

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

		status[0] = 0;
		infoLogLength[0] = 0;
		log = null;
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
		shaderProgramObjectF = GLES32.glCreateProgram();
		GLES32.glAttachShader(shaderProgramObjectF, vertexShaderObjectF);
		GLES32.glAttachShader(shaderProgramObjectF, fragmentShaderObjectF);
		GLES32.glBindAttribLocation(shaderProgramObjectF, MyGLESMacros.SAB_ATTRIBUTE_POSITION, "a_position");
		GLES32.glBindAttribLocation(shaderProgramObjectF, MyGLESMacros.SAB_ATTRIBUTE_NORMAL, "a_normal");
		GLES32.glLinkProgram(shaderProgramObjectF);
		
		// Error Checking
		status[0] = 0;
		infoLogLength[0] = 0;
		log = null;
		GLES32.glGetProgramiv(shaderProgramObjectF, GLES32.GL_LINK_STATUS, status, 0);
		if(status[0] == GLES32.GL_FALSE)
		{
			GLES32.glGetProgramiv(shaderProgramObjectF, GLES32.GL_INFO_LOG_LENGTH, infoLogLength, 0);
			if(infoLogLength[0] > 0)
			{
				log = GLES32.glGetProgramInfoLog(shaderProgramObjectF);
				System.out.println("SAB: Shader Program Link log : "+log);
				uninitialize();
				System.exit(0);
			}
		}
		// Post linking Uniforms Fragment
		modelMatrixUniformF = GLES32.glGetUniformLocation(shaderProgramObjectF, "u_modelMatrix");
		viewMatrixUniformF = GLES32.glGetUniformLocation(shaderProgramObjectF, "u_viewMatrix");
		projectionMatrixUniformF = GLES32.glGetUniformLocation(shaderProgramObjectF, "u_projectionMatrix");
	
		laUniformF[0] = GLES32.glGetUniformLocation(shaderProgramObjectF, "u_la[0]");
		ldUniformF[0] = GLES32.glGetUniformLocation(shaderProgramObjectF, "u_ld[0]");
		lsUniformF[0] = GLES32.glGetUniformLocation(shaderProgramObjectF, "u_ls[0]");
		lightPositionUniformF[0] = GLES32.glGetUniformLocation(shaderProgramObjectF, "u_lightPosition[0]");

		laUniformF[1] = GLES32.glGetUniformLocation(shaderProgramObjectF, "u_la[1]");
		ldUniformF[1] = GLES32.glGetUniformLocation(shaderProgramObjectF, "u_ld[1]");
		lsUniformF[1] = GLES32.glGetUniformLocation(shaderProgramObjectF, "u_ls[1]");
		lightPositionUniformF[1] = GLES32.glGetUniformLocation(shaderProgramObjectF, "u_lightPosition[1]");

		laUniformF[2] = GLES32.glGetUniformLocation(shaderProgramObjectF, "u_la[2]");
		ldUniformF[2] = GLES32.glGetUniformLocation(shaderProgramObjectF, "u_ld[2]");
		lsUniformF[2] = GLES32.glGetUniformLocation(shaderProgramObjectF, "u_ls[2]");
		lightPositionUniformF[2] = GLES32.glGetUniformLocation(shaderProgramObjectF, "u_lightPosition[2]");

		kaUniformF = GLES32.glGetUniformLocation(shaderProgramObjectF, "u_ka");
		kdUniformF = GLES32.glGetUniformLocation(shaderProgramObjectF, "u_kd");
		ksUniformF = GLES32.glGetUniformLocation(shaderProgramObjectF, "u_ks");
		materialShininessUniformF = GLES32.glGetUniformLocation(shaderProgramObjectF, "u_materialShininess");

		lightingEnableUniformF = GLES32.glGetUniformLocation(shaderProgramObjectF, "u_lightingEnabled");

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
		// Enable Culling
		GLES32.glEnable(GLES32.GL_CULL_FACE);
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

		// Transformations
		float translationMatrix[] = new float[16];
		Matrix.setIdentityM(translationMatrix, 0);
		Matrix.translateM(translationMatrix, 0, 0.0f, 0.0f, -2.0f);
		float modelMatrix[] = new float[16];
		Matrix.setIdentityM(modelMatrix, 0);
		float viewMatrix[] = new float[16];
		Matrix.setIdentityM(viewMatrix, 0);
		Matrix.multiplyMM(modelMatrix, 0, modelMatrix, 0, translationMatrix, 0);
		if(singleTapCount == 0)
		{
			GLES32.glUseProgram(shaderProgramObjectV);
			GLES32.glUniformMatrix4fv(modelMatrixUniformV, 1, false, modelMatrix, 0);
			GLES32.glUniformMatrix4fv(viewMatrixUniformV, 1, false, viewMatrix, 0);
			GLES32.glUniformMatrix4fv(projectionMatrixUniformV, 1, false, perspectiveProjectionMatrix, 0);
			
			if(doubleTapCount == 1)
			{
				GLES32.glUniform1i(lightingEnableUniformV, 1);

				for(int i = 0; i < 3; i++)
				{
					GLES32.glUniform3fv(laUniformV[i], 1, lights[i].lightAmbient, 0);
					GLES32.glUniform3fv(ldUniformV[i], 1, lights[i].lightDiffuse, 0);
					GLES32.glUniform3fv(lsUniformV[i], 1, lights[i].lightSpecular, 0);
					GLES32.glUniform4fv(lightPositionUniformV[i], 1, lights[i].lightPosition, 0);
				}

				GLES32.glUniform3fv(kaUniformV, 1, materialAmbient, 0);
				GLES32.glUniform3fv(kdUniformV, 1, materialDiffuse, 0);
				GLES32.glUniform3fv(ksUniformV, 1, materialSpecular, 0);
				GLES32.glUniform1f(materialShininessUniformV, materialShininess);

			}
			else
			{
				GLES32.glUniform1i(lightingEnableUniformV, 0);
			}
		}
		else
		{
			GLES32.glUseProgram(shaderProgramObjectF);
			GLES32.glUniformMatrix4fv(modelMatrixUniformF, 1, false, modelMatrix, 0);
			GLES32.glUniformMatrix4fv(viewMatrixUniformF, 1, false, viewMatrix, 0);
			GLES32.glUniformMatrix4fv(projectionMatrixUniformF, 1, false, perspectiveProjectionMatrix, 0);
			
			if(doubleTapCount == 1)
			{
				GLES32.glUniform1i(lightingEnableUniformF, 1);

				for(int i = 0; i < 3; i++)
				{
					GLES32.glUniform3fv(laUniformF[i], 1, lights[i].lightAmbient, 0);
					GLES32.glUniform3fv(ldUniformF[i], 1, lights[i].lightDiffuse, 0);
					GLES32.glUniform3fv(lsUniformF[i], 1, lights[i].lightSpecular, 0);
					GLES32.glUniform4fv(lightPositionUniformF[i], 1, lights[i].lightPosition, 0);
				}
				GLES32.glUniform3fv(kaUniformF, 1, materialAmbient, 0);
				GLES32.glUniform3fv(kdUniformF, 1, materialDiffuse, 0);
				GLES32.glUniform3fv(ksUniformF, 1, materialSpecular, 0);
				GLES32.glUniform1f(materialShininessUniformF, materialShininess);

			}
			else
			{
				GLES32.glUniform1i(lightingEnableUniformF, 0);
			}
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
		requestRender(); // SwapBuffers()
	}

	private void update()
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
		if(shaderProgramObjectF > 0)
		{
			GLES32.glUseProgram(shaderProgramObjectF);
			int retVal[] = new int[1];
			GLES32.glGetProgramiv(shaderProgramObjectF, GLES32.GL_ATTACHED_SHADERS, retVal, 0);
			if(retVal[0] > 0)
			{
				int numAttachedShaders = retVal[0];
				int shaderObjects[] = new int[numAttachedShaders];
				GLES32.glGetAttachedShaders(shaderProgramObjectF, numAttachedShaders, retVal, 0, shaderObjects, 0);
				for(int i = 0; i < numAttachedShaders; i++)
				{
					GLES32.glDetachShader(shaderProgramObjectF, shaderObjects[i]);
					GLES32.glDeleteShader(shaderObjects[i]);
					shaderObjects[i] = 0;
				}
			}
			GLES32.glUseProgram(0);
			GLES32.glDeleteProgram(shaderProgramObjectF);
			shaderProgramObjectF = 0;	
		}
		if(shaderProgramObjectV > 0)
		{
			GLES32.glUseProgram(shaderProgramObjectV);
			int retVal[] = new int[1];
			GLES32.glGetProgramiv(shaderProgramObjectV, GLES32.GL_ATTACHED_SHADERS, retVal, 0);
			if(retVal[0] > 0)
			{
				int numAttachedShaders = retVal[0];
				int shaderObjects[] = new int[numAttachedShaders];
				GLES32.glGetAttachedShaders(shaderProgramObjectV, numAttachedShaders, retVal, 0, shaderObjects, 0);
				for(int i = 0; i < numAttachedShaders; i++)
				{
					GLES32.glDetachShader(shaderProgramObjectV, shaderObjects[i]);
					GLES32.glDeleteShader(shaderObjects[i]);
					shaderObjects[i] = 0;
				}
			}
			GLES32.glUseProgram(0);
			GLES32.glDeleteProgram(shaderProgramObjectV);
			shaderProgramObjectV = 0;	
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
		singleTapCount++;
		if(singleTapCount > 1)
			singleTapCount = 0;
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

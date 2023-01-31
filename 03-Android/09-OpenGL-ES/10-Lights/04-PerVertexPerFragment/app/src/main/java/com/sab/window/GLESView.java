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
	private int ldUniformV = 0;
	private int laUniformV = 0;
	private int lsUniformV = 0;
	private int lightPositionUniformV = 0;

	private int kaUniformV = 0;
	private int kdUniformV = 0;
	private int ksUniformV = 0;
	private int materialShininessUniformV = 0;

	private int lightingEnableUniformV = 0;

	// Fragment
	private int modelMatrixUniformF = 0;
	private int viewMatrixUniformF = 0;
	private int projectionMatrixUniformF = 0;
	private int ldUniformF = 0;
	private int laUniformF = 0;
	private int lsUniformF = 0;
	private int lightPositionUniformF = 0;

	private int kaUniformF = 0;
	private int kdUniformF = 0;
	private int ksUniformF = 0;
	private int materialShininessUniformF = 0;

	private int lightingEnableUniformF = 0;

	private float[] lightAmbient = new float[] {0.1f, 0.1f, 0.1f, 1.0f};
	private float[] lightDiffuse = new float[] {1.0f, 1.0f, 1.0f, 1.0f};
	private float[] lightSpecular = new float[] {1.0f, 1.0f, 1.0f, 1.0f};
	private float[] lightPosition = new float[] {100.0f, 100.0f, 100.0f, 1.0f};
	
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
			"uniform vec3 u_la;\n" +
			"uniform vec3 u_ld;" +
			"uniform vec3 u_ls;" +
			"uniform vec4 u_lightPosition;" +
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
			"vec3 ambient = u_la * u_ka;" +
			"vec4 eyeCoordinates = u_viewMatrix * u_modelMatrix * a_position;" +
			"mat3 normalMatrix = mat3(u_viewMatrix * u_modelMatrix);" +
			"vec3 transformedNormals = normalize(normalMatrix * a_normal);" +
			"vec3 lightDirection = normalize(vec3(u_lightPosition) - eyeCoordinates.xyz);" +
			"vec3 diffuse = u_ld * u_kd * max(dot(lightDirection, transformedNormals), 0.0);" +
			"vec3 reflectionVector = reflect(-lightDirection, transformedNormals);" +
			"vec3 viewerVector = normalize(-eyeCoordinates.xyz);" +
			"vec3 specular = u_ls * u_ks * pow(max(dot(reflectionVector, viewerVector), 0.0), u_materialShininess);" +
			"fong_ads_light = ambient + diffuse + specular;" +
			"}" +
			"else" +
			"{" +
			"fong_ads_light = vec3(1.0, 1.0, 1.0);" +
			"}" +
			"gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * a_position;" +
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
			"FragColor = vec4(fong_ads_light, 1.0);" +
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
	
		laUniformV = GLES32.glGetUniformLocation(shaderProgramObjectV, "u_la");
		ldUniformV = GLES32.glGetUniformLocation(shaderProgramObjectV, "u_ld");
		lsUniformV = GLES32.glGetUniformLocation(shaderProgramObjectV, "u_ls");
		lightPositionUniformV = GLES32.glGetUniformLocation(shaderProgramObjectV, "u_lightPosition");

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
			"uniform vec4 u_lightPosition;\n" +
			"uniform int u_lightingEnabled;\n" +
			"out vec3 transformedNormals;\n" +
			"out vec3 lightDirection;\n" +
			"out vec3 viewerVector;\n" +
			"void main(void)\n" +
			"{\n" +
			"if(u_lightingEnabled == 1)\n" +
			"{\n" +
			"vec4 eyeCoordinates = u_viewMatrix * u_modelMatrix * a_position;\n" +
			"mat3 normalMatrix = mat3(u_viewMatrix * u_modelMatrix);\n" +
			"transformedNormals = normalMatrix * a_normal;\n" +
			"lightDirection = vec3(u_lightPosition) - eyeCoordinates.xyz;\n" +
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
			"in vec3 lightDirection;\n" +
			"in vec3 viewerVector;\n" +
			"uniform vec3 u_la;\n" +
			"uniform vec3 u_ld;\n" +
			"uniform vec3 u_ls;\n" +
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
			"vec3 ambient = u_la * u_ka;\n" +
			"vec3 normalized_transformedNormals = normalize(transformedNormals);\n" +
			"vec3 normalized_lightDirection = normalize(lightDirection);\n" +
			"vec3 diffuse = u_ld * u_kd * max(dot(normalized_lightDirection, normalized_transformedNormals), 0.0);\n" +
			"vec3 reflectionVector = reflect(-normalized_lightDirection, normalized_transformedNormals);\n" +
			"vec3 normalized_viewerVector = normalize(viewerVector);\n" +
			"vec3 specular = u_ls * u_ks * pow(max(dot(reflectionVector, normalized_viewerVector), 0.0), u_materialShininess);\n" +
			"fong_ads_light = vec4(ambient + diffuse + specular, 1.0);\n" +
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
	
		laUniformF = GLES32.glGetUniformLocation(shaderProgramObjectF, "u_la");
		ldUniformF = GLES32.glGetUniformLocation(shaderProgramObjectF, "u_ld");
		lsUniformF = GLES32.glGetUniformLocation(shaderProgramObjectF, "u_ls");
		lightPositionUniformF = GLES32.glGetUniformLocation(shaderProgramObjectF, "u_lightPosition");

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

				GLES32.glUniform3fv(laUniformV, 1, lightAmbient, 0);
				GLES32.glUniform3fv(ldUniformV, 1, lightDiffuse, 0);
				GLES32.glUniform3fv(lsUniformV, 1, lightSpecular, 0);
				GLES32.glUniform4fv(lightPositionUniformV, 1, lightPosition, 0);

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

				GLES32.glUniform3fv(laUniformF, 1, lightAmbient, 0);
				GLES32.glUniform3fv(ldUniformF, 1, lightDiffuse, 0);
				GLES32.glUniform3fv(lsUniformF, 1, lightSpecular, 0);
				GLES32.glUniform4fv(lightPositionUniformF, 1, lightPosition, 0);

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

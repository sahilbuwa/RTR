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
	private int shaderProgramObject;
	private int modelMatrixUniform = 0;
	private int viewMatrixUniform = 0;
	private int projectionMatrixUniform = 0;
	private int ldUniform = 0;
	private int laUniform = 0;
	private int lsUniform = 0;
	private int lightPositionUniform = 0;

	private int kaUniform = 0;
	private int kdUniform = 0;
	private int ksUniform = 0;
	private int materialShininessUniform = 0;

	private int lightingEnableUniform = 0;

	private float[] lightAmbient = new float[] {0.0f, 0.0f, 0.0f, 1.0f};
	private float[] lightDiffuse = new float[] {1.0f, 1.0f, 1.0f, 1.0f};
	private float[] lightSpecular = new float[] {1.0f, 1.0f, 1.0f, 1.0f};
	private float[] lightPosition = new float[] {0.0f, 0.0f, 0.0f, 1.0f};
	
	private float[] materialAmbient = new float[4];
	private float[] materialDiffuse = new float[4];
	private float[] materialSpecular = new float[4];
	private float materialShininess;

	
	private int doubleTapCount = 0;
	private int singleTapCount = 0;

	private int[] vao_sphere = new int[1];
	private int[] vbo_sphere_position = new int[1];
	private int[] vbo_sphere_normal = new int[1];
	private int[] vbo_sphere_element = new int[1];
	private int numVertices = 0;
	private int numElements = 0;

	private float perspectiveProjectionMatrix[] = new float[16];
	private float angleForXRotation = 0.0f;
	private float angleForYRotation = 0.0f;
	private float angleForZRotation = 0.0f;

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
	
		laUniform = GLES32.glGetUniformLocation(shaderProgramObject, "u_la");
		ldUniform = GLES32.glGetUniformLocation(shaderProgramObject, "u_ld");
		lsUniform = GLES32.glGetUniformLocation(shaderProgramObject, "u_ls");
		lightPositionUniform = GLES32.glGetUniformLocation(shaderProgramObject, "u_lightPosition");

		kaUniform = GLES32.glGetUniformLocation(shaderProgramObject, "u_ka");
		kdUniform = GLES32.glGetUniformLocation(shaderProgramObject, "u_kd");
		ksUniform = GLES32.glGetUniformLocation(shaderProgramObject, "u_ks");
		materialShininessUniform = GLES32.glGetUniformLocation(shaderProgramObject, "u_materialShininess");

		lightingEnableUniform = GLES32.glGetUniformLocation(shaderProgramObject, "u_lightingEnabled");

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
		// Clearing the screen to grau color
		GLES32.glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

		// Initialization of Projection Matrix
		Matrix.setIdentityM(perspectiveProjectionMatrix, 0); // OpenGL purpose
	}

	private void resize(int width, int height)
	{
		if(height == 0)
			height = 1;
		GLES32.glViewport(0, 0, width, height);
		Matrix.perspectiveM(perspectiveProjectionMatrix, 0, 45.0f, (float)width / (float)height, 0.1f, 1000.0f);
	}

	private void display()
	{
		GLES32.glClear(GLES32.GL_COLOR_BUFFER_BIT | GLES32.GL_DEPTH_BUFFER_BIT);
		GLES32.glUseProgram(shaderProgramObject);

		// Transformations
		float translationMatrix[] = new float[16];
		Matrix.setIdentityM(translationMatrix, 0);
		Matrix.translateM(translationMatrix, 0, 0.0f, 0.0f, -2.0f);
		float modelMatrix[] = new float[16];
		Matrix.setIdentityM(modelMatrix, 0);
		float viewMatrix[] = new float[16];
		Matrix.setIdentityM(viewMatrix, 0);
		Matrix.translateM(viewMatrix, 0, 0.0f, 0.0f, -20.0f);
		Matrix.multiplyMM(modelMatrix, 0, modelMatrix, 0, translationMatrix, 0);

		GLES32.glUniformMatrix4fv(viewMatrixUniform, 1, false, viewMatrix, 0);
		GLES32.glUniformMatrix4fv(projectionMatrixUniform, 1, false, perspectiveProjectionMatrix, 0);

		if(doubleTapCount == 1)
		{
			GLES32.glUniform1i(lightingEnableUniform, 1);

			GLES32.glUniform3fv(laUniform, 1, lightAmbient, 0);
			GLES32.glUniform3fv(ldUniform, 1, lightDiffuse, 0);
			GLES32.glUniform3fv(lsUniform, 1, lightSpecular, 0);
			GLES32.glUniform4fv(lightPositionUniform, 1, lightPosition, 0);
		}
		else
		{
			GLES32.glUniform1i(lightingEnableUniform, 0);
		}
		
		// ***** 1st sphere on 1st column, emerald *****
		// ambient material
		materialAmbient[0] = 0.0215f; // r
		materialAmbient[1] = 0.1745f; // g
		materialAmbient[2] = 0.0215f; // b
		materialAmbient[3] = 1.0f;   // a

		// diffuse material
		materialDiffuse[0] = 0.07568f; // r
		materialDiffuse[1] = 0.61424f; // g
		materialDiffuse[2] = 0.07568f; // b
		materialDiffuse[3] = 1.0f;    // a
		
		// specular material
		materialSpecular[0] = 0.633f;    // r
		materialSpecular[1] = 0.727811f; // g
		materialSpecular[2] = 0.633f;    // b
		materialSpecular[3] = 1.0f;     // a
		
		// materialShininess
		materialShininess = 0.6f * 128.0f;
		GLES32.glUniform3fv(kaUniform, 1, materialAmbient, 0);
		GLES32.glUniform3fv(kdUniform, 1, materialDiffuse, 0);
		GLES32.glUniform3fv(ksUniform, 1, materialSpecular, 0);
		GLES32.glUniform1f(materialShininessUniform, materialShininess);

		// geometry
		Matrix.setIdentityM(translationMatrix, 0);
		Matrix.translateM(translationMatrix, 0, 4.0f, 3.15f, 10.0f);
		Matrix.setIdentityM(modelMatrix, 0);
		Matrix.multiplyMM(modelMatrix, 0, modelMatrix, 0, translationMatrix, 0);
		GLES32.glUniformMatrix4fv(modelMatrixUniform, 1, false, modelMatrix, 0);
		
		// bind vao
		GLES32.glBindVertexArray(vao_sphere[0]);
		
		// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
		GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);
		GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements, GLES32.GL_UNSIGNED_SHORT, 0);
		
		// unbind vao
		GLES32.glBindVertexArray(0);
		
		// // *******************************************************

		// ***** 2nd sphere on 1st column, jade *****
		// ambient material
		materialAmbient[0] = 0.135f;  // r
		materialAmbient[1] = 0.2225f; // g
		materialAmbient[2] = 0.1575f; // b
		materialAmbient[3] = 1.0f;   // a
		

		// diffuse material
		materialDiffuse[0] = 0.54f; // r
		materialDiffuse[1] = 0.89f; // g
		materialDiffuse[2] = 0.63f; // b
		materialDiffuse[3] = 1.0f; // a
		

		// specular material
		materialSpecular[0] = 0.316228f; // r
		materialSpecular[1] = 0.316228f; // g
		materialSpecular[2] = 0.316228f; // b
		materialSpecular[3] = 1.0f;     // a
		

		// materialShininess
		materialShininess = 0.1f * 128.0f;
		GLES32.glUniform3fv(kaUniform, 1, materialAmbient, 0);
		GLES32.glUniform3fv(kdUniform, 1, materialDiffuse, 0);
		GLES32.glUniform3fv(ksUniform, 1, materialSpecular, 0);
		GLES32.glUniform1f(materialShininessUniform, materialShininess);

		// geometry
		//translationMatrix = translate();
		Matrix.setIdentityM(translationMatrix, 0);
		Matrix.translateM(translationMatrix, 0, 4.0f, 1.9f, 10.0f);
		Matrix.setIdentityM(modelMatrix, 0);
		Matrix.multiplyMM(modelMatrix, 0, modelMatrix, 0, translationMatrix, 0);
		GLES32.glUniformMatrix4fv(modelMatrixUniform, 1, false, modelMatrix, 0);
		// bind vao
		GLES32.glBindVertexArray(vao_sphere[0]);
		
		// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
		GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);
		GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements, GLES32.GL_UNSIGNED_SHORT, 0);
		
		// unbind vao
		GLES32.glBindVertexArray(0);
		// *******************************************************

		// ***** 3rd sphere on 1st column, obsidian *****
		// ambient material
		materialAmbient[0] = 0.05375f; // r
		materialAmbient[1] = 0.05f;    // g
		materialAmbient[2] = 0.06625f; // b
		materialAmbient[3] = 1.0f;    // a
		

		// diffuse material
		materialDiffuse[0] = 0.18275f; // r
		materialDiffuse[1] = 0.17f;    // g
		materialDiffuse[2] = 0.22525f; // b
		materialDiffuse[3] = 1.0f;    // a
		

		// specular material
		materialSpecular[0] = 0.332741f; // r
		materialSpecular[1] = 0.328634f; // g
		materialSpecular[2] = 0.346435f; // b
		materialSpecular[3] = 1.0f;     // a
		

		// materialShininess
		materialShininess = 0.3f * 128.0f;
		

		// geometry
		GLES32.glUniform3fv(kaUniform, 1, materialAmbient, 0);
		GLES32.glUniform3fv(kdUniform, 1, materialDiffuse, 0);
		GLES32.glUniform3fv(ksUniform, 1, materialSpecular, 0);
		GLES32.glUniform1f(materialShininessUniform, materialShininess);

		// geometry
		//translationMatrix = translate(4.0f,0.65f, 10.0f);
				Matrix.setIdentityM(translationMatrix, 0);
		Matrix.translateM(translationMatrix, 0, 4.0f,0.65f, 10.0f);
		Matrix.setIdentityM(modelMatrix, 0);
		Matrix.multiplyMM(modelMatrix, 0, modelMatrix, 0, translationMatrix, 0);
		GLES32.glUniformMatrix4fv(modelMatrixUniform, 1, false, modelMatrix, 0);
		// bind vao
		GLES32.glBindVertexArray(vao_sphere[0]);
		
		// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
		GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);
		GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements, GLES32.GL_UNSIGNED_SHORT, 0);
		
		// unbind vao
		GLES32.glBindVertexArray(0);
		// *******************************************************

		// ***** 4th sphere on 1st column, pearl *****
		// ambient material
		materialAmbient[0] = 0.25f;    // r
		materialAmbient[1] = 0.20725f; // g
		materialAmbient[2] = 0.20725f; // b
		materialAmbient[3] = 1.0f;    // a
		

		// diffuse material
		materialDiffuse[0] = 1.0f;   // r
		materialDiffuse[1] = 0.829f; // g
		materialDiffuse[2] = 0.829f; // b
		materialDiffuse[3] = 1.0f;  // a
		

		// specular material
		materialSpecular[0] = 0.296648f; // r
		materialSpecular[1] = 0.296648f; // g
		materialSpecular[2] = 0.296648f; // b
		materialSpecular[3] = 1.0f;     // a
		

		// materialShininess
		materialShininess = 0.088f * 128.0f;
		

		// geometry
		GLES32.glUniform3fv(kaUniform, 1, materialAmbient, 0);
		GLES32.glUniform3fv(kdUniform, 1, materialDiffuse, 0);
		GLES32.glUniform3fv(ksUniform, 1, materialSpecular, 0);
		GLES32.glUniform1f(materialShininessUniform, materialShininess);

		// geometry
		//translationMatrix = translate(4.0f, -0.6f, 10.0f);
				Matrix.setIdentityM(translationMatrix, 0);
		Matrix.translateM(translationMatrix, 0, 4.0f, -0.6f, 10.0f);
		Matrix.setIdentityM(modelMatrix, 0);
		Matrix.multiplyMM(modelMatrix, 0, modelMatrix, 0, translationMatrix, 0);
		GLES32.glUniformMatrix4fv(modelMatrixUniform, 1, false, modelMatrix, 0);
		// bind vao
		GLES32.glBindVertexArray(vao_sphere[0]);
		
		// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
		GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);
		GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements, GLES32.GL_UNSIGNED_SHORT, 0);
		
		// unbind vao
		GLES32.glBindVertexArray(0);
		// *******************************************************

		// ***** 5th sphere on 1st column, ruby *****
		// ambient material
		materialAmbient[0] = 0.1745f;  // r
		materialAmbient[1] = 0.01175f; // g
		materialAmbient[2] = 0.01175f; // b
		materialAmbient[3] = 1.0f;    // a
		

		// diffuse material
		materialDiffuse[0] = 0.61424f; // r
		materialDiffuse[1] = 0.04136f; // g
		materialDiffuse[2] = 0.04136f; // b
		materialDiffuse[3] = 1.0f;    // a
		

		// specular material
		materialSpecular[0] = 0.727811f; // r
		materialSpecular[1] = 0.626959f; // g
		materialSpecular[2] = 0.626959f; // b
		materialSpecular[3] = 1.0f;     // a
		

		// materialShininess
		materialShininess = 0.6f * 128.0f;
		

		// geometry
		GLES32.glUniform3fv(kaUniform, 1, materialAmbient, 0);
		GLES32.glUniform3fv(kdUniform, 1, materialDiffuse, 0);
		GLES32.glUniform3fv(ksUniform, 1, materialSpecular, 0);
		GLES32.glUniform1f(materialShininessUniform, materialShininess);

		// geometry
		//translationMatrix = translate(4.0f, -1.85f, 10.0f);
		Matrix.setIdentityM(translationMatrix, 0);
		Matrix.translateM(translationMatrix, 0, 4.0f, -1.85f, 10.0f);
		Matrix.setIdentityM(modelMatrix, 0);
		Matrix.multiplyMM(modelMatrix, 0, modelMatrix, 0, translationMatrix, 0);
		GLES32.glUniformMatrix4fv(modelMatrixUniform, 1, false, modelMatrix, 0);
		// bind vao
		GLES32.glBindVertexArray(vao_sphere[0]);
		
		// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
		GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);
		GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements, GLES32.GL_UNSIGNED_SHORT, 0);
		
		// unbind vao
		GLES32.glBindVertexArray(0);
		// *******************************************************

		// ***** 6th sphere on 1st column, turquoise *****
		// ambient material
		materialAmbient[0] = 0.1f;     // r
		materialAmbient[1] = 0.18725f; // g
		materialAmbient[2] = 0.1745f;  // b
		materialAmbient[3] = 1.0f;    // a
		

		// diffuse material
		materialDiffuse[0] = 0.396f;   // r
		materialDiffuse[1] = 0.74151f; // g
		materialDiffuse[2] = 0.69102f; // b
		materialDiffuse[3] = 1.0f;    // a
		

		// specular material
		materialSpecular[0] = 0.297254f; // r
		materialSpecular[1] = 0.30829f;  // g
		materialSpecular[2] = 0.306678f; // b
		materialSpecular[3] = 1.0f;     // a
		

		// materialShininess
		materialShininess = 0.1f * 128.0f;
		

		// geometry
		GLES32.glUniform3fv(kaUniform, 1, materialAmbient, 0);
		GLES32.glUniform3fv(kdUniform, 1, materialDiffuse, 0);
		GLES32.glUniform3fv(ksUniform, 1, materialSpecular, 0);
		GLES32.glUniform1f(materialShininessUniform, materialShininess);

		// geometry
		//translationMatrix = translate(4.0f, -3.1f, 10.0f);
		// scaleMatrix = scale(1.0f, 1.0f, 0.8f);
				Matrix.setIdentityM(translationMatrix, 0);
		Matrix.translateM(translationMatrix, 0, 4.0f, -3.1f, 10.0f);
		Matrix.setIdentityM(modelMatrix, 0);
		Matrix.multiplyMM(modelMatrix, 0, modelMatrix, 0, translationMatrix, 0);
		GLES32.glUniformMatrix4fv(modelMatrixUniform, 1, false, modelMatrix, 0);
		// bind vao
		GLES32.glBindVertexArray(vao_sphere[0]);
		
		// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
		GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);
		GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements, GLES32.GL_UNSIGNED_SHORT, 0);
		
		// unbind vao
		GLES32.glBindVertexArray(0);
		// *******************************************************
		// *******************************************************
		// *******************************************************

		// ***** 1st sphere on 2nd column, brass *****
		// ambient material
		materialAmbient[0] = 0.329412f; // r
		materialAmbient[1] = 0.223529f; // g
		materialAmbient[2] = 0.027451f; // b
		materialAmbient[3] = 1.0f;     // a
		

		// diffuse material
		materialDiffuse[0] = 0.780392f; // r
		materialDiffuse[1] = 0.568627f; // g
		materialDiffuse[2] = 0.113725f; // b
		materialDiffuse[3] = 1.0f;     // a
		

		// specular material
		materialSpecular[0] = 0.992157f; // r
		materialSpecular[1] = 0.941176f; // g
		materialSpecular[2] = 0.807843f; // b
		materialSpecular[3] = 1.0f;     // a
		

		// materialShininess
		materialShininess = 0.21794872f * 128.0f;
		

		// geometry
		GLES32.glUniform3fv(kaUniform, 1, materialAmbient, 0);
		GLES32.glUniform3fv(kdUniform, 1, materialDiffuse, 0);
		GLES32.glUniform3fv(ksUniform, 1, materialSpecular, 0);
		GLES32.glUniform1f(materialShininessUniform, materialShininess);

		// geometry
		//translationMatrix = translate(1.0f,3.15f, 10.0f);
				Matrix.setIdentityM(translationMatrix, 0);
		Matrix.translateM(translationMatrix, 0, 1.0f,3.15f, 10.0f);
		Matrix.setIdentityM(modelMatrix, 0);
		Matrix.multiplyMM(modelMatrix, 0, modelMatrix, 0, translationMatrix, 0);
		GLES32.glUniformMatrix4fv(modelMatrixUniform, 1, false, modelMatrix, 0);
		// bind vao
		GLES32.glBindVertexArray(vao_sphere[0]);
		
		// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
		GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);
		GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements, GLES32.GL_UNSIGNED_SHORT, 0);
		
		// unbind vao
		GLES32.glBindVertexArray(0);
		// *******************************************************

		// ***** 2nd sphere on 2nd column, bronze *****
		// ambient material
		materialAmbient[0] = 0.2125f; // r
		materialAmbient[1] = 0.1275f; // g
		materialAmbient[2] = 0.054f;  // b
		materialAmbient[3] = 1.0f;   // a
		

		// diffuse material
		materialDiffuse[0] = 0.714f;   // r
		materialDiffuse[1] = 0.4284f;  // g
		materialDiffuse[2] = 0.18144f; // b
		materialDiffuse[3] = 1.0f;    // a
		

		// specular material
		materialSpecular[0] = 0.393548f; // r
		materialSpecular[1] = 0.271906f; // g
		materialSpecular[2] = 0.166721f; // b
		materialSpecular[3] = 1.0f;     // a
		

		// materialShininess
		materialShininess = 0.2f * 128.0f;
		

		// geometry
		GLES32.glUniform3fv(kaUniform, 1, materialAmbient, 0);
		GLES32.glUniform3fv(kdUniform, 1, materialDiffuse, 0);
		GLES32.glUniform3fv(ksUniform, 1, materialSpecular, 0);
		GLES32.glUniform1f(materialShininessUniform, materialShininess);

		// geometry
		//translationMatrix = translate(1.0f,1.9f, 10.0f);
				Matrix.setIdentityM(translationMatrix, 0);
		Matrix.translateM(translationMatrix, 0, 1.0f,1.9f, 10.0f);
		Matrix.setIdentityM(modelMatrix, 0);
		Matrix.multiplyMM(modelMatrix, 0, modelMatrix, 0, translationMatrix, 0);
		GLES32.glUniformMatrix4fv(modelMatrixUniform, 1, false, modelMatrix, 0);
		// bind vao
		GLES32.glBindVertexArray(vao_sphere[0]);
		
		// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
		GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);
		GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements, GLES32.GL_UNSIGNED_SHORT, 0);
		
		// unbind vao
		GLES32.glBindVertexArray(0);
		// *******************************************************

		// ***** 3rd sphere on 2nd column, chrome *****
		// ambient material
		materialAmbient[0] = 0.25f; // r
		materialAmbient[1] = 0.25f; // g
		materialAmbient[2] = 0.25f; // b
		materialAmbient[3] = 1.0f; // a
		

		// diffuse material
		materialDiffuse[0] = 0.4f;  // r
		materialDiffuse[1] = 0.4f;  // g
		materialDiffuse[2] = 0.4f;  // b
		materialDiffuse[3] = 1.0f; // a
		

		// specular material
		materialSpecular[0] = 0.774597f; // r
		materialSpecular[1] = 0.774597f; // g
		materialSpecular[2] = 0.774597f; // b
		materialSpecular[3] = 1.0f;     // a
		

		// materialShininess
		materialShininess = 0.6f * 128.0f;
		

		// geometry
		GLES32.glUniform3fv(kaUniform, 1, materialAmbient, 0);
		GLES32.glUniform3fv(kdUniform, 1, materialDiffuse, 0);
		GLES32.glUniform3fv(ksUniform, 1, materialSpecular, 0);
		GLES32.glUniform1f(materialShininessUniform, materialShininess);

		// geometry
		//translationMatrix = translate(1.0f,0.65f, 10.0f);
				Matrix.setIdentityM(translationMatrix, 0);
		Matrix.translateM(translationMatrix, 0, 1.0f,0.65f, 10.0f);
		Matrix.setIdentityM(modelMatrix, 0);
		Matrix.multiplyMM(modelMatrix, 0, modelMatrix, 0, translationMatrix, 0);
		GLES32.glUniformMatrix4fv(modelMatrixUniform, 1, false, modelMatrix, 0);
		// bind vao
		GLES32.glBindVertexArray(vao_sphere[0]);
		
		// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
		GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);
		GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements, GLES32.GL_UNSIGNED_SHORT, 0);
		
		// unbind vao
		GLES32.glBindVertexArray(0);
		// *******************************************************

		// ***** 4th sphere on 2nd column, copper *****
		// ambient material
		materialAmbient[0] = 0.19125f; // r
		materialAmbient[1] = 0.0735f;  // g
		materialAmbient[2] = 0.0225f;  // b
		materialAmbient[3] = 1.0f;    // a
		

		// diffuse material
		materialDiffuse[0] = 0.7038f;  // r
		materialDiffuse[1] = 0.27048f; // g
		materialDiffuse[2] = 0.0828f;  // b
		materialDiffuse[3] = 1.0f;    // a
		

		// specular material
		materialSpecular[0] = 0.256777f; // r
		materialSpecular[1] = 0.137622f; // g
		materialSpecular[2] = 0.086014f; // b
		materialSpecular[3] = 1.0f;     // a
		

		// materialShininess
		materialShininess = 0.1f * 128.0f;
		

		// geometry
		GLES32.glUniform3fv(kaUniform, 1, materialAmbient, 0);
		GLES32.glUniform3fv(kdUniform, 1, materialDiffuse, 0);
		GLES32.glUniform3fv(ksUniform, 1, materialSpecular, 0);
		GLES32.glUniform1f(materialShininessUniform, materialShininess);

		// geometry
		//translationMatrix = translate(1.0f, -0.6f, 10.0f);
				Matrix.setIdentityM(translationMatrix, 0);
		Matrix.translateM(translationMatrix, 0, 1.0f, -0.6f, 10.0f);
		Matrix.setIdentityM(modelMatrix, 0);
		Matrix.multiplyMM(modelMatrix, 0, modelMatrix, 0, translationMatrix, 0);
		GLES32.glUniformMatrix4fv(modelMatrixUniform, 1, false, modelMatrix, 0);
		// bind vao
		GLES32.glBindVertexArray(vao_sphere[0]);
		
		// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
		GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);
		GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements, GLES32.GL_UNSIGNED_SHORT, 0);
		
		// unbind vao
		GLES32.glBindVertexArray(0);
		// *******************************************************

		// ***** 5th sphere on 2nd column, gold *****
		// ambient material
		materialAmbient[0] = 0.24725f; // r
		materialAmbient[1] = 0.1995f;  // g
		materialAmbient[2] = 0.0745f;  // b
		materialAmbient[3] = 1.0f;    // a
		

		// diffuse material
		materialDiffuse[0] = 0.75164f; // r
		materialDiffuse[1] = 0.60648f; // g
		materialDiffuse[2] = 0.22648f; // b
		materialDiffuse[3] = 1.0f;    // a
		

		// specular material
		materialSpecular[0] = 0.628281f; // r
		materialSpecular[1] = 0.555802f; // g
		materialSpecular[2] = 0.366065f; // b
		materialSpecular[3] = 1.0f;     // a
		

		// materialShininess
		materialShininess = 0.4f * 128.0f;
		

		// geometry
		GLES32.glUniform3fv(kaUniform, 1, materialAmbient, 0);
		GLES32.glUniform3fv(kdUniform, 1, materialDiffuse, 0);
		GLES32.glUniform3fv(ksUniform, 1, materialSpecular, 0);
		GLES32.glUniform1f(materialShininessUniform, materialShininess);

		// geometry
		//translationMatrix = translate(1.0f, -1.85f, 10.0f);
				Matrix.setIdentityM(translationMatrix, 0);
		Matrix.translateM(translationMatrix, 0, 1.0f, -1.85f, 10.0f);
		Matrix.setIdentityM(modelMatrix, 0);
		Matrix.multiplyMM(modelMatrix, 0, modelMatrix, 0, translationMatrix, 0);
		GLES32.glUniformMatrix4fv(modelMatrixUniform, 1, false, modelMatrix, 0);
		// bind vao
		GLES32.glBindVertexArray(vao_sphere[0]);
		
		// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
		GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);
		GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements, GLES32.GL_UNSIGNED_SHORT, 0);
		
		// unbind vao
		GLES32.glBindVertexArray(0);
		// *******************************************************

		// ***** 6th sphere on 2nd column, silver *****
		// ambient material
		materialAmbient[0] = 0.19225f; // r
		materialAmbient[1] = 0.19225f; // g
		materialAmbient[2] = 0.19225f; // b
		materialAmbient[3] = 1.0f;    // a
		

		// diffuse material
		materialDiffuse[0] = 0.50754f; // r
		materialDiffuse[1] = 0.50754f; // g
		materialDiffuse[2] = 0.50754f; // b
		materialDiffuse[3] = 1.0f;    // a
		

		// specular material
		materialSpecular[0] = 0.508273f; // r
		materialSpecular[1] = 0.508273f; // g
		materialSpecular[2] = 0.508273f; // b
		materialSpecular[3] = 1.0f;     // a
		

		// materialShininess
		materialShininess = 0.4f * 128.0f;
		

		// geometry
		GLES32.glUniform3fv(kaUniform, 1, materialAmbient, 0);
		GLES32.glUniform3fv(kdUniform, 1, materialDiffuse, 0);
		GLES32.glUniform3fv(ksUniform, 1, materialSpecular, 0);
		GLES32.glUniform1f(materialShininessUniform, materialShininess);

		// geometry
		//translationMatrix = translate(1.0f, -3.1f, 10.0f);
				Matrix.setIdentityM(translationMatrix, 0);
		Matrix.translateM(translationMatrix, 0, 1.0f, -3.1f, 10.0f);
		Matrix.setIdentityM(modelMatrix, 0);
		Matrix.multiplyMM(modelMatrix, 0, modelMatrix, 0, translationMatrix, 0);
		GLES32.glUniformMatrix4fv(modelMatrixUniform, 1, false, modelMatrix, 0);
		// bind vao
		GLES32.glBindVertexArray(vao_sphere[0]);
		
		// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
		GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);
		GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements, GLES32.GL_UNSIGNED_SHORT, 0);
		
		// unbind vao
		GLES32.glBindVertexArray(0);
		// *******************************************************
		// *******************************************************
		// *******************************************************

		// ***** 1st sphere on 3rd column, black *****
		// ambient material
		materialAmbient[0] = 0.0f;  // r
		materialAmbient[1] = 0.0f;  // g
		materialAmbient[2] = 0.0f;  // b
		materialAmbient[3] = 1.0f; // a
		

		// diffuse material
		materialDiffuse[0] = 0.01f; // r
		materialDiffuse[1] = 0.01f; // g
		materialDiffuse[2] = 0.01f; // b
		materialDiffuse[3] = 1.0f; // a
		

		// specular material
		materialSpecular[0] = 0.50f; // r
		materialSpecular[1] = 0.50f; // g
		materialSpecular[2] = 0.50f; // b
		materialSpecular[3] = 1.0f; // a
		

		// materialShininess
		materialShininess = 0.25f * 128.0f;
		

		// geometry
		GLES32.glUniform3fv(kaUniform, 1, materialAmbient, 0);
		GLES32.glUniform3fv(kdUniform, 1, materialDiffuse, 0);
		GLES32.glUniform3fv(ksUniform, 1, materialSpecular, 0);
		GLES32.glUniform1f(materialShininessUniform, materialShininess);

		// geometry
		//translationMatrix = translate(-2.0f,3.15f, 10.0f);
				Matrix.setIdentityM(translationMatrix, 0);
		Matrix.translateM(translationMatrix, 0, -2.0f,3.15f, 10.0f);
		Matrix.setIdentityM(modelMatrix, 0);
		Matrix.multiplyMM(modelMatrix, 0, modelMatrix, 0, translationMatrix, 0);
		GLES32.glUniformMatrix4fv(modelMatrixUniform, 1, false, modelMatrix, 0);
		// bind vao
		GLES32.glBindVertexArray(vao_sphere[0]);
		
		// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
		GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);
		GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements, GLES32.GL_UNSIGNED_SHORT, 0);
		
		// unbind vao
		GLES32.glBindVertexArray(0);
		// *******************************************************

		// ***** 2nd sphere on 3rd column, cyan *****
		// ambient material
		materialAmbient[0] = 0.0f;  // r
		materialAmbient[1] = 0.1f;  // g
		materialAmbient[2] = 0.06f; // b
		materialAmbient[3] = 1.0f; // a
		

		// diffuse material
		materialDiffuse[0] = 0.0f;        // r
		materialDiffuse[1] = 0.50980392f; // g
		materialDiffuse[2] = 0.50980392f; // b
		materialDiffuse[3] = 1.0f;       // a
		

		// specular material
		materialSpecular[0] = 0.50196078f; // r
		materialSpecular[1] = 0.50196078f; // g
		materialSpecular[2] = 0.50196078f; // b
		materialSpecular[3] = 1.0f;       // a
		

		// materialShininess
		materialShininess = 0.25f * 128.0f;
		

		// geometry
		GLES32.glUniform3fv(kaUniform, 1, materialAmbient, 0);
		GLES32.glUniform3fv(kdUniform, 1, materialDiffuse, 0);
		GLES32.glUniform3fv(ksUniform, 1, materialSpecular, 0);
		GLES32.glUniform1f(materialShininessUniform, materialShininess);

		// geometry
		//translationMatrix = translate(-2.0f,1.9f, 10.0f);
				Matrix.setIdentityM(translationMatrix, 0);
		Matrix.translateM(translationMatrix, 0, -2.0f,1.9f, 10.0f);
		Matrix.setIdentityM(modelMatrix, 0);
		Matrix.multiplyMM(modelMatrix, 0, modelMatrix, 0, translationMatrix, 0);
		GLES32.glUniformMatrix4fv(modelMatrixUniform, 1, false, modelMatrix, 0);
		// bind vao
		GLES32.glBindVertexArray(vao_sphere[0]);
		
		// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
		GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);
		GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements, GLES32.GL_UNSIGNED_SHORT, 0);
		
		// unbind vao
		GLES32.glBindVertexArray(0);
		// *******************************************************

		// ***** 3rd sphere on 2nd column, green *****
		// ambient material
		materialAmbient[0] = 0.0f;  // r
		materialAmbient[1] = 0.0f;  // g
		materialAmbient[2] = 0.0f;  // b
		materialAmbient[3] = 1.0f; // a
		

		// diffuse material
		materialDiffuse[0] = 0.1f;  // r
		materialDiffuse[1] = 0.35f; // g
		materialDiffuse[2] = 0.1f;  // b
		materialDiffuse[3] = 1.0f; // a
		

		// specular material
		materialSpecular[0] = 0.45f; // r
		materialSpecular[1] = 0.55f; // g
		materialSpecular[2] = 0.45f; // b
		materialSpecular[3] = 1.0f; // a
		

		// materialShininess
		materialShininess = 0.25f * 128.0f;
		

		// geometry
		GLES32.glUniform3fv(kaUniform, 1, materialAmbient, 0);
		GLES32.glUniform3fv(kdUniform, 1, materialDiffuse, 0);
		GLES32.glUniform3fv(ksUniform, 1, materialSpecular, 0);
		GLES32.glUniform1f(materialShininessUniform, materialShininess);

		// geometry
		//translationMatrix = translate(-2.0f,0.65f, 10.0f);
				Matrix.setIdentityM(translationMatrix, 0);
		Matrix.translateM(translationMatrix, 0, -2.0f,0.65f, 10.0f);
		Matrix.setIdentityM(modelMatrix, 0);
		Matrix.multiplyMM(modelMatrix, 0, modelMatrix, 0, translationMatrix, 0);
		GLES32.glUniformMatrix4fv(modelMatrixUniform, 1, false, modelMatrix, 0);
		// bind vao
		GLES32.glBindVertexArray(vao_sphere[0]);
		
		// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
		GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);
		GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements, GLES32.GL_UNSIGNED_SHORT, 0);
		
		// unbind vao
		GLES32.glBindVertexArray(0);
		// *******************************************************

		// ***** 4th sphere on 3rd column, red *****
		// ambient material
		materialAmbient[0] = 0.0f;  // r
		materialAmbient[1] = 0.0f;  // g
		materialAmbient[2] = 0.0f;  // b
		materialAmbient[3] = 1.0f; // a
		

		// diffuse material
		materialDiffuse[0] = 0.5f;  // r
		materialDiffuse[1] = 0.0f;  // g
		materialDiffuse[2] = 0.0f;  // b
		materialDiffuse[3] = 1.0f; // a
		

		// specular material
		materialSpecular[0] = 0.7f;  // r
		materialSpecular[1] = 0.6f;  // g
		materialSpecular[2] = 0.6f;  // b
		materialSpecular[3] = 1.0f; // a
		

		// materialShininess
		materialShininess = 0.25f * 128.0f;
		

		// geometry
		GLES32.glUniform3fv(kaUniform, 1, materialAmbient, 0);
		GLES32.glUniform3fv(kdUniform, 1, materialDiffuse, 0);
		GLES32.glUniform3fv(ksUniform, 1, materialSpecular, 0);
		GLES32.glUniform1f(materialShininessUniform, materialShininess);

		// geometry
		//translationMatrix = translate(-2.0f, -0.6f, 10.0f);
				Matrix.setIdentityM(translationMatrix, 0);
		Matrix.translateM(translationMatrix, 0, -2.0f, -0.6f, 10.0f);
		Matrix.setIdentityM(modelMatrix, 0);
		Matrix.multiplyMM(modelMatrix, 0, modelMatrix, 0, translationMatrix, 0);
		GLES32.glUniformMatrix4fv(modelMatrixUniform, 1, false, modelMatrix, 0);
		// bind vao
		GLES32.glBindVertexArray(vao_sphere[0]);
		
		// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
		GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);
		GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements, GLES32.GL_UNSIGNED_SHORT, 0);
		
		// unbind vao
		GLES32.glBindVertexArray(0);
		// *******************************************************

		// ***** 5th sphere on 3rd column, white *****
		// ambient material
		materialAmbient[0] = 0.0f;  // r
		materialAmbient[1] = 0.0f;  // g
		materialAmbient[2] = 0.0f;  // b
		materialAmbient[3] = 1.0f; // a
		

		// diffuse material
		materialDiffuse[0] = 0.55f; // r
		materialDiffuse[1] = 0.55f; // g
		materialDiffuse[2] = 0.55f; // b
		materialDiffuse[3] = 1.0f; // a
		

		// specular material
		materialSpecular[0] = 0.70f; // r
		materialSpecular[1] = 0.70f; // g
		materialSpecular[2] = 0.70f; // b
		materialSpecular[3] = 1.0f; // a
		

		// materialShininess
		materialShininess = 0.25f * 128.0f;
		

		// geometry
		GLES32.glUniform3fv(kaUniform, 1, materialAmbient, 0);
		GLES32.glUniform3fv(kdUniform, 1, materialDiffuse, 0);
		GLES32.glUniform3fv(ksUniform, 1, materialSpecular, 0);
		GLES32.glUniform1f(materialShininessUniform, materialShininess);

		// geometry
		//translationMatrix = translate(-2.0f, -1.85f, 10.0f);
				Matrix.setIdentityM(translationMatrix, 0);
		Matrix.translateM(translationMatrix, 0, -2.0f, -1.85f, 10.0f);
		Matrix.setIdentityM(modelMatrix, 0);
		Matrix.multiplyMM(modelMatrix, 0, modelMatrix, 0, translationMatrix, 0);
		GLES32.glUniformMatrix4fv(modelMatrixUniform, 1, false, modelMatrix, 0);
		// bind vao
		GLES32.glBindVertexArray(vao_sphere[0]);
		
		// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
		GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);
		GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements, GLES32.GL_UNSIGNED_SHORT, 0);
		
		// unbind vao
		GLES32.glBindVertexArray(0);
		// *******************************************************

		// ***** 6th sphere on 3rd column, yellow plastic *****
		// ambient material
		materialAmbient[0] = 0.0f;  // r
		materialAmbient[1] = 0.0f;  // g
		materialAmbient[2] = 0.0f;  // b
		materialAmbient[3] = 1.0f; // a
		

		// diffuse material
		materialDiffuse[0] = 0.5f;  // r
		materialDiffuse[1] = 0.5f;  // g
		materialDiffuse[2] = 0.0f;  // b
		materialDiffuse[3] = 1.0f; // a
		

		// specular material
		materialSpecular[0] = 0.60f; // r
		materialSpecular[1] = 0.60f; // g
		materialSpecular[2] = 0.50f; // b
		materialSpecular[3] = 1.0f; // a
		

		// materialShininess
		materialShininess = 0.25f * 128.0f;
		

		// geometry
		GLES32.glUniform3fv(kaUniform, 1, materialAmbient, 0);
		GLES32.glUniform3fv(kdUniform, 1, materialDiffuse, 0);
		GLES32.glUniform3fv(ksUniform, 1, materialSpecular, 0);
		GLES32.glUniform1f(materialShininessUniform, materialShininess);

		// geometry
		//translationMatrix = translate(-2.0f, -3.1f, 10.0f);
				Matrix.setIdentityM(translationMatrix, 0);
		Matrix.translateM(translationMatrix, 0, -2.0f, -3.1f, 10.0f);
		Matrix.setIdentityM(modelMatrix, 0);
		Matrix.multiplyMM(modelMatrix, 0, modelMatrix, 0, translationMatrix, 0);
		GLES32.glUniformMatrix4fv(modelMatrixUniform, 1, false, modelMatrix, 0);
		// bind vao
		GLES32.glBindVertexArray(vao_sphere[0]);
		
		// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
		GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);
		GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements, GLES32.GL_UNSIGNED_SHORT, 0);
		
		// unbind vao
		GLES32.glBindVertexArray(0);
		// *******************************************************
		// *******************************************************
		// *******************************************************

		// ***** 1st sphere on 4th column, black *****
		// ambient material
		materialAmbient[0] = 0.02f; // r
		materialAmbient[1] = 0.02f; // g
		materialAmbient[2] = 0.02f; // b
		materialAmbient[3] = 1.0f; // a
		

		// diffuse material
		materialDiffuse[0] = 0.01f; // r
		materialDiffuse[1] = 0.01f; // g
		materialDiffuse[2] = 0.01f; // b
		materialDiffuse[3] = 1.0f; // a
		

		// specular material
		materialSpecular[0] = 0.4f;  // r
		materialSpecular[1] = 0.4f;  // g
		materialSpecular[2] = 0.4f;  // b
		materialSpecular[3] = 1.0f; // a
		

		// materialShininess
		materialShininess = 0.078125f * 128.0f;
		

		// geometry
		GLES32.glUniform3fv(kaUniform, 1, materialAmbient, 0);
		GLES32.glUniform3fv(kdUniform, 1, materialDiffuse, 0);
		GLES32.glUniform3fv(ksUniform, 1, materialSpecular, 0);
		GLES32.glUniform1f(materialShininessUniform, materialShininess);

		// geometry
		//translationMatrix = translate(-5.0f,3.15f, 10.0f);
		// scaleMatrix = scale(1.0f, 1.0f, 0.8f);
				Matrix.setIdentityM(translationMatrix, 0);
		Matrix.translateM(translationMatrix, 0, -5.0f,3.15f, 10.0f);
		Matrix.setIdentityM(modelMatrix, 0);
		Matrix.multiplyMM(modelMatrix, 0, modelMatrix, 0, translationMatrix, 0);
		GLES32.glUniformMatrix4fv(modelMatrixUniform, 1, false, modelMatrix, 0);
		// bind vao
		GLES32.glBindVertexArray(vao_sphere[0]);
		
		// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
		GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);
		GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements, GLES32.GL_UNSIGNED_SHORT, 0);
		
		// unbind vao
		GLES32.glBindVertexArray(0);
		// *******************************************************

		// ***** 2nd sphere on 4th column, cyan *****
		// ambient material
		materialAmbient[0] = 0.0f;  // r
		materialAmbient[1] = 0.05f; // g
		materialAmbient[2] = 0.05f; // b
		materialAmbient[3] = 1.0f; // a
		

		// diffuse material
		materialDiffuse[0] = 0.4f;  // r
		materialDiffuse[1] = 0.5f;  // g
		materialDiffuse[2] = 0.5f;  // b
		materialDiffuse[3] = 1.0f; // a
		

		// specular material
		materialSpecular[0] = 0.04f; // r
		materialSpecular[1] = 0.7f;  // g
		materialSpecular[2] = 0.7f;  // b
		materialSpecular[3] = 1.0f; // a
		

		// materialShininess
		materialShininess = 0.078125f * 128.0f;
		

		// geometry
		GLES32.glUniform3fv(kaUniform, 1, materialAmbient, 0);
		GLES32.glUniform3fv(kdUniform, 1, materialDiffuse, 0);
		GLES32.glUniform3fv(ksUniform, 1, materialSpecular, 0);
		GLES32.glUniform1f(materialShininessUniform, materialShininess);

		// geometry
		//translationMatrix = translate(-5.0f,1.9f, 10.0f);
				Matrix.setIdentityM(translationMatrix, 0);
		Matrix.translateM(translationMatrix, 0, -5.0f,1.9f, 10.0f);
		Matrix.setIdentityM(modelMatrix, 0);
		Matrix.multiplyMM(modelMatrix, 0, modelMatrix, 0, translationMatrix, 0);
		GLES32.glUniformMatrix4fv(modelMatrixUniform, 1, false, modelMatrix, 0);
		// bind vao
		GLES32.glBindVertexArray(vao_sphere[0]);
		
		// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
		GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);
		GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements, GLES32.GL_UNSIGNED_SHORT, 0);
		
		// unbind vao
		GLES32.glBindVertexArray(0);
		// *******************************************************

		// ***** 3rd sphere on 4th column, green *****
		// ambient material
		materialAmbient[0] = 0.0f;  // r
		materialAmbient[1] = 0.05f; // g
		materialAmbient[2] = 0.0f;  // b
		materialAmbient[3] = 1.0f; // a
		

		// diffuse material
		materialDiffuse[0] = 0.4f;  // r
		materialDiffuse[1] = 0.5f;  // g
		materialDiffuse[2] = 0.4f;  // b
		materialDiffuse[3] = 1.0f; // a
		

		// specular material
		materialSpecular[0] = 0.04f; // r
		materialSpecular[1] = 0.7f;  // g
		materialSpecular[2] = 0.04f; // b
		materialSpecular[3] = 1.0f; // a
		

		// materialShininess
		materialShininess = 0.078125f * 128.0f;
		

		// geometry
		GLES32.glUniform3fv(kaUniform, 1, materialAmbient, 0);
		GLES32.glUniform3fv(kdUniform, 1, materialDiffuse, 0);
		GLES32.glUniform3fv(ksUniform, 1, materialSpecular, 0);
		GLES32.glUniform1f(materialShininessUniform, materialShininess);

		// geometry
		//translationMatrix = translate(-5.0f,0.65f, 10.0f);
				Matrix.setIdentityM(translationMatrix, 0);
		Matrix.translateM(translationMatrix, 0, -5.0f,0.65f, 10.0f);
		Matrix.setIdentityM(modelMatrix, 0);
		Matrix.multiplyMM(modelMatrix, 0, modelMatrix, 0, translationMatrix, 0);
		GLES32.glUniformMatrix4fv(modelMatrixUniform, 1, false, modelMatrix, 0);
		// bind vao
		GLES32.glBindVertexArray(vao_sphere[0]);
		
		// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
		GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);
		GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements, GLES32.GL_UNSIGNED_SHORT, 0);
		
		// unbind vao
		GLES32.glBindVertexArray(0);
		// *******************************************************

		// ***** 4th sphere on 4th column, red *****
		// ambient material
		materialAmbient[0] = 0.05f; // r
		materialAmbient[1] = 0.0f;  // g
		materialAmbient[2] = 0.0f;  // b
		materialAmbient[3] = 1.0f; // a
		

		// diffuse material
		materialDiffuse[0] = 0.5f;  // r
		materialDiffuse[1] = 0.4f;  // g
		materialDiffuse[2] = 0.4f;  // b
		materialDiffuse[3] = 1.0f; // a
		

		// specular material
		materialSpecular[0] = 0.7f;  // r
		materialSpecular[1] = 0.04f; // g
		materialSpecular[2] = 0.04f; // b
		materialSpecular[3] = 1.0f; // a
		

		// materialShininess
		materialShininess = 0.078125f * 128.0f;
		

		// geometry
		GLES32.glUniform3fv(kaUniform, 1, materialAmbient, 0);
		GLES32.glUniform3fv(kdUniform, 1, materialDiffuse, 0);
		GLES32.glUniform3fv(ksUniform, 1, materialSpecular, 0);
		GLES32.glUniform1f(materialShininessUniform, materialShininess);

		// geometry
		//translationMatrix = translate(-5.0f, -0.6f, 10.0f);
				Matrix.setIdentityM(translationMatrix, 0);
		Matrix.translateM(translationMatrix, 0, -5.0f, -0.6f, 10.0f);
		Matrix.setIdentityM(modelMatrix, 0);
		Matrix.multiplyMM(modelMatrix, 0, modelMatrix, 0, translationMatrix, 0);
		GLES32.glUniformMatrix4fv(modelMatrixUniform, 1, false, modelMatrix, 0);
		// bind vao
		GLES32.glBindVertexArray(vao_sphere[0]);
		
		// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
		GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);
		GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements, GLES32.GL_UNSIGNED_SHORT, 0);
		
		// unbind vao
		GLES32.glBindVertexArray(0);
		// *******************************************************

		// ***** 5th sphere on 4th column, white *****
		// ambient material
		materialAmbient[0] = 0.05f; // r
		materialAmbient[1] = 0.05f; // g
		materialAmbient[2] = 0.05f; // b
		materialAmbient[3] = 1.0f; // a
		

		// diffuse material
		materialDiffuse[0] = 0.5f;  // r
		materialDiffuse[1] = 0.5f;  // g
		materialDiffuse[2] = 0.5f;  // b
		materialDiffuse[3] = 1.0f; // a
		

		// specular material
		materialSpecular[0] = 0.7f;  // r
		materialSpecular[1] = 0.7f;  // g
		materialSpecular[2] = 0.7f;  // b
		materialSpecular[3] = 1.0f; // a
		

		// materialShininess
		materialShininess = 0.078125f * 128.0f;
		

		// geometry
		GLES32.glUniform3fv(kaUniform, 1, materialAmbient, 0);
		GLES32.glUniform3fv(kdUniform, 1, materialDiffuse, 0);
		GLES32.glUniform3fv(ksUniform, 1, materialSpecular, 0);
		GLES32.glUniform1f(materialShininessUniform, materialShininess);

		// geometry
		//translationMatrix = translate(-5.0f, -1.85f, 10.0f);
				Matrix.setIdentityM(translationMatrix, 0);
		Matrix.translateM(translationMatrix, 0, -5.0f, -1.85f, 10.0f);
		Matrix.setIdentityM(modelMatrix, 0);
		Matrix.multiplyMM(modelMatrix, 0, modelMatrix, 0, translationMatrix, 0);
		GLES32.glUniformMatrix4fv(modelMatrixUniform, 1, false, modelMatrix, 0);
		// bind vao
		GLES32.glBindVertexArray(vao_sphere[0]);
		
		// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
		GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);
		GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements, GLES32.GL_UNSIGNED_SHORT, 0);
		
		// unbind vao
		GLES32.glBindVertexArray(0);
		// *******************************************************

		// ***** 6th sphere on 4th column, yellow rubber *****
		// ambient material
		materialAmbient[0] = 0.05f; // r
		materialAmbient[1] = 0.05f; // g
		materialAmbient[2] = 0.0f;  // b
		materialAmbient[3] = 1.0f; // a
		

		// diffuse material
		materialDiffuse[0] = 0.5f;  // r
		materialDiffuse[1] = 0.5f;  // g
		materialDiffuse[2] = 0.4f;  // b
		materialDiffuse[3] = 1.0f; // a
		

		// specular material
		materialSpecular[0] = 0.7f;  // r
		materialSpecular[1] = 0.7f;  // g
		materialSpecular[2] = 0.04f; // b
		materialSpecular[3] = 1.0f; // a
		

		// materialShininess
		materialShininess = 0.078125f * 128.0f;
		

		// geometry
		GLES32.glUniform3fv(kaUniform, 1, materialAmbient, 0);
		GLES32.glUniform3fv(kdUniform, 1, materialDiffuse, 0);
		GLES32.glUniform3fv(ksUniform, 1, materialSpecular, 0);
		GLES32.glUniform1f(materialShininessUniform, materialShininess);

		// geometry
		//translationMatrix = translate(-5.0f, -3.1f, 10.0f);
		// scaleMatrix = scale(1.0f, 1.0f, 0.8f);
				Matrix.setIdentityM(translationMatrix, 0);
		Matrix.translateM(translationMatrix, 0, -5.0f, -3.1f, 10.0f);
		Matrix.setIdentityM(modelMatrix, 0);
		Matrix.multiplyMM(modelMatrix, 0, modelMatrix, 0, translationMatrix, 0);
		GLES32.glUniformMatrix4fv(modelMatrixUniform, 1, false, modelMatrix, 0);
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
		if(singleTapCount == 1)
		{
			angleForXRotation += 5.0f;
			if(angleForXRotation >= 360.0f)
				angleForXRotation -= 360.0f;

			lightPosition[0] = 0.0f;
			lightPosition[1] = 50.0f * (float) Math.cos(angleForXRotation * Math.PI / 180.0f);
			lightPosition[2] = 50.0f * (float) Math.sin(angleForXRotation * Math.PI / 180.0f);
			lightPosition[3] = 1.0f;
		}
		else if(singleTapCount == 2)
		{
			angleForYRotation += 5.0f;
			if(angleForYRotation >= 360.0f)
				angleForYRotation -= 360.0f;

			lightPosition[0] = 50.0f * (float) Math.cos(angleForYRotation * Math.PI / 180.0f);
			lightPosition[1] = 0.0f;
			lightPosition[2] = 50.0f * (float) Math.sin(angleForYRotation * Math.PI / 180.0f);
			lightPosition[3] = 1.0f;
		}
		else if(singleTapCount == 3)
		{
			angleForZRotation += 5.0f;
			if(angleForZRotation >= 360.0f)
				angleForZRotation -= 360.0f;

			lightPosition[0] = 50.0f * (float) Math.cos(angleForZRotation * Math.PI / 180.0f);
			lightPosition[1] = 50.0f * (float) Math.sin(angleForZRotation * Math.PI / 180.0f);
			lightPosition[2] = 0.0f;
			lightPosition[3] = 1.0f;
		}
		else
		{
			lightPosition[0] = 0.0f;
			lightPosition[1] = 0.0f;
			lightPosition[2] = 10.0f;
			lightPosition[3] = 1.0f;
		}
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
		if(singleTapCount > 3)
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

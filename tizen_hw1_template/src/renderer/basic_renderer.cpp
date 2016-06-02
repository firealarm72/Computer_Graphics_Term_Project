#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>

#include "basic/basic_renderer.h"

using namespace std;
using namespace glm;

BasicRenderer::BasicRenderer() :
	mWidth(0),
	mHeight(0),
	mDeltaTime(0),
	mTimer(nullptr),
	mCamera(nullptr),
	mShader(nullptr),
	mIsAutoRotateEye(true),
	mIsFill(true),
	mVboVertices(0),
	mVboIndices(0),
	mVboInstTransforms(0),
	mInstCount(1)
{
	mTimer = new BasicTimer();
	mCamera = new BasicCamera();
	mShader = new BasicShader();
}

BasicRenderer::~BasicRenderer()
{
	delete mTimer;
	delete mShader;
	delete mCamera;
}

// Interface functions
/// Sets vertex shader and fragment shader for rendering
bool BasicRenderer::SetProgram(const std::string& vertexSource, const std::string& fragmentSource) const
{
	mShader->CreateProgram(vertexSource, fragmentSource);

	if (!mShader->GetProgram())
	{
		LOGE("Could not create program.\n");
		return false;
	}

	mShader->Use();

	return true;
}

bool BasicRenderer::SetProgram(const char* vertexSource, const char* fragmentSource) const
{
	mShader->CreateProgram(vertexSource, fragmentSource);

	if (!mShader->GetProgram())
	{
		LOGE("Could not create program.\n");
		return false;
	}

	mShader->Use();

	return true;
}


/****************************
 **** Interface functions ***
 ****************************/
void BasicRenderer::SetNewModel(const std::string& objSource, const float& scale)
{
	char* buffer = new char[objSource.length() + 1];
	strcpy(buffer, objSource.c_str());

	SetNewModel(buffer, scale);

	delete (buffer);
}

void BasicRenderer::SetNewModel(char* objSource)
{
	ImportModel(objSource, this);
}

void BasicRenderer::SetNewModel(char* objSource, const float& scale)
{
	ImportModelScale(objSource, this, scale);
}

bool BasicRenderer::Initialize()
{
	LOGI("Initialize renderer.\n");
	LogInfo();

	CountTickInit();

	CreateVbo();
	SetState();

	return true;
}

void BasicRenderer::SetViewPort(int w, int h)
{
	LOGI("SetViewPort(%d, %d)\n", w, h);
	mWidth = w;
	mHeight = h;
	glViewport(0, 0, w, h);
	check_gl_error("glViewport");

	mCamera->ComputePerspective(60.0f, w, h);
}

void BasicRenderer::RenderFrame()
{
	//LOGI("BasicRenderer::RenderFrame()");
	ComputeTick();

	//if (mIsAutoRotateEye) mCamera->RotateAuto(mDeltaTime);

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	check_gl_error("glClear");

	PassUniform();

	Draw();
}

void BasicRenderer::TogglePolygonMode()
{
#ifdef BASIC_OPENGL
    mIsFill = !mIsFill;
    glPolygonMode(GL_FRONT_AND_BACK, mIsFill ? GL_FILL : GL_LINE);
#endif
}


/*******************************
 ***** Rendering functions *****
 *******************************/
void BasicRenderer::SetState() const
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);
}

void BasicRenderer::CreateVbo()//create vertex buffer object for caching CPU mesh data in the GPU
{
	LOGI("BasicRenderer::RebindVbo\n");
	glGenBuffers(1, &mVboVertices);//returns 1(first parameter is the buffer size) buffer object names(mVboVertices) in buffers.
	glGenBuffers(1, &mVboIndices);//returns 1 buffer object names(mVboIndices) in buffers.
	//made the buffer for vertex buffer object vertices and indices
	//vertex array(mVboVertices) is specified by GL_ARRAY_BUFFER
	//index array(mVboIndices) is specified by GL_ELEMENT_ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, mVboVertices);//binds mVboVertices to the target, GL_ARRAY_BUFFER for vertex attributes
	glBufferData(GL_ARRAY_BUFFER,
	                            static_cast<GLsizeiptr>(mVertexBuffer.size() * sizeof(VertexStruct)),
	                            &(mVertexBuffer.at(0)), GL_STATIC_DRAW);
	//creates a new data store for the buffer object currently bound to GL_ARRAY_BUFFER with the size of mVertexBuffer * size of VertexStrct
	//the data in the location of mVertexBuffer.at(0) is copied
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVboIndices);//binds mVboIndices to the GL_ELEMENT_ARRAY_BUFFER
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
	                                    static_cast<GLsizeiptr>(mIndexBuffer.size() * sizeof(GLushort)),
	                                    &(mIndexBuffer.at(0)), GL_STATIC_DRAW);
	//creates a new data store for the buffer bound to GL_ELEMENT_ARRAY_BUFFER with size of mIndexBuffer * size of GLushort
	//the data in the located in mIndexBuffer.at(0) is copied

	int stride = sizeof(VertexStruct); // stride: sizeof(float) * number of components
	int offset = 0;
	glEnableVertexAttribArray(V_ATTRIB_POSITION);
	glVertexAttribPointer(V_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<GLvoid *>(offset));
	//inform the vertex shader of a array structure
	offset += sizeof(vec3); //for accessing normal
	glEnableVertexAttribArray(V_ATTRIB_NORMAL);
	glVertexAttribPointer(V_ATTRIB_NORMAL, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<GLvoid *>(offset));
}

void BasicRenderer::RebindVbo() const
{
	LOGI("BasicRenderer::RebindVbo\n");
	glBindBuffer(GL_ARRAY_BUFFER, mVboVertices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVboIndices);

	int stride = sizeof(VertexStruct); // stride: sizeof(float) * number of components
	int offset = 0;
	glEnableVertexAttribArray(V_ATTRIB_POSITION);
	glVertexAttribPointer(V_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<GLvoid *>(offset));

	offset += sizeof(vec3);
	glEnableVertexAttribArray(V_ATTRIB_NORMAL);
	glVertexAttribPointer(V_ATTRIB_NORMAL, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<GLvoid *>(offset));
}


void BasicRenderer::PassUniform() const
{
	mat4 worldMat = GetWorldMatrix();
	mat4 viewMat = mCamera->GetViewMat();
	mat4 projMat = mCamera->GetPerspectiveMat();

	mShader->SetUniform("worldMat", worldMat);
	mShader->SetUniform("viewMat", viewMat);
	mShader->SetUniform("projMat", projMat);
}

void BasicRenderer::Draw() const
{
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mIndexBuffer.size()), GL_UNSIGNED_SHORT, static_cast<GLvoid *>(nullptr));
	check_gl_error("glDrawElements");
}


/*****************************
 ***** Utility functions *****
 *****************************/
void BasicRenderer::LogInfo() const
{
	print_gl_string("Version", GL_VERSION);
	print_gl_string("Vendor", GL_VENDOR);
	print_gl_string("Renderer", GL_RENDERER);
	print_gl_string("Extensions", GL_EXTENSIONS);
	print_gl_string("GLSLversion", GL_SHADING_LANGUAGE_VERSION);
}

void BasicRenderer::CountTickInit() const
{
	mTimer->InitTimer();
}

void BasicRenderer::ComputeTick()
{
	static double lastTick = 0;
	double currTick = mTimer->GetElapsedTime();
	mDeltaTime = currTick - lastTick;
	lastTick = currTick;
	//LOGI("Tick: %f\n", mDeltaTime);
}

void BasicRenderer::ImportModel(char* objSource, BasicRenderer* renderer) const
{
	ImportModelScale(objSource, renderer, 1.0f);
}

void BasicRenderer::ImportModelScale(char* objSource, BasicRenderer* renderer, const float& scale) const
{
	LOGI("funcition : getObjModel\n");

	vec3 sVec = vec3(scale);

	vector<vec3> posCoords;
	vector<vec3> norCoords;
	vector<vec2> texCoords;

	vector<string> strIndexer;

	float x, y, z;
	char *line, *linePtr;
	line = util_strtok(objSource, "\r\n", &linePtr);
	while (line)
	{
		char *word, *wordPtr;
		word = util_strtok(line, " ", &wordPtr);

		switch (word[0])
		{
		case 'v':
			switch (word[1])
			{
			case 'n': //normal coordinate
				word = util_strtok(nullptr, " ", &wordPtr);
				x = static_cast<float>(atof(word));
				word = util_strtok(nullptr, " ", &wordPtr);
				y = static_cast<float>(atof(word));
				word = util_strtok(nullptr, " ", &wordPtr);
				z = static_cast<float>(atof(word));
				norCoords.push_back(vec3(x, y, z));
				break;
			case 't': //texture coordinate
				word = util_strtok(nullptr, " ", &wordPtr);
				x = static_cast<float>(atof(word));
				word = util_strtok(nullptr, " ", &wordPtr);
				y = static_cast<float>(atof(word));
				texCoords.push_back(vec2(x, y));
				break;
			default: //vertex position
				word = util_strtok(nullptr, " ", &wordPtr);
				x = static_cast<float>(atof(word));
				word = util_strtok(nullptr, " ", &wordPtr);
				y = static_cast<float>(atof(word));
				word = util_strtok(nullptr, " ", &wordPtr);
				z = static_cast<float>(atof(word));
				posCoords.push_back(sVec * vec3(x, y, z));
				break;
			}

			break;

		case 'f':
			while ((word = util_strtok(nullptr, " ", &wordPtr)))
			{
				string face = word;
				vector<string>::iterator findIter = find(strIndexer.begin(),
				                                                   strIndexer.end(), word);

				if (findIter != strIndexer.end())
				{
					// this face already in vertex data
					renderer->mIndexBuffer.push_back(std::distance(strIndexer.begin(), findIter));
				}
				else
				{
					unsigned short face_index = static_cast<unsigned short>(strIndexer.size());
					renderer->mIndexBuffer.push_back(face_index);
					strIndexer.push_back(face);

					VertexStruct newVertex;
					char *num, *numPtr;
					num = util_strtok(word, "/", &numPtr); // position index
					newVertex.pos = posCoords.at(atoi(num) - 1);
					num = util_strtok(nullptr, "/", &numPtr); // texture index
					newVertex.tex = texCoords.at(atoi(num) - 1);
					num = util_strtok(nullptr, "/", &numPtr); // normal index
					newVertex.nor = norCoords.at(atoi(num) - 1);

					renderer->mVertexBuffer.push_back(newVertex);
				}
			}

			break;

		case 'g':
			LOGI("name : %s\n", util_strtok(nullptr, " ", &wordPtr));
			break;

		default:
			break;
		}
		line = util_strtok(nullptr, "\r\n", &linePtr);
	}
}

mat4 BasicRenderer::GetWorldMatrix() const
{
	static quat startRotQuat = quat_cast(mat3(1));
	static quat lastRotQuat = startRotQuat;

	startRotQuat = lastRotQuat;

	mat4 rotationMat = mat4_cast(lastRotQuat);

	return rotationMat;
}


#ifndef __BASIC_RENDERER_H__
#define __BASIC_RENDERER_H__

#include <vector>

#include "basic_gl_set.h"
#include "basic_utils.h"
#include "basic_camera.h"
#include "basic_shader.h"
#include "basic_timer.h"

struct VertexStruct
{
	glm::vec3 pos;
	glm::vec3 nor;
	glm::vec2 tex;
};

class BasicRenderer
{
public:
	const int V_ATTRIB_POSITION = 0;
	const int V_ATTRIB_NORMAL = 1;

protected:
	int mWidth;
	int mHeight;
	double mDeltaTime;

	BasicTimer* mTimer;
	BasicCamera* mCamera;
	BasicShader* mShader;

	bool mIsAutoRotateEye;
	bool mIsFill;

	// vertex buffer
	std::vector<VertexStruct> mVertexBuffer;
	std::vector<GLushort> mIndexBuffer;

	// vertex buffer object and index buffer object
	GLuint mVboVertices;
	GLuint mVboIndices;
	GLuint mVboInstTransforms;

	int mInstCount;


public:
	BasicRenderer();

	~BasicRenderer();

	// App interface functions
	// GLES20
	bool SetProgram(const std::string& vertexSource, const std::string& fragmentSource) const;

	bool SetProgram(const char* vertexSource, const char* fragmentSource) const;

	// GLES31
	bool SetProgram(const char* vertexSource, const char* tessControlSource, const char* tessEvalSource,
	                const char* geometrySource, const char* fragmentSource) const;

	void SetNewModel(const std::string& objSource, const float& scale);

	void SetNewModel(char* objSource);

	void SetNewModel(char* objSource, const float& scale);

	bool Initialize();

	void SetViewPort(int w, int h);

	void RenderFrame();

	// CANNOT USE IN GLES
	void TogglePolygonMode();

private:// Rendering functions
	void SetState() const;

	void CreateVbo();

	void PassUniform() const;

	void Draw() const;

public:
	void RebindVbo() const;

private:// Utility functions
	void LogInfo() const;

	void CountTickInit() const;

	void ComputeTick();

	void ImportModel(char* objSource, BasicRenderer* renderer) const;

	void ImportModelScale(char* objSource, BasicRenderer* renderer, const float& scale) const;

private:// Touch handling funtions

	glm::mat4 GetWorldMatrix() const;

public:// inline functions
	BasicCamera* GetCamera() const;

	void SetTouchPoint(const float& x, const float& y);

	void OnAutoRotate();

	void OffAutoRotate();

	void ToggleRotate();

	void TouchOn();

	void TouchOff();
};

inline BasicCamera* BasicRenderer::GetCamera() const
{
	return mCamera;
}

inline void BasicRenderer::OnAutoRotate()
{
	mIsAutoRotateEye = true;
}

inline void BasicRenderer::OffAutoRotate()
{
	mIsAutoRotateEye = false;
}

inline void BasicRenderer::ToggleRotate()
{
	mIsAutoRotateEye = !mIsAutoRotateEye;
}

#endif //__BASIC_RENDERER_H__



#include "pch.h"
#include "PostProcessor.h"

//Number of samples to use for multisampling
const int numSamples = 8;

const GLfloat offset = 1.0f / 300.0f;

GLfloat offsets[9][2] = {
	{ -offset,  offset },  // top-left
	{ 0.0f,    offset },  // top-center
	{ offset,  offset },  // top-right
	{ -offset,  0.0f },  // center-left
	{ 0.0f,    0.0f },  // center-center
	{ offset,  0.0f },  // center - right
	{ -offset, -offset },  // bottom-left
	{ 0.0f,   -offset },  // bottom-center
	{ offset, -offset }   // bottom-right    
};

const GLint edge_kernel[9] = {
	-1, -1, -1,
	-1,  8, -1,
	-1, -1, -1
};

const GLfloat blur_kernel[9] = {
	1.0 / 16, 2.0 / 16, 1.0 / 16,
	2.0 / 16, 4.0 / 16, 2.0 / 16,
	1.0 / 16, 2.0 / 16, 1.0 / 16
};

const GLfloat vertices[] = {
	// Position    // Texture
	-1.0f, -1.0f, 0.0f, 0.0f,
	1.0f,  1.0f, 1.0f, 1.0f,
	-1.0f,  1.0f, 0.0f, 1.0f,

	-1.0f, -1.0f, 0.0f, 0.0f,
	1.0f, -1.0f, 1.0f, 0.0f,
	1.0f,  1.0f, 1.0f, 1.0f
};

PostProcessor::PostProcessor(Shader & shader, GLuint width, GLuint height) : mShader(&shader), mWidth(width), mHeight(height), mEdgeUsed(false), mBlurUsed(false), mWaveUsed(false)
{	
	//Init render data and uniforms
	InitRenderData();	
}

void PostProcessor::BeginRender()
{
	glBindFramebuffer(GL_FRAMEBUFFER, mMSFBO);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void PostProcessor::EndRender()
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, mMSFBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFBO);
	glBlitFramebuffer(0, 0, mWidth, mHeight, 0, 0, 
		mWidth, mHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PostProcessor::Render(GLfloat time)
{
	//Time not used yet
	UNREFERENCED_PARAMETER(time);
	mShader->Use();
	mShader->SetInteger("edge", mEdgeUsed);
	mShader->SetInteger("blur", mBlurUsed);
	mShader->SetInteger("wave", mWaveUsed);
	mShader->SetFloat("time", time);

	glActiveTexture(GL_TEXTURE0);
	mTexture.Bind();
	glBindVertexArray(mVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

bool PostProcessor::IsEdgeUsed()
{
	return mEdgeUsed;
}

void PostProcessor::SetIsEdgeUsed(bool used)
{
	mEdgeUsed = used;
}

bool PostProcessor::IsBlurUsed()
{
	return mBlurUsed;
}

void PostProcessor::SetIsBlurUsed(bool used)
{
	mBlurUsed = used;
}

bool PostProcessor::IsWaveUsed()
{
	return mWaveUsed;
}

void PostProcessor::SetIsWaveUsed(bool used)
{
	mWaveUsed = used;
}

void PostProcessor::InitRenderData()
{
	glGenFramebuffers(1, &mMSFBO);
	glGenFramebuffers(1, &mFBO);
	glGenRenderbuffers(1, &mRBO);

	BindFramebuffer(mWidth, mHeight);

	//Configure buffer and array objects
	GLuint VBO;
	
	glGenVertexArrays(1, &mVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(mVAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GL_FLOAT), reinterpret_cast<GLvoid*>(0));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	mShader->SetInteger("scene", 0, GL_TRUE);
	glUniform2fv(glGetUniformLocation(mShader->GetProgram(), "offsets"), 9, reinterpret_cast<GLfloat*>(offsets));
	glUniform1iv(glGetUniformLocation(mShader->GetProgram(), "edge_kernel"), 9, edge_kernel);
	glUniform1fv(glGetUniformLocation(mShader->GetProgram(), "blur_kernel"), 9, blur_kernel);
}

void PostProcessor::BindFramebuffer(GLuint width, GLuint height)
{
	mWidth = width;
	mHeight = height;

	//Initialize renderbuffer with multisampled color buffer
	glBindFramebuffer(GL_FRAMEBUFFER, mMSFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, mRBO);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, numSamples, GL_RGB, mWidth, mHeight);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, mRBO);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Failed to initialize MSFBO!" << std::endl;
	}

	//Initialize FBO/texture for blitting multisampled color buffer; used for shader operations
	glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
	mTexture.Generate(mWidth, mHeight, nullptr);
	//Attach texture to framebuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTexture.GetID(), 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Failed to initialize FBO!" << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


#include "cphipch.h"
#include "ShaderPipeline.h"

namespace Comphi::OpenGL {

	ShaderPipeline::ShaderPipeline() {
		// Init pipeline
		glGenProgramPipelines(1, &m_pipeline);
	}

	ShaderPipeline::~ShaderPipeline()
	{
		glDeleteProgramPipelines(1, &m_pipeline);
	}
	
	bool ShaderPipeline::InitPipeline()
	{
		return true;
	}

	bool ShaderPipeline::BindProgram(IShaderProgram& shaderProgram)
	{
		// Attach the first program to the vertex stage, and the second program
		// to the geometry and fragment stages
		//dynamic_cast<ShaderProgram*>(&shaderProgram)->GetTypeMask()
		//std::unique_ptr<ShaderProgram> shp = std::make_unique<ShaderProgram>(shaderProgram);
		//glUseProgramStages(m_pipeline, shp->GetTypeMask(), shaderProgram.shaderID);
		return true;
	}

	bool ShaderPipeline::UnbindProgram(IShaderProgram& shaderProgram)
	{
		//std::unique_ptr<ShaderProgram> shp = std::make_unique<ShaderProgram>(shaderProgram);
		//glUseProgramStages(m_pipeline, shp->GetTypeMask(), 0);
		return true;
	}

	bool ShaderPipeline::BindPipeline()
	{
		glBindProgramPipeline(m_pipeline);
		return true;
	}

}

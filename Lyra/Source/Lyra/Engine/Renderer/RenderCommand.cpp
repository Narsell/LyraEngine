#include "lrpch.h"

#include "RenderCommand.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"
#include "Renderer/Renderer.h"

namespace Lyra
{
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI();

	RenderCommand::RenderCommand(const RenderCommandData& commandData)
		: m_CommandData(commandData) {}

	RenderCommand::RenderCommand(VertexArray* vertexArray, const Ref<Material>& material, const Ref<const Scene>& scene, const glm::mat4& transform, bool drawIndexed, RenderType renderType)
	{
		m_CommandData.vertexArray = vertexArray;
		m_CommandData.material = material;
		m_CommandData.scene = scene;
		m_CommandData.transform = transform;
		m_CommandData.drawIndexed = drawIndexed;
		m_CommandData.renderType = renderType;
	}

	void RenderCommand::Execute()
	{
		SetSceneUniforms();
		SetMaterialUniforms();
		SetEntityUniforms();

		if (m_CommandData.vertexArray)
		{
			s_RendererAPI->DrawVertices(m_CommandData.vertexArray, m_CommandData.drawIndexed);
		}
	}

	void RenderCommand::SetSceneUniforms()
	{
		Ref<Shader> shader = m_CommandData.material->GetShader();
		// Only upload scene shader uniforms once per frame or if another shader uploaded these uniforms then I *think* we need to upload again.
		// Not 100% sure though, TODO: Not a big overhead but check this later when I'm wiser.
		if (!m_CommandData.scene->AreUniformsCached(shader))
		{
			m_CommandData.scene->UploadUniforms(shader);
		}
	}

	void RenderCommand::SetMaterialUniforms()
	{
		// Only bind new texture slots if the material has changed.
		if (!m_CommandData.material->IsCurrentlyBound())
		{
			m_CommandData.material->UploadUniforms();
			m_CommandData.material->BindTextures();
		}
	}

	void RenderCommand::SetEntityUniforms()
	{
		Ref<Shader> shader = m_CommandData.material->GetShader();

		if (!shader->IsCurrentlyBound())
		{
			shader->Bind();
		}
		shader->UploadUniform_Mat4f("u_Model", m_CommandData.transform);
		glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(m_CommandData.scene->GetCamera()->GetViewMatrix() * m_CommandData.transform)));
		shader->UploadUniform_Mat3f("u_Normal", normalMatrix);
	}
}
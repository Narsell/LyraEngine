#include "EditorLayer.h"

namespace Lyra
{
	class EditorApp : public Application
	{
	public:
		EditorApp(const ApplicationProps& props)
			: Application(props)
		{
			PushLayer(new EditorLayer());
		}
		~EditorApp() {}
	};

	Application* CreateApplication()
	{
		ApplicationProps props;
		props.windowProps = WindowProps("Lyra Editor");
		return new EditorApp(props);
	}
}
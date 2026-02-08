modded class PluginManager {
	override void Init() {
		super.Init();
		RegisterPlugin("PanelUIManager", true, false);
	}
}
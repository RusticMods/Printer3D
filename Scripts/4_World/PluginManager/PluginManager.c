modded class PluginManager {
	override void Init() {
		super.Init();
		RegisterPlugin("PanelUIManager", true, false);

#ifdef SERVER
		RegisterPlugin("PrinterRPCManager", false, true);
		RegisterPlugin("QueueManager", false, true);
#endif
	}
}

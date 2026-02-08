class Printer3D_USB: ItemBase
{
	static const string			AttachSound_SOUND = "cablereel_plugin_SoundSet";
	protected EffectSound 		m_AttachSound;

	override void OnWasAttached(EntityAI parent, int slot_id) 
	{
        super.OnWasAttached(parent, slot_id);
        
        PlaySoundSet(m_AttachSound, AttachSound_SOUND, 0, 0);
    }
		
	override void SetActions()
	{
		super.SetActions();
		
		AddAction(ActionAttach);
		AddAction(ActionDetach);
	};
};

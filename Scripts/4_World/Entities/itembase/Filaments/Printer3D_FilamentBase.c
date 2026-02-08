class FilamentBase: ItemBase
{
	override void SetActions()
	{
		super.SetActions();
		
		AddAction(ActionAttach);
		AddAction(ActionDetach);
	};
};

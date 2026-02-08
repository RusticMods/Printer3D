class Printer3D_FilingCabinet extends DeployableContainer_Base 
{
    override void SetActions()
	{
		super.SetActions();
		
        AddAction(ActionTogglePlaceObject);
        AddAction(ActionPlaceObject);
	};
}

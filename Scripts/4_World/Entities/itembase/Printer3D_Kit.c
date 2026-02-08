class Printer3D_KitBase extends ItemBase 
{
    override void OnPlacementComplete( Man player, vector position = "0 0 0", vector orientation = "0 0 0" ) 
	{		
		if ( GetGame().IsServer() )
		{	
			HideAllSelections();

            string targetClassname = GetDeployableClass();
            if (targetClassname != string.Empty)
            {
                ItemBase placedObject = ItemBase.Cast(GetGame().CreateObjectEx(targetClassname, position, ECE_PLACE_ON_SURFACE));
                if (placedObject)
                {
                    placedObject.SetOrientation(orientation);
                    placedObject.SetPosition(position);
                }
                
				SetSynchDirty();
            }
		}	
	}

	override bool IsBasebuildingKit()
    {
        return true;
    }

    string GetDeployableClass()
    {
        return string.Empty;
    }

	override bool IsDeployable()
	{
		return true;
	}

	override bool DoPlacingHeightCheck()
	{
		return false;
	}
	override float HeightCheckOverride()
	{
		return 20;
	}
	
	override void SetActions()
	{
		super.SetActions();
		
		AddAction(ActionTogglePlaceObject);
		AddAction(ActionDeployObject);
	}
}

class Printer3D_Printer_Kit extends Printer3D_KitBase
{
	override string GetDeployableClass()
	{
		return "Printer3D_Printer";
	}
}
class Printer3D_Printer_1000S_Kit extends Printer3D_KitBase
{
	override string GetDeployableClass()
	{
		return "Printer3D_Printer_1000_Slots";
	}
}
class Printer3D_FilingCabinet_Kit extends Printer3D_KitBase
{
	override string GetDeployableClass()
	{
		return "Printer3D_FilingCabinet";
	}
}
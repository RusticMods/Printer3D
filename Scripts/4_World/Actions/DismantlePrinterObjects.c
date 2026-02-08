class DismantlePrinterObjectsCB: ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime( 5 );
	}
}

class DismantlePrinterObjects: ActionContinuousBase
{
	void DismantlePrinterObjects()
	{
		m_CallbackClass = DismantlePrinterObjectsCB;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_CRAFTING;
	}

	override string GetText()
	{
        return "#str_dismantle";
	}

	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINonRuined();
		m_ConditionTarget = new CCTNonRuined();
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
        ItemBase target_item = ItemBase.Cast( target.GetObject() );

		array<string> printerObjects = new array<string>;
		printerObjects.Insert("Printer3D_Printer");
		printerObjects.Insert("Printer3D_Printer_1000_Slots");
		printerObjects.Insert("Printer3D_FilingCabinet");
		
		if (!target_item || !item)
		{
			return false;
		}

		int AttachmentCount = target_item.GetInventory().AttachmentCount();
		int ItemsCount = target_item.GetInventory().CountInventory();

		if (printerObjects.Find(target_item.GetType()) > -1 && item.IsKindOf("Screwdriver") && AttachmentCount == 0 && ItemsCount == 1)
		{	
			printerObjects.Clear();
			return true;
		}
		
		printerObjects.Clear();
		return false;
	}

	override void OnFinishProgressServer(ActionData action_data) 
	{
		ItemBase target_item = ItemBase.Cast(action_data.m_Target.GetObject());

    	if (!target_item)
    	{
    	    return;
    	}

    	string target_item_type = target_item.GetType();
    	string kit_type = "";

    	switch(target_item_type)
    	{
    	    case "Printer3D_Printer":
    	        kit_type = "Printer3D_Printer_Kit";
    	        break;
    	    case "Printer3D_Printer_1000_Slots":
    	        kit_type = "Printer3D_Printer_1000S_Kit";
    	        break;
    	    case "Printer3D_FilingCabinet":
    	        kit_type = "Printer3D_FilingCabinet_Kit";
    	        break;
    	    default:
    	        break;
    	}

    	if (kit_type != "")
    	{
    	    ItemBase item = GetGame().CreateObject(kit_type, target_item.GetPosition(), false);
    	    if (item)
    	    {
    	        target_item.Delete();
    	    }
    	}
	}
}


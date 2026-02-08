class OpenPrinterMenu: ActionInteractBase
{
	void OpenPrinterMenu()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ALL;
	}

	override string GetText()
	{
        return "#str_open_printer_menu";
	}

	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTNone;
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{		
        Object target_object = target.GetObject();

        Printer3D_Printer printer = Printer3D_Printer.Cast( target_object );

        if (printer)
		{
		    return true;
		}
        return false;
	}

    override void OnExecuteClient(ActionData action_data) 
    {
        Printer3D_Printer printer = Printer3D_Printer.Cast( action_data.m_Target.GetObject() );

        if (printer)
        {
            GetPanelUIManager().ShowMenu(PRINTER_MENU, printer); 
			PrinterMenu menu = GetPanelUIManager().GetMenu();
			menu.SetTarget(printer);
			menu.SetPlayer(action_data.m_Player);
			
			menu.SetMenuOpen(false);
        }
    }

	override void OnExecuteServer( ActionData action_data )
	{

	}
}


class PanelUIManager : PluginBase 
{
    private ref PrinterMenu menu;

    void ShowMenu(int menuId, ItemBase target) {
        if (GetGame().IsServer()) { return; }
        if (GetGame().GetUIManager().GetMenu()) {
            return;
        }

        if ( menu ) 
        {
            if (menu.IsMenuOpen()) 
            {
                //Hide Menu
                menu.SetMenuOpen(false);
                GetGame().GetUIManager().HideScriptedMenu(menu);
                UnlockControls();
            } else if (GetGame().GetUIManager().GetMenu() == NULL) {
                //Show Menu
                GetGame().GetUIManager().ShowScriptedMenu(menu, NULL);
                menu.SetMenuOpen(true);
                LockControls();
            }
        } else if (GetGame().GetUIManager().GetMenu() == NULL && menu == null) {
            //Create Menu
            LockControls();
            menu = PrinterMenu.Cast(GetGame().GetUIManager().EnterScriptedMenu(PRINTER_MENU, null));
            menu.SetMenuOpen(true);
        }
    }

    PrinterMenu GetMenu() 
    {
        if (menu) {
            return menu;
        }
        return null;
    }

    private void LockControls()
    {
        GetGame().GetMission().PlayerControlDisable(INPUT_EXCLUDE_ALL);
        GetGame().GetUIManager().ShowUICursor( true );
        GetGame().GetMission().GetHud().Show( false );
    }

    private void UnlockControls()
    {
        GetGame().GetMission().PlayerControlEnable(false);
        GetGame().GetInput().ResetGameFocus();
        GetGame().GetUIManager().ShowUICursor( false );
        GetGame().GetMission().GetHud().Show( true );
    }
}

PanelUIManager GetPanelUIManager() {
    return PanelUIManager.Cast(GetPlugin(PanelUIManager));
}





